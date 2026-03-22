

#include "generic.h"
#include "navier_stokes.h"
#include "advection_diffusion.h"
#include "meshes/rectangular_quadmesh.h"


using namespace std;
using namespace oomph;

namespace Global_Physical_Variables
{
  double Peclet=1.0;
  double Inverse_Prandtl=1.0;
  double Rayleigh=1800.0;
  Vector<double> Direction_of_gravity(2);
}


template<class ELEMENT>
class HeatedDrivenCavityProblem : public Problem
{
    public:
    
    // Constructor 
    HeatedDrivenCavityProblem();

    // Empty destructor
    ~HeatedDrivenCavityProblem(){}

    // To be implemented
    void actions_before_newton_solve();

    // Empty function
    void actions_after_newton_solve(){}

    // Mesh accessor function
    RectangularQuadMesh<ELEMENT>* mesh_pt();

    // Empty Doc function
    void doc_solution(DocInfo& doc_info){};  

}; // End of problem class


// Start of constructor
template<class ELEMENT>
HeatedDrivenCavityProblem<ELEMENT>::HeatedDrivenCavityProblem()
{

    unsigned Nx = 10;
    unsigned Ny = 20;

    double Lx = 0.1;
    double Ly = 0.1;

    Problem::mesh_pt() = new RectangularQuadMesh<ELEMENT>(Nx, Ny, Lx, Ly);

    // Pin constant components
    unsigned num_bound = mesh_pt()->nboundary();
    for (unsigned ibound=0; ibound<num_bound ;ibound++)
    {
        unsigned num_nod = mesh_pt()->nboundary_node(ibound);
        for (unsigned inod=0; inod < num_nod; inod++)
        {
            mesh_pt()->boundary_node_pt(ibound,inod)->pin(1); 
            if (ibound!=1)  mesh_pt()->boundary_node_pt(ibound,inod)->pin(0); 
        }
    }

    // Set the Physics constants
    unsigned n_element = mesh_pt()->nelement();
    for (unsigned e=0; e < n_element; e++)
    {
        ELEMENT *el_pt = dynamic_cast<ELEMENT*>(mesh_pt()->element_pt(e));
        
        el_pt->pe_pt() = &Global_Physical_Variables::Peclet;
        el_pt->pe_st_pt() = &Global_Physical_Variables::Peclet;
        el_pt->re_pt() = &Global_Physical_Variables::Inverse_Prandtl;
        el_pt->re_st_pt() = &Global_Physical_Variables::Inverse_Prandtl;
        el_pt->ra_pt() = &Global_Physical_Variables::Rayleigh;
        el_pt->g_pt() = &Global_Physical_Variables::Direction_of_gravity;
        el_pt->disable_ALE();
    }

} // End of constructor



// Start of actions_before_newton_solve
template<class ELEMENT>
void HeatedDrivenCavityProblem<ELEMENT>::actions_before_newton_solve()
{

    unsigned inflow_boundary = 3;
    unsigned outflow_boundary = 1;
    unsigned node_count = mesh_pt()->nboundary_node(inflow_boundary);

    for (unsigned node_id=0; node_id < node_count; node_id++)
    {
        // Constant Inflow
        // mesh_pt()->boundary_node_pt(inflow_boundary, node_id)->set_value(0, 1.0);
        // mesh_pt()->boundary_node_pt(inflow_boundary, node_id)->set_value(1, 0.0);

        // Set up Parabolic Inflow
        double x_pos = (node_id*20)/node_count;

        // Getting the inflow velocity at that point
        double inflow_vel = -(60/4000)*x_pos*(x_pos-20);

        
    }

    // Default other boundary node values to 0.0
    // Outflow on boundary 1
    // Temperature allowed to vary everywhere except on heated surface
    unsigned boundary_count = mesh_pt()->nboundary();
    for (unsigned boundary_id=0; boundary_id < boundary_count; boundary_id++)
    {
        if (boundary_id == inflow_boundary) continue;
        unsigned node_count = mesh_pt()->nboundary_node(boundary_id);
        for (unsigned node_id{}; node_id < node_count; node_id++)
        {
            mesh_pt()->boundary_node_pt(boundary_id, node_id)->set_value(0, 0.0);
            mesh_pt()->boundary_node_pt(boundary_id, node_id)->set_value(1, 0.0);

            if (boundary_id == outflow_boundary)
            {
                double curr_pos = node_id*0.1;
                if (0.5 <= curr_pos <= 1.5) mesh_pt()->boundary_node_pt(boundary_id, node_id)->set_value(2, 0.5);
            }
        }
    }
} // End of actions_before_newton_solve


// Start of mesh_pt
template<class ELEMENT>
RectangularQuadMesh<ELEMENT>* HeatedDrivenCavityProblem<ELEMENT>::mesh_pt()
{
    return dynamic_cast<RectangularQuadMesh<ELEMENT>*>(Problem::mesh_pt());
} // End of mesh_pt



// Start of main
int main(int argc, char* argv[])
{   

    return 0;
    // Set the direction of gravity
    Global_Physical_Variables::Direction_of_gravity[0] = 0.0;
    Global_Physical_Variables::Direction_of_gravity[1] = -1.0;

} // End of main