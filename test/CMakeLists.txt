
############################################################
### Unit testing                                         ###
############################################################
add_subdirectory(gtest-1.7.0)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests test_read_write.cpp)

if (${CMAKE_VERSION} VERSION_GREATER 3.1)
  set_property(TARGET runUnitTests PROPERTY CXX_STANDARD 11)
else()
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if (COMPILER_SUPPORTS_CXX11)
    set_source_files_properties(test_read_write.cpp
                                PROPERTIES
                                COMPILE_FLAGS "-std=c++11")
  elseif (COMPILER_SUPPORTS_CXX0X)
    set_source_files_properties(test_read_write.cpp
                                PROPERTIES
                                COMPILE_FLAGS "-std=c++0x")
  else()
    message(FATAL_ERROR
            "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. "
            "Please use a different C++ compiler.")
  endif()
endif()

target_link_libraries(runUnitTests
                      gtest
                      gtest_main
                      asdf
                      ${MPI_LIBRARIES})

add_test(runUnitTests runUnitTests)
