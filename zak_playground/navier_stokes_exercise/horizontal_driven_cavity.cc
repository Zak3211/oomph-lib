//LIC// ====================================================================
//LIC// This file forms part of oomph-lib, the object-oriented, 
//LIC// multi-physics finite-element library, available 
//LIC// at http://www.oomph-lib.org.
//LIC// 
//LIC// Copyright (C) 2006-2025 Matthias Heil and Andrew Hazel
//LIC// 
//LIC// This library is free software; you can redistribute it and/or
//LIC// modify it under the terms of the GNU Lesser General Public
//LIC// License as published by the Free Software Foundation; either
//LIC// version 2.1 of the License, or (at your option) any later version.
//LIC// 
//LIC// This library is distributed in the hope that it will be useful,
//LIC// but WITHOUT ANY WARRANTY; without even the implied warranty of
//LIC// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//LIC// Lesser General Public License for more details.
//LIC// 
//LIC// You should have received a copy of the GNU Lesser General Public
//LIC// License along with this library; if not, write to the Free Software
//LIC// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//LIC// 02110-1301  USA.
//LIC// 
//LIC// The authors may be contacted at oomph-lib@maths.man.ac.uk.
//LIC// 
//LIC//====================================================================
//Driver for 2D rectangular driven cavity

//Generic includes

#include "generic.h"
#include "navier_stokes.h"

#include "meshes/simple_rectangular_quadmesh.h"


using namespace std;

using namespace oomph;
 

//==start_of_namespace==============================
/// Namespace for physical parameters
//==================================================
namespace Global_Physical_Variables
{

  // Reynolds number
  double Re=0;

} // end_of_namespace


//==start_of_problem_class============================================
/// Driven cavity problem in rectangular domain
//====================================================================
template<class ELEMENT>
class RectangularDrivenCavityProblem : public Problem
{

public:

/// Constructor
RectangularDrivenCavityProblem( int n_elements, double domain_length=1.0);

/// Destructor (empty)
~RectangularDrivenCavityProblem(){}

/// Fix pressure in element e at pressure dof pdof and set to pvalue
void fix_pressure(const unsigned &e, const unsigned &pdof, const double &pvalue) 
{
  dynamic_cast<ELEMENT*>(mesh_pt()->element_pt(e))->fix_pressure(pdof,pvalue);
} // end of fix_pressure


/// Update the after solve (empty)
void actions_after_newton_solve(){}


/// Update the problem specs before solve. 
/// Re-set velocity boundary conditions just to be on the safe side...
void actions_before_newton_solve()
{
  // Setup inflow on inflow boundary (3):
  unsigned ibound=3; 
  unsigned num_nod= mesh_pt()->nboundary_node(ibound);

  for (unsigned inod=0; inod < num_nod ; inod++)
  {

    // Setting up Parabolic inflow
    double x_pos = mesh_pt()->boundary_node_pt(ibound,inod)->x(1);

    // Getting the inflow velocity at that point
    double inflow_vel = -(60.0/4000.0)*x_pos*(x_pos-20.0);

    mesh_pt()->boundary_node_pt(ibound,inod)->set_value(0, inflow_vel);
    mesh_pt()->boundary_node_pt(ibound,inod)->set_value(1, 0.0);
  }
    
  // Apply no slip condition
  for (unsigned ibound=0;ibound<3;ibound+=2)
  {
    unsigned num_nod= mesh_pt()->nboundary_node(ibound);
    for (unsigned inod=0;inod<num_nod;inod++)
    {
      mesh_pt()->boundary_node_pt(ibound,inod)->set_value(0, 0.0);
      mesh_pt()->boundary_node_pt(ibound,inod)->set_value(1, 0.0);
    }
  }
} // end_of_actions_before_newton_solve

// Access function for the specific mesh
SimpleRectangularQuadMesh<ELEMENT>* mesh_pt() 
{
  return dynamic_cast<SimpleRectangularQuadMesh<ELEMENT>*>(Problem::mesh_pt());
}

/// Doc the solution
void doc_solution(DocInfo& doc_info);

}; // end_of_problem_class


//==start_of_constructor==================================================
/// Constructor for RectangularDrivenCavity problem 
//========================================================================
template<class ELEMENT>
RectangularDrivenCavityProblem<ELEMENT>::RectangularDrivenCavityProblem(int n_elements, double domain_length){ 

  // Setup mesh
  unsigned n_x = n_elements;
  unsigned n_y = n_elements;

  double l_x = domain_length;
  double l_y = domain_length;

  Problem::mesh_pt() = new SimpleRectangularQuadMesh<ELEMENT>(n_x,n_y,l_x,l_y);

  // Set the boundary conditions for this problem: All nodes are
  // free by default -- just pin the ones that have Dirichlet conditions here. 
  unsigned num_bound = mesh_pt()->nboundary();
  for(unsigned ibound=0;ibound<num_bound;ibound++){

    unsigned num_nod = mesh_pt()->nboundary_node(ibound);
    for (unsigned inod=0;inod<num_nod;inod++)
    {
      if (ibound != 1)  mesh_pt()->boundary_node_pt(ibound,inod)->pin(0); 
      mesh_pt()->boundary_node_pt(ibound,inod)->pin(1); 
    }

  } // end loop over boundaries

  // Set the Reynold's number for every element
  unsigned n_element = mesh_pt()->nelement();
  for (unsigned e=0; e < n_element; e++)
  {
    ELEMENT* el_pt = dynamic_cast<ELEMENT*>(mesh_pt()->element_pt(e));
    el_pt->re_pt() = &Global_Physical_Variables::Re;
  } // end loop over elements



  // Now set the first pressure value in element 0 to 0.0
  // hierher explore this!  
  //fix_pressure(0,0,0.0);

  // Setup equation numbering scheme
  cout <<"Number of equations: " << assign_eqn_numbers() << std::endl; 

} // end_of_constructor



//==start_of_doc_solution=================================================
/// Doc the solution
//========================================================================
template<class ELEMENT>
void RectangularDrivenCavityProblem<ELEMENT>::doc_solution(DocInfo& doc_info)
{ 
  ofstream some_file;
  char filename[100];

  // Number of plot points
  unsigned npts;
  npts=5; 

  // Output solution 
  snprintf(filename, sizeof(filename), "%s/soln%i.dat",doc_info.directory().c_str(),
          doc_info.number());
  some_file.open(filename);
  mesh_pt()->output(some_file,npts);
  some_file.close();

} // end_of_doc_solution



//==start_of_main======================================================
/// Driver for RectangularDrivenCavity test problem -- test drive
/// with two different types of element.
//=====================================================================
int main(int argc, char* argv[])
{

  // Set up doc info
  DocInfo doc_info;
  doc_info.set_directory("RESLT");
  doc_info.number() = 0;

  // Doing QTaylorHoodElements
  RectangularDrivenCavityProblem<QTaylorHoodElement<2>> problem(10, 20);
  
  for (unsigned i = 0; i < 5; i++) 
  {
    std::cout << "Solving for Reynolds" << Global_Physical_Variables::Re << std::endl;
    problem.newton_solve();
    problem.doc_solution(doc_info);
    doc_info.number()++;
    Global_Physical_Variables::Re += 10.0;
  }
 } // end_of_main


 // Change the inflow profile to a blunt profile (depends on Reynold's number)
 // Make the channel longer
 // Put the temperature back in