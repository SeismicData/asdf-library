cmake_minimum_required (VERSION 2.8)

############################################################
project (ASDF)

find_package(HDF5)
include_directories(${HDF5_INCLUDE_DIR})
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
option (TEST "Run unit tests" OFF)

############################################################
### Setup directory structure                            ###
############################################################
include_directories (./inc)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)  # .so
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)  # .a

set (SRC_DIR ${CMAKE_SOURCE_DIR}/src)

############################################################
### ASDF library                                         ###
############################################################
add_library (asdf 
             ${SRC_DIR}/ASDF_init.c
             ${SRC_DIR}/ASDF_read.c
             ${SRC_DIR}/ASDF_write.c
             ${SRC_DIR}/ASDF_f.c)

target_link_libraries(asdf ${HDF5_LIBRARIES} ${MPI_C_LIBRARIES})

############################################################
### Documentation Generation                             ###
############################################################
# See: https://www.tty1.net/blog/2014/cmake-doxygen_en.html
find_package(Doxygen)
option (BUILD_DOCUMENTATION
        "Create and install the HTML based API documentation (requires Doxygen)"
        ${DOXYGEN_FOUND})

if (BUILD_DOCUMENTATION)
  if (NOT DOXYGEN_FOUND)
    message (FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif ()

  set (doxyfile ${CMAKE_SOURCE_DIR}/Doxyfile)

  add_custom_target (doc
      command ${DOXYGEN_EXECUTABLE} ${doxyfile}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)

  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html 
           DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/doc)
endif()


############################################################
if (TEST)
  include(CTest)  # enable_testing() is not enough to 'make test'
  add_subdirectory(test)
endif()
add_subdirectory(examples)
