include_directories(../include)
set(SOURCE_IO io_test.cpp) 
set(SOURCE_GENERATE generate.cpp ../src/sort_option.cpp)
add_executable(io_test ${SOURCE_IO})
add_executable(generate ${SOURCE_GENERATE})
