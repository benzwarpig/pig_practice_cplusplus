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
include CMakeFiles/pig_message_queue_B.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pig_message_queue_B.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pig_message_queue_B.dir/flags.make

CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.o: CMakeFiles/pig_message_queue_B.dir/flags.make
CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.o: ../linux/message_queue/pig_message_queue_B.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pig/wxc/pig_home/practice_cplusplus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.o -c /home/pig/wxc/pig_home/practice_cplusplus/linux/message_queue/pig_message_queue_B.cpp

CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pig/wxc/pig_home/practice_cplusplus/linux/message_queue/pig_message_queue_B.cpp > CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.i

CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pig/wxc/pig_home/practice_cplusplus/linux/message_queue/pig_message_queue_B.cpp -o CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.s

# Object files for target pig_message_queue_B
pig_message_queue_B_OBJECTS = \
"CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.o"

# External object files for target pig_message_queue_B
pig_message_queue_B_EXTERNAL_OBJECTS =

pig_message_queue_B: CMakeFiles/pig_message_queue_B.dir/linux/message_queue/pig_message_queue_B.cpp.o
pig_message_queue_B: CMakeFiles/pig_message_queue_B.dir/build.make
pig_message_queue_B: CMakeFiles/pig_message_queue_B.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pig/wxc/pig_home/practice_cplusplus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable pig_message_queue_B"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pig_message_queue_B.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pig_message_queue_B.dir/build: pig_message_queue_B

.PHONY : CMakeFiles/pig_message_queue_B.dir/build

CMakeFiles/pig_message_queue_B.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pig_message_queue_B.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pig_message_queue_B.dir/clean

CMakeFiles/pig_message_queue_B.dir/depend:
	cd /home/pig/wxc/pig_home/practice_cplusplus/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pig/wxc/pig_home/practice_cplusplus /home/pig/wxc/pig_home/practice_cplusplus /home/pig/wxc/pig_home/practice_cplusplus/build /home/pig/wxc/pig_home/practice_cplusplus/build /home/pig/wxc/pig_home/practice_cplusplus/build/CMakeFiles/pig_message_queue_B.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pig_message_queue_B.dir/depend
