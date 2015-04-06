############################################################
### Unit testing                                         ### 
############################################################
add_subdirectory(gtest-1.7.0)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests test_read_write.cpp)

set_property(TARGET runUnitTests PROPERTY CXX_STANDARD 11)

target_link_libraries(runUnitTests 
                      gtest
                      gtest_main
                      ${CMAKE_BINARY_DIR}/lib/libasdf.a
                      ${HDF5_LIBRARIES}
                      ${MPI_LIBRARIES})

add_test(runUnitTests runUnitTests)
