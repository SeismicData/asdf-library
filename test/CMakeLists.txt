
############################################################
### Unit testing                                         ### 
############################################################
add_subdirectory(gtest-1.7.0)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests test_read_write.cpp)

if (${CMAKE_VERSION} VERSION_GREATER 3.0) 
  message(STATUS "yaaa")
  set_property(TARGET runUnitTests PROPERTY CXX_STANDARD 11)
else()
  message(STATUS "yooo")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

target_link_libraries(runUnitTests 
                      gtest
                      gtest_main
                      ${CMAKE_BINARY_DIR}/lib/libasdf.a
                      ${HDF5_LIBRARIES}
                      ${MPI_LIBRARIES})

add_test(runUnitTests runUnitTests)
