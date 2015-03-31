############################################################
### Exemple showing how to write ASDF files in C.        ###
############################################################

set (${HDF5_ENABLE_PARALLEL} 1)

# message (STATUS ${HDF5_LIBRARIES})
# message (STATUS ${HDF5_C_LIBRARIES})

add_executable(write_ASDF write_ASDF.c)
target_link_libraries(write_ASDF
                      asdf
                      ${HDF5_LIBRARIES}
                      ${MPI_LIBRARIES})

enable_language (Fortran)
add_definitions(${MP0I_Fortran_COMPILE_FLAGS})
include_directories(${MPI_Fortran_INCLUDE_PATH})
link_directories(${MPI_Fortran_LIBRARIES})
                    
#message(Status "MPI libs: " ${MPI_LIBRARIES})
#message(Status "MPI_Fortran libs: " ${MPI_Fortran_LIBRARIES})
#message(Status "HDF5 fort: " ${HDF5_C_LIBRARIES})
#message(Status "HDF5 fort: " "${HDF5_LIBRARIES}")

add_executable(test_f test.f90)
target_link_libraries(test_f
                      asdf
                      ${HDF5_C_LIBRARIES}
                      ${MPI_Fortran_LINK_FLAGS}
                      ${MPI_LIBRARIES}
                      ${MPI_Fortran_LIBRARIES})


##############
# Unit Tests
##############
############################################################
### Unit testing                                         ### 
############################################################

# Gtest's test

add_subdirectory(gtest-1.7.0)
enable_testing()

# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests test_read_write.cpp)

set_property(TARGET runUnitTests PROPERTY CXX_STANDARD 11)

# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest gtest_main)

# Extra linking for the project.
target_link_libraries(runUnitTests 
                      ${CMAKE_BINARY_DIR}/lib/libasdf.a
                      ${HDF5_LIBRARIES}
                      ${MPI_LIBRARIES})

# target_link_libraries(runUnitTests project1_lib)

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
#add_test(NAME that-test-I-made COMMAND runUnitTests)

# You can also omit NAME and COMMAND. The second argument could be some other
# test executable.
#add_test(that-other-test-I-made runUnitTests)
