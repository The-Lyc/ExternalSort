# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/shy/cpp_ws/ExternalSort/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shy/cpp_ws/ExternalSort/test/build

# Include any dependencies generated for this target.
include CMakeFiles/cas_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cas_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cas_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cas_test.dir/flags.make

CMakeFiles/cas_test.dir/cas_test.o: CMakeFiles/cas_test.dir/flags.make
CMakeFiles/cas_test.dir/cas_test.o: /home/shy/cpp_ws/ExternalSort/test/cas_test.cpp
CMakeFiles/cas_test.dir/cas_test.o: CMakeFiles/cas_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/shy/cpp_ws/ExternalSort/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cas_test.dir/cas_test.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cas_test.dir/cas_test.o -MF CMakeFiles/cas_test.dir/cas_test.o.d -o CMakeFiles/cas_test.dir/cas_test.o -c /home/shy/cpp_ws/ExternalSort/test/cas_test.cpp

CMakeFiles/cas_test.dir/cas_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cas_test.dir/cas_test.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shy/cpp_ws/ExternalSort/test/cas_test.cpp > CMakeFiles/cas_test.dir/cas_test.i

CMakeFiles/cas_test.dir/cas_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cas_test.dir/cas_test.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shy/cpp_ws/ExternalSort/test/cas_test.cpp -o CMakeFiles/cas_test.dir/cas_test.s

# Object files for target cas_test
cas_test_OBJECTS = \
"CMakeFiles/cas_test.dir/cas_test.o"

# External object files for target cas_test
cas_test_EXTERNAL_OBJECTS =

cas_test: CMakeFiles/cas_test.dir/cas_test.o
cas_test: CMakeFiles/cas_test.dir/build.make
cas_test: CMakeFiles/cas_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/shy/cpp_ws/ExternalSort/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cas_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cas_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cas_test.dir/build: cas_test
.PHONY : CMakeFiles/cas_test.dir/build

CMakeFiles/cas_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cas_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cas_test.dir/clean

CMakeFiles/cas_test.dir/depend:
	cd /home/shy/cpp_ws/ExternalSort/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shy/cpp_ws/ExternalSort/test /home/shy/cpp_ws/ExternalSort/test /home/shy/cpp_ws/ExternalSort/test/build /home/shy/cpp_ws/ExternalSort/test/build /home/shy/cpp_ws/ExternalSort/test/build/CMakeFiles/cas_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cas_test.dir/depend

