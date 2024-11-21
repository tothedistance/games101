cmake_minimum_required(VERSION 3.20)
project(cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(utils)

include_directories(include)

find_package(glog REQUIRED)
find_package(GTest REQUIRED)
find_package(gflags REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)


link_libraries(glog::glog)
link_libraries(GTest::gtest)
link_libraries(gflags)
link_libraries(Eigen3::Eigen)
link_libraries(opencv_core)
link_libraries(opencv_highgui)


subdirlist(src_dirs ${CMAKE_SOURCE_DIR})
list(
  REMOVE_ITEM
  src_dirs
  .git
  .vs
  .vscode
  out
  build
  cmake
  include
  dependencies)
foreach(src_dir ${src_dirs})
  add_subdirectory(${src_dir})
endforeach(src_dir ${src_dirs})

enable_testing()