# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/luchenglin/opencl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luchenglin/opencl/build

# Include any dependencies generated for this target.
include Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/depend.make

# Include the progress variables for this target.
include Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/progress.make

# Include the compile flags for this target's objects.
include Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/flags.make

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o: Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/flags.make
Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o: ../Chapter_7/SimpleBufferSubBuffer/simple.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/luchenglin/opencl/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o"
	cd /home/luchenglin/opencl/build/Chapter_7/SimpleBufferSubBuffer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SimpleBufferSubBuffer.dir/simple.o -c /home/luchenglin/opencl/Chapter_7/SimpleBufferSubBuffer/simple.cpp

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleBufferSubBuffer.dir/simple.i"
	cd /home/luchenglin/opencl/build/Chapter_7/SimpleBufferSubBuffer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/luchenglin/opencl/Chapter_7/SimpleBufferSubBuffer/simple.cpp > CMakeFiles/SimpleBufferSubBuffer.dir/simple.i

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleBufferSubBuffer.dir/simple.s"
	cd /home/luchenglin/opencl/build/Chapter_7/SimpleBufferSubBuffer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/luchenglin/opencl/Chapter_7/SimpleBufferSubBuffer/simple.cpp -o CMakeFiles/SimpleBufferSubBuffer.dir/simple.s

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.requires:
.PHONY : Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.requires

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.provides: Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.requires
	$(MAKE) -f Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/build.make Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.provides.build
.PHONY : Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.provides

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.provides.build: Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o

# Object files for target SimpleBufferSubBuffer
SimpleBufferSubBuffer_OBJECTS = \
"CMakeFiles/SimpleBufferSubBuffer.dir/simple.o"

# External object files for target SimpleBufferSubBuffer
SimpleBufferSubBuffer_EXTERNAL_OBJECTS =

Chapter_7/SimpleBufferSubBuffer/SimpleBufferSubBuffer: Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o
Chapter_7/SimpleBufferSubBuffer/SimpleBufferSubBuffer: Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/build.make
Chapter_7/SimpleBufferSubBuffer/SimpleBufferSubBuffer: Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable SimpleBufferSubBuffer"
	cd /home/luchenglin/opencl/build/Chapter_7/SimpleBufferSubBuffer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SimpleBufferSubBuffer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/build: Chapter_7/SimpleBufferSubBuffer/SimpleBufferSubBuffer
.PHONY : Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/build

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/requires: Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/simple.o.requires
.PHONY : Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/requires

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/clean:
	cd /home/luchenglin/opencl/build/Chapter_7/SimpleBufferSubBuffer && $(CMAKE_COMMAND) -P CMakeFiles/SimpleBufferSubBuffer.dir/cmake_clean.cmake
.PHONY : Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/clean

Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/depend:
	cd /home/luchenglin/opencl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luchenglin/opencl /home/luchenglin/opencl/Chapter_7/SimpleBufferSubBuffer /home/luchenglin/opencl/build /home/luchenglin/opencl/build/Chapter_7/SimpleBufferSubBuffer /home/luchenglin/opencl/build/Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Chapter_7/SimpleBufferSubBuffer/CMakeFiles/SimpleBufferSubBuffer.dir/depend

