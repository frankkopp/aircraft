cmake_minimum_required(VERSION 3.19)
project(flybywire-a32nx)

set(CMAKE_CXX_STANDARD 20)
set(MSFS_SDK "C:\\MSFS SDK")

add_subdirectory(src/fadec)
add_subdirectory(src/fbw_a320)
add_subdirectory(src/fbw_a380)
add_subdirectory(src/extra-backend)
