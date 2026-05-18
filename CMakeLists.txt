# (C) Copyright 2026 UCAR

cmake_minimum_required( VERSION 3.14 FATAL_ERROR )

find_package( ecbuild 3.6 REQUIRED HINTS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../ecbuild )

project( lola-jedi VERSION 1.0.0 LANGUAGES C CXX )

set( ECBUILD_DEFAULT_BUILD_TYPE Release )
set( ENABLE_MPI ON CACHE BOOL "Compile with MPI" )
set( ENABLE_OMP ON CACHE BOOL "Compile with OpenMP" )

include( GNUInstallDirs )

# Source is under src/; include root is src/ so headers are found as lola/<file>.h
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src
                     ${CMAKE_CURRENT_BINARY_DIR}/src )

enable_testing()

add_subdirectory( src )
