# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lcl/Desktop/projects/C++_projects/cmake-atan

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lcl/Desktop/projects/C++_projects/cmake-atan

# Include any dependencies generated for this target.
include CMakeFiles/atan_demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/atan_demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/atan_demo.dir/flags.make

CMakeFiles/atan_demo.dir/atan_demo.cpp.o: CMakeFiles/atan_demo.dir/flags.make
CMakeFiles/atan_demo.dir/atan_demo.cpp.o: atan_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lcl/Desktop/projects/C++_projects/cmake-atan/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/atan_demo.dir/atan_demo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/atan_demo.dir/atan_demo.cpp.o -c /home/lcl/Desktop/projects/C++_projects/cmake-atan/atan_demo.cpp

CMakeFiles/atan_demo.dir/atan_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/atan_demo.dir/atan_demo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lcl/Desktop/projects/C++_projects/cmake-atan/atan_demo.cpp > CMakeFiles/atan_demo.dir/atan_demo.cpp.i

CMakeFiles/atan_demo.dir/atan_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/atan_demo.dir/atan_demo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lcl/Desktop/projects/C++_projects/cmake-atan/atan_demo.cpp -o CMakeFiles/atan_demo.dir/atan_demo.cpp.s

CMakeFiles/atan_demo.dir/atan_demo.cpp.o.requires:

.PHONY : CMakeFiles/atan_demo.dir/atan_demo.cpp.o.requires

CMakeFiles/atan_demo.dir/atan_demo.cpp.o.provides: CMakeFiles/atan_demo.dir/atan_demo.cpp.o.requires
	$(MAKE) -f CMakeFiles/atan_demo.dir/build.make CMakeFiles/atan_demo.dir/atan_demo.cpp.o.provides.build
.PHONY : CMakeFiles/atan_demo.dir/atan_demo.cpp.o.provides

CMakeFiles/atan_demo.dir/atan_demo.cpp.o.provides.build: CMakeFiles/atan_demo.dir/atan_demo.cpp.o


# Object files for target atan_demo
atan_demo_OBJECTS = \
"CMakeFiles/atan_demo.dir/atan_demo.cpp.o"

# External object files for target atan_demo
atan_demo_EXTERNAL_OBJECTS =

atan_demo: CMakeFiles/atan_demo.dir/atan_demo.cpp.o
atan_demo: CMakeFiles/atan_demo.dir/build.make
atan_demo: CMakeFiles/atan_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lcl/Desktop/projects/C++_projects/cmake-atan/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable atan_demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/atan_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/atan_demo.dir/build: atan_demo

.PHONY : CMakeFiles/atan_demo.dir/build

CMakeFiles/atan_demo.dir/requires: CMakeFiles/atan_demo.dir/atan_demo.cpp.o.requires

.PHONY : CMakeFiles/atan_demo.dir/requires

CMakeFiles/atan_demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/atan_demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/atan_demo.dir/clean

CMakeFiles/atan_demo.dir/depend:
	cd /home/lcl/Desktop/projects/C++_projects/cmake-atan && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lcl/Desktop/projects/C++_projects/cmake-atan /home/lcl/Desktop/projects/C++_projects/cmake-atan /home/lcl/Desktop/projects/C++_projects/cmake-atan /home/lcl/Desktop/projects/C++_projects/cmake-atan /home/lcl/Desktop/projects/C++_projects/cmake-atan/CMakeFiles/atan_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/atan_demo.dir/depend

