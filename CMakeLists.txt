cmake_minimum_required (VERSION 2.8.7)

############################################################
project (ASDF)

include(GNUInstallDirs)

find_package(HDF5 1.8.0 REQUIRED)
include_directories(${HDF5_INCLUDE_DIR})
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
option (TEST "Run unit tests" OFF)

#set(Boost_NO_SYSTEM_PATHS TRUE)
#if (Boost_NO_SYSTEM_PATHS)
#  set(BOOST_ROOT /tigress/jas11/packages/boost)
#  set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
#  set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
#endif (Boost_NO_SYSTEM_PATHS)

find_package(Boost 1.43 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

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
             ${SRC_DIR}/parse_sf_parfile.cpp
             ${SRC_DIR}/parse_provfile.cpp
             ${SRC_DIR}/gen_provenance_entity.cpp
             ${SRC_DIR}/gen_sf_parfile_provenance.cpp
             ${SRC_DIR}/gen_sf_provenance.cpp
             #${SRC_DIR}/gen_sf_provfile.cpp
             #${SRC_DIR}/gen_provenance_id.cpp
             ${SRC_DIR}/gen_software_provenance.cpp
             ${SRC_DIR}/gen_simulation_provenance.cpp
             ${SRC_DIR}/gen_trace_provenance.cpp
             ${SRC_DIR}/gen_association_provenance.cpp
             ${SRC_DIR}/gen_usage_provenance.cpp
             ${SRC_DIR}/gen_wasGeneratedby_provenance.cpp
             ${SRC_DIR}/ASDF_generate_provenance.c
             ${SRC_DIR}/ASDF_f.c)

target_link_libraries(asdf
                      ${Boost_LIBRARIES} ${HDF5_LIBRARIES} ${MPI_C_LIBRARIES})

install(TARGETS asdf
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
file(GLOB inc_files inc/*.h)
install(FILES ${inc_files}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if (${CMAKE_VERSION} VERSION_GREATER 3.1)
  set_property(TARGET asdf PROPERTY CXX_STANDARD 11)
else()
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if (COMPILER_SUPPORTS_CXX11)
    SET(CMAKE_CXX_FLAGS "-std=c++11")
  elseif (COMPILER_SUPPORTS_CXX0X)
    SET(CMAKE_CXX_FLAGS "-std=c++0x")
  else()
    message(FATAL_ERROR
            "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. "
            "Please use a different C++ compiler.")
  endif()
endif()

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

  set (doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set (doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target (doc
      command ${DOXYGEN_EXECUTABLE} ${doxyfile}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)

  install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html
          DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()

install(FILES README.md
        DESTINATION ${CMAKE_INSTALL_DOCDIR})

############################################################
if (TEST)
  include(CTest)  # enable_testing() is not enough to 'make test'
  add_subdirectory(test)
endif()
add_subdirectory(examples)
