# (C) Copyright 2024 UCAR
#

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

#option(BUILD_GSIBEC "Build GSIbec" OFF)
#if(BUILD_GSIBEC)
#  ecbuild_bundle( PROJECT gsibec   GIT "https://github.com/geos-esm/GSIbec" TAG 1.4.2 )
#endif()
#
#option(BUILD_RTTOV "Build RTTOV" OFF)
#if(BUILD_RTTOV)
#  ecbuild_bundle( PROJECT rttov    GIT "https://github.com/jcsda-internal/rttov" BRANCH develop UPDATE )
#endif()
#
#option(BUILD_OASIM "Build OASIM" OFF)
#if(BUILD_OASIM)
#  ecbuild_bundle( PROJECT oasim    GIT "https://github.com/jcsda-internal/oasim.git" BRANCH develop UPDATE )
#endif()
#
#option(BUILD_ROPP "Build ROPP" OFF)
#if(BUILD_ROPP)
#  ecbuild_bundle( PROJECT ropp-ufo GIT "https://github.com/jcsda-internal/ropp-test.git"  BRANCH develop UPDATE )
#endif()
#
#ecbuild_bundle( PROJECT gsw      GIT "https://github.com/jcsda-internal/GSW-Fortran.git" BRANCH develop UPDATE )

ecbuild_bundle( PROJECT oops     GIT "https://github.com/jcsda-internal/oops.git"        TAG abb9223e695f7fde265e052c824392be0b2b7032 )

#ecbuild_bundle( PROJECT jedi-model-data GIT "https://github.com/jcsda-internal/jedi-model-data.git"  BRANCH develop UPDATE )
ecbuild_bundle( PROJECT vader    GIT "https://github.com/jcsda-internal/vader.git"       TAG d413aa52940fa12fdabfc97f50df36a89e55008c )
ecbuild_bundle( PROJECT saber    GIT "https://github.com/jcsda-internal/saber.git"       TAG f155f4260004a5152db60ccdfd64f1eabc3bc47a )

# mist now lives in its own repository (previously part of oops feature/mist branches)
ecbuild_bundle( PROJECT mist     GIT "https://github.com/jcsda-internal/mist.git"        TAG af5c1d9726e65ed46c2866638c84a93bfb844cef )

ecbuild_bundle( PROJECT crtm     GIT "https://github.com/jcsda/CRTMv3.git"               TAG 9901de139d02a09e615c0e6a9b244fd2c2494e26 )

option(ENABLE_IODA_DATA "Obtain ioda test data from ioda-data repository (vs tarball)" ON)
ecbuild_bundle( PROJECT ioda-data GIT "https://github.com/jcsda-internal/ioda-data.git"  TAG 75b42686c53cb2f803a7b7d79f307440b67c2471 )
ecbuild_bundle( PROJECT ioda     GIT "https://github.com/jcsda-internal/ioda.git"        TAG ee9e0861678fddc0f56f8402a0fc5c8f87ac6a3a )

option(ENABLE_UFO_DATA "Obtain ufo test data from ufo-data repository (vs tarball)" ON)
ecbuild_bundle( PROJECT ufo-data GIT "https://github.com/jcsda-internal/ufo-data.git"    TAG ca0d0e8746a1f1c88cb4ea8cd6af4918e264bc43 )
ecbuild_bundle( PROJECT ufo      GIT "https://github.com/jcsda-internal/ufo.git"         TAG b4688e1b48b4706c47bf6444eda4e20c626d7f4d )


ecbuild_bundle( PROJECT lola-jedi      GIT "https://github.com/mer-a-o/lola-jedi.git"         TAG 70ebdffc99e5bdbb09912fe4cdf21327f6cbe3b8 )

# Build IODA converters if requested
#option(BUILD_IODA_CONVERTERS "Build IODA Converters" OFF)
#if(BUILD_IODA_CONVERTERS)
#  ecbuild_bundle( PROJECT iodaconv GIT "https://github.com/jcsda-internal/ioda-converters.git" BRANCH develop UPDATE )
#endif()
#
#ecbuild_bundle( PROJECT fv3-jedi-lm GIT "https://github.com/jcsda-internal/fv3-jedi-linearmodel.git"      BRANCH develop UPDATE )
#
#option(ENABLE_FV3_JEDI_DATA "Obtain fv3-jedi test data from fv3-jedi-data repository (vs tarball)" ON)
#ecbuild_bundle( PROJECT fv3-jedi-data  GIT "https://github.com/jcsda-internal/fv3-jedi-data.git"          BRANCH develop UPDATE )
#ecbuild_bundle( PROJECT fv3-jedi    GIT "https://github.com/jcsda-internal/fv3-jedi.git"                  BRANCH develop UPDATE )
#
#ecbuild_bundle( PROJECT soca        GIT "https://github.com/jcsda-internal/soca.git"            BRANCH develop UPDATE RECURSIVE )
#
#option(BUILD_MPAS "Build mpas-jedi" ON)
#if(BUILD_MPAS)
#  set(MPAS_DOUBLE_PRECISION "ON" CACHE STRING "MPAS-Model: Use double precision 64-bit Floating point.")
#  set(MPAS_CORES init_atmosphere atmosphere CACHE STRING "MPAS-Model: cores to build.")
#  set(MPAS_OPENMP "ON" CACHE STRING "MPAS-Model: Enable OpenMP.")
#
#  # Using a commit from MPAS-model release-v8.4.0 branch which contains the fix for MPAS-model#1420. Once an offical tag with this fix exists we should use that instead.
#  ecbuild_bundle( PROJECT mpas           GIT "https://github.com/MPAS-Dev/MPAS-Model.git"     TAG v8.4.0 )
#  ecbuild_bundle( PROJECT mpas-jedi-data GIT "https://github.com/jcsda-internal/mpas-jedi-data.git" BRANCH develop UPDATE )
#  ecbuild_bundle( PROJECT mpas-jedi      GIT "https://github.com/jcsda-internal/mpas-jedi.git"      BRANCH develop UPDATE )
#endif()
#
#ecbuild_bundle( PROJECT coupling   GIT "https://github.com/jcsda-internal/coupling.git"           BRANCH develop UPDATE )
#
#option(BUILD_PYIRI "Build PyIRI-jedi" OFF)
#if(BUILD_PYIRI)
#  ecbuild_bundle( PROJECT pyiri-jedi  GIT "https://github.com/jcsda-internal/pyiri-jedi.git"  BRANCH develop UPDATE RECURSIVE )
#endif()

ecbuild_bundle_finalize()

