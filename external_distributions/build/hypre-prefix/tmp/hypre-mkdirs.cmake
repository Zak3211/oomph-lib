# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/zak3211/projects/oomph-lib/external_distributions/build/hypre-prefix/src/hypre"
  "/home/zak3211/projects/oomph-lib/external_distributions/build/hypre-prefix/src/hypre-build"
  "/home/zak3211/projects/oomph-lib/external_distributions/install/hypre"
  "/home/zak3211/projects/oomph-lib/external_distributions/build/hypre-prefix/tmp"
  "/home/zak3211/projects/oomph-lib/external_distributions/build/hypre-prefix/src/hypre-stamp"
  "/home/zak3211/projects/oomph-lib/external_distributions/build/hypre-prefix/src"
  "/home/zak3211/projects/oomph-lib/external_distributions/build/logs"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/zak3211/projects/oomph-lib/external_distributions/build/hypre-prefix/src/hypre-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/zak3211/projects/oomph-lib/external_distributions/build/hypre-prefix/src/hypre-stamp${cfgdir}") # cfgdir has leading slash
endif()
