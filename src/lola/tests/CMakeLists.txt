
# Macro to symlink test input files from source tree into the build directory
macro(LINK_FILES filelist src_dir dst_dir)
  foreach(FILENAME ${filelist})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${src_dir}/${FILENAME} ${dst_dir}/${FILENAME})
  endforeach(FILENAME)
endmacro()

list(APPEND lola_testinput
     testinput/geometry.yaml
     testinput/hofx3d.yaml
     testinput/hofx4d.yaml
     )

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testinput)
LINK_FILES("${lola_testinput}" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ecbuild_add_test(TARGET lola_test_geometry
                 ARGS testinput/geometry.yaml
                 COMMAND lola_test_geometry.x
                 )

add_subdirectory(executables)
