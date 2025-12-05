# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitclone-lastrun.txt" AND EXISTS "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitinfo.txt" AND
  "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitclone-lastrun.txt" IS_NEWER_THAN "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --depth 1 --no-single-branch --config "advice.detachedHead=false" "https://github.com/OpenMathLib/OpenBLAS.git" "openblas"
    WORKING_DIRECTORY "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/OpenMathLib/OpenBLAS.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "v0.3.29" --
  WORKING_DIRECTORY "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v0.3.29'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitinfo.txt" "/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/zak3211/projects/oomph-lib/external_distributions/build/openblas-prefix/src/openblas-stamp/openblas-gitclone-lastrun.txt'")
endif()
