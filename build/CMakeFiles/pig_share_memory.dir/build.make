# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/pig/wxc/pig_home/practice_cplusplus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pig/wxc/pig_home/practice_cplusplus/build

# Include any dependencies generated for this target.
include CMakeFiles/pig_share_memory.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pig_share_memory.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pig_share_memory.dir/flags.make

CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.o: CMakeFiles/pig_share_memory.dir/flags.make
CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.o: ../linux/share_memory/pig_share_memory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pig/wxc/pig_home/practice_cplusplus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.o -c /home/pig/wxc/pig_home/practice_cplusplus/linux/share_memory/pig_share_memory.cpp

CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pig/wxc/pig_home/practice_cplusplus/linux/share_memory/pig_share_memory.cpp > CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.i

CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pig/wxc/pig_home/practice_cplusplus/linux/share_memory/pig_share_memory.cpp -o CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.s

# Object files for target pig_share_memory
pig_share_memory_OBJECTS = \
"CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.o"

# External object files for target pig_share_memory
pig_share_memory_EXTERNAL_OBJECTS =

pig_share_memory: CMakeFiles/pig_share_memory.dir/linux/share_memory/pig_share_memory.cpp.o
pig_share_memory: CMakeFiles/pig_share_memory.dir/build.make
pig_share_memory: CMakeFiles/pig_share_memory.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pig/wxc/pig_home/practice_cplusplus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable pig_share_memory"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pig_share_memory.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pig_share_memory.dir/build: pig_share_memory

.PHONY : CMakeFiles/pig_share_memory.dir/build

CMakeFiles/pig_share_memory.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pig_share_memory.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pig_share_memory.dir/clean

CMakeFiles/pig_share_memory.dir/depend:
	cd /home/pig/wxc/pig_home/practice_cplusplus/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pig/wxc/pig_home/practice_cplusplus /home/pig/wxc/pig_home/practice_cplusplus /home/pig/wxc/pig_home/practice_cplusplus/build /home/pig/wxc/pig_home/practice_cplusplus/build /home/pig/wxc/pig_home/practice_cplusplus/build/CMakeFiles/pig_share_memory.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pig_share_memory.dir/depend

