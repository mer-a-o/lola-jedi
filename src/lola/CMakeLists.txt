
list(APPEND lola_src_files
     Increment.cc
     Increment.h
     IOUtils.h
     Model.h
     State.cc
     State.h
     Traits.h
     )

ecbuild_add_library(TARGET lola
                    SOURCES ${lola_src_files}
                    PUBLIC_LIBS oops saber vader ioda ufo mist_base mist_utils
                    LINKER_LANGUAGE CXX
                    )

# Include root is src/ so headers resolve as lola/<file>.h
target_include_directories(lola PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>)

target_compile_features(lola PUBLIC cxx_std_17)

target_link_libraries(lola PUBLIC mist_api mist_base)

add_subdirectory(executables)
add_subdirectory(tests)
