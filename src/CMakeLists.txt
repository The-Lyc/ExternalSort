include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCE "*.cpp")
add_executable(external_sort main.cpp;${SOURCE})