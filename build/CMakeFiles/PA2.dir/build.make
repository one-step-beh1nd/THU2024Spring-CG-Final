# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/step/GRAPH/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/step/GRAPH/code/build

# Include any dependencies generated for this target.
include CMakeFiles/PA2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PA2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PA2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PA2.dir/flags.make

CMakeFiles/PA2.dir/src/image.cpp.o: CMakeFiles/PA2.dir/flags.make
CMakeFiles/PA2.dir/src/image.cpp.o: ../src/image.cpp
CMakeFiles/PA2.dir/src/image.cpp.o: CMakeFiles/PA2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/step/GRAPH/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PA2.dir/src/image.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PA2.dir/src/image.cpp.o -MF CMakeFiles/PA2.dir/src/image.cpp.o.d -o CMakeFiles/PA2.dir/src/image.cpp.o -c /home/step/GRAPH/code/src/image.cpp

CMakeFiles/PA2.dir/src/image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA2.dir/src/image.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/step/GRAPH/code/src/image.cpp > CMakeFiles/PA2.dir/src/image.cpp.i

CMakeFiles/PA2.dir/src/image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA2.dir/src/image.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/step/GRAPH/code/src/image.cpp -o CMakeFiles/PA2.dir/src/image.cpp.s

CMakeFiles/PA2.dir/src/main.cpp.o: CMakeFiles/PA2.dir/flags.make
CMakeFiles/PA2.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/PA2.dir/src/main.cpp.o: CMakeFiles/PA2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/step/GRAPH/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/PA2.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PA2.dir/src/main.cpp.o -MF CMakeFiles/PA2.dir/src/main.cpp.o.d -o CMakeFiles/PA2.dir/src/main.cpp.o -c /home/step/GRAPH/code/src/main.cpp

CMakeFiles/PA2.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA2.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/step/GRAPH/code/src/main.cpp > CMakeFiles/PA2.dir/src/main.cpp.i

CMakeFiles/PA2.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA2.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/step/GRAPH/code/src/main.cpp -o CMakeFiles/PA2.dir/src/main.cpp.s

CMakeFiles/PA2.dir/src/mesh.cpp.o: CMakeFiles/PA2.dir/flags.make
CMakeFiles/PA2.dir/src/mesh.cpp.o: ../src/mesh.cpp
CMakeFiles/PA2.dir/src/mesh.cpp.o: CMakeFiles/PA2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/step/GRAPH/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/PA2.dir/src/mesh.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PA2.dir/src/mesh.cpp.o -MF CMakeFiles/PA2.dir/src/mesh.cpp.o.d -o CMakeFiles/PA2.dir/src/mesh.cpp.o -c /home/step/GRAPH/code/src/mesh.cpp

CMakeFiles/PA2.dir/src/mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA2.dir/src/mesh.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/step/GRAPH/code/src/mesh.cpp > CMakeFiles/PA2.dir/src/mesh.cpp.i

CMakeFiles/PA2.dir/src/mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA2.dir/src/mesh.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/step/GRAPH/code/src/mesh.cpp -o CMakeFiles/PA2.dir/src/mesh.cpp.s

CMakeFiles/PA2.dir/src/scene_parser.cpp.o: CMakeFiles/PA2.dir/flags.make
CMakeFiles/PA2.dir/src/scene_parser.cpp.o: ../src/scene_parser.cpp
CMakeFiles/PA2.dir/src/scene_parser.cpp.o: CMakeFiles/PA2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/step/GRAPH/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/PA2.dir/src/scene_parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PA2.dir/src/scene_parser.cpp.o -MF CMakeFiles/PA2.dir/src/scene_parser.cpp.o.d -o CMakeFiles/PA2.dir/src/scene_parser.cpp.o -c /home/step/GRAPH/code/src/scene_parser.cpp

CMakeFiles/PA2.dir/src/scene_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA2.dir/src/scene_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/step/GRAPH/code/src/scene_parser.cpp > CMakeFiles/PA2.dir/src/scene_parser.cpp.i

CMakeFiles/PA2.dir/src/scene_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA2.dir/src/scene_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/step/GRAPH/code/src/scene_parser.cpp -o CMakeFiles/PA2.dir/src/scene_parser.cpp.s

CMakeFiles/PA2.dir/src/camera_controller.cpp.o: CMakeFiles/PA2.dir/flags.make
CMakeFiles/PA2.dir/src/camera_controller.cpp.o: ../src/camera_controller.cpp
CMakeFiles/PA2.dir/src/camera_controller.cpp.o: CMakeFiles/PA2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/step/GRAPH/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/PA2.dir/src/camera_controller.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PA2.dir/src/camera_controller.cpp.o -MF CMakeFiles/PA2.dir/src/camera_controller.cpp.o.d -o CMakeFiles/PA2.dir/src/camera_controller.cpp.o -c /home/step/GRAPH/code/src/camera_controller.cpp

CMakeFiles/PA2.dir/src/camera_controller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA2.dir/src/camera_controller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/step/GRAPH/code/src/camera_controller.cpp > CMakeFiles/PA2.dir/src/camera_controller.cpp.i

CMakeFiles/PA2.dir/src/camera_controller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA2.dir/src/camera_controller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/step/GRAPH/code/src/camera_controller.cpp -o CMakeFiles/PA2.dir/src/camera_controller.cpp.s

CMakeFiles/PA2.dir/src/texture.cpp.o: CMakeFiles/PA2.dir/flags.make
CMakeFiles/PA2.dir/src/texture.cpp.o: ../src/texture.cpp
CMakeFiles/PA2.dir/src/texture.cpp.o: CMakeFiles/PA2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/step/GRAPH/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/PA2.dir/src/texture.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PA2.dir/src/texture.cpp.o -MF CMakeFiles/PA2.dir/src/texture.cpp.o.d -o CMakeFiles/PA2.dir/src/texture.cpp.o -c /home/step/GRAPH/code/src/texture.cpp

CMakeFiles/PA2.dir/src/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA2.dir/src/texture.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/step/GRAPH/code/src/texture.cpp > CMakeFiles/PA2.dir/src/texture.cpp.i

CMakeFiles/PA2.dir/src/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA2.dir/src/texture.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/step/GRAPH/code/src/texture.cpp -o CMakeFiles/PA2.dir/src/texture.cpp.s

# Object files for target PA2
PA2_OBJECTS = \
"CMakeFiles/PA2.dir/src/image.cpp.o" \
"CMakeFiles/PA2.dir/src/main.cpp.o" \
"CMakeFiles/PA2.dir/src/mesh.cpp.o" \
"CMakeFiles/PA2.dir/src/scene_parser.cpp.o" \
"CMakeFiles/PA2.dir/src/camera_controller.cpp.o" \
"CMakeFiles/PA2.dir/src/texture.cpp.o"

# External object files for target PA2
PA2_EXTERNAL_OBJECTS =

PA2: CMakeFiles/PA2.dir/src/image.cpp.o
PA2: CMakeFiles/PA2.dir/src/main.cpp.o
PA2: CMakeFiles/PA2.dir/src/mesh.cpp.o
PA2: CMakeFiles/PA2.dir/src/scene_parser.cpp.o
PA2: CMakeFiles/PA2.dir/src/camera_controller.cpp.o
PA2: CMakeFiles/PA2.dir/src/texture.cpp.o
PA2: CMakeFiles/PA2.dir/build.make
PA2: deps/vecmath/libvecmath.a
PA2: /usr/lib/x86_64-linux-gnu/libGL.so
PA2: /usr/lib/x86_64-linux-gnu/libGLU.so
PA2: /usr/lib/x86_64-linux-gnu/libglut.so
PA2: /usr/lib/x86_64-linux-gnu/libXi.so
PA2: CMakeFiles/PA2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/step/GRAPH/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable PA2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PA2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PA2.dir/build: PA2
.PHONY : CMakeFiles/PA2.dir/build

CMakeFiles/PA2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PA2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PA2.dir/clean

CMakeFiles/PA2.dir/depend:
	cd /home/step/GRAPH/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/step/GRAPH/code /home/step/GRAPH/code /home/step/GRAPH/code/build /home/step/GRAPH/code/build /home/step/GRAPH/code/build/CMakeFiles/PA2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PA2.dir/depend

