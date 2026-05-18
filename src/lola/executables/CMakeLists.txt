
ecbuild_add_executable(TARGET lola_hofx3d.x
                       SOURCES HofX3D.cc
                       LIBS mist_api mist_base lola
                       )

ecbuild_add_executable(TARGET lola_hofx4d.x
                       SOURCES HofX4D.cc
                       LIBS mist_api mist_base lola
                       )
