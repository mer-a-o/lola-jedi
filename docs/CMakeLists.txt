cmake_minimum_required( VERSION 3.14 FATAL_ERROR )

find_package( ecbuild 3.6 REQUIRED HINTS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../ecbuild)

project( jedi-bundle VERSION 8.0.0 LANGUAGES C CXX Fortran )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include( ecbuild_bundle )

# Default release mode
set( ECBUILD_DEFAULT_BUILD_TYPE Release )

# Enable OpenMP and MPI
set( ENABLE_MPI ON CACHE BOOL "Compile with MPI" )
set( ENABLE_OMP ON CACHE BOOL "Compile with OpenMP" )

# Stop search as soon as Python satisfying version constraints is found.
set( Python3_FIND_STRATEGY LOCATION )

# Depend path for non-ecbuild packages
set(DEPEND_LIB_ROOT ${CMAKE_CURRENT_BINARY_DIR}/Depends)
list(APPEND CMAKE_PREFIX_PATH ${DEPEND_LIB_ROOT})

# Library path for non-ecbuild packages
link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

include( GNUInstallDirs )
if(APPLE)
  list( APPEND CMAKE_INSTALL_RPATH $ENV{llvm_openmp_ROOT}/lib )
endif()

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# when building, already use the install RPATH
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

# Define bundle
ecbuild_bundle_initialize()

# Use external jedi-cmake
include( $ENV{jedi_cmake_ROOT}/share/jedicmake/Functions/git_functions.cmake )

#ecbuild_bundle( PROJECT eckit    GIT "https://github.com/ecmwf/eckit.git" TAG 1.24.4 )
#ecbuild_bundle( PROJECT fckit    GIT "https://github.com/ecmwf/fckit.git" TAG 0.11.0 )
#ecbuild_bundle( PROJECT atlas    GIT "https://github.com/ecmwf/atlas.git" TAG 0.35.0 )


ecbuild_bundle( PROJECT oops     GIT "https://github.com/jcsda-internal/oops.git"        BRANCH feature/mist_demo_using_base UPDATE )

ecbuild_bundle( PROJECT vader    GIT "https://github.com/jcsda-internal/vader.git"       BRANCH develop UPDATE )
ecbuild_bundle( PROJECT saber    GIT "https://github.com/jcsda-internal/saber.git"       TAG 8571579e )  # pinned: last commit before oops::Diffusion::VerticalMethod dependency (PR #1234)


ecbuild_bundle( PROJECT crtm     GIT "https://github.com/jcsda/CRTMv3.git"               BRANCH develop UPDATE )

option(ENABLE_IODA_DATA "Obtain ioda test data from ioda-data repository (vs tarball)" ON)
ecbuild_bundle( PROJECT ioda-data GIT "https://github.com/jcsda-internal/ioda-data.git"  BRANCH develop UPDATE )
ecbuild_bundle( PROJECT ioda     GIT "https://github.com/jcsda-internal/ioda.git"        BRANCH develop UPDATE )

option(ENABLE_UFO_DATA "Obtain ufo test data from ufo-data repository (vs tarball)" ON)
ecbuild_bundle( PROJECT ufo-data GIT "https://github.com/jcsda-internal/ufo-data.git"    BRANCH develop UPDATE )
ecbuild_bundle( PROJECT ufo      GIT "https://github.com/jcsda-internal/ufo.git"         TAG a13a70e42 )  # pinned: last commit before shared_ptr→reference change in ObsFilters (PR #4112)


ecbuild_bundle( PROJECT lola-jedi      GIT "https://github.com/mer-a-o/lola-jedi.git"         BRANCH main UPDATE )


ecbuild_bundle_finalize()

