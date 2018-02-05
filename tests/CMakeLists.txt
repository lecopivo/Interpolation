add_executable(interpolation1d interpolation1d.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/interpolation1d.sh
  ${CMAKE_CURRENT_BINARY_DIR}/interpolation1d.sh COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/interpolation1d.gnu
  ${CMAKE_CURRENT_BINARY_DIR}/interpolation1d.gnu COPYONLY)

add_executable(interpolation2d interpolation2d.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/interpolation2d.sh
  ${CMAKE_CURRENT_BINARY_DIR}/interpolation2d.sh COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/interpolation2d.gnu
  ${CMAKE_CURRENT_BINARY_DIR}/interpolation2d.gnu COPYONLY)
