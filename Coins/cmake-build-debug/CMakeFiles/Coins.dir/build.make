# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/maxlord/Google Drive/Computer Vision/Coins"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/maxlord/Google Drive/Computer Vision/Coins/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Coins.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Coins.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Coins.dir/flags.make

CMakeFiles/Coins.dir/main.cpp.o: CMakeFiles/Coins.dir/flags.make
CMakeFiles/Coins.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/maxlord/Google Drive/Computer Vision/Coins/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Coins.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Coins.dir/main.cpp.o -c "/Users/maxlord/Google Drive/Computer Vision/Coins/main.cpp"

CMakeFiles/Coins.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Coins.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/maxlord/Google Drive/Computer Vision/Coins/main.cpp" > CMakeFiles/Coins.dir/main.cpp.i

CMakeFiles/Coins.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Coins.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/maxlord/Google Drive/Computer Vision/Coins/main.cpp" -o CMakeFiles/Coins.dir/main.cpp.s

CMakeFiles/Coins.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Coins.dir/main.cpp.o.requires

CMakeFiles/Coins.dir/main.cpp.o.provides: CMakeFiles/Coins.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Coins.dir/build.make CMakeFiles/Coins.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Coins.dir/main.cpp.o.provides

CMakeFiles/Coins.dir/main.cpp.o.provides.build: CMakeFiles/Coins.dir/main.cpp.o


# Object files for target Coins
Coins_OBJECTS = \
"CMakeFiles/Coins.dir/main.cpp.o"

# External object files for target Coins
Coins_EXTERNAL_OBJECTS =

Coins: CMakeFiles/Coins.dir/main.cpp.o
Coins: CMakeFiles/Coins.dir/build.make
Coins: CMakeFiles/Coins.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/maxlord/Google Drive/Computer Vision/Coins/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Coins"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Coins.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Coins.dir/build: Coins

.PHONY : CMakeFiles/Coins.dir/build

CMakeFiles/Coins.dir/requires: CMakeFiles/Coins.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Coins.dir/requires

CMakeFiles/Coins.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Coins.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Coins.dir/clean

CMakeFiles/Coins.dir/depend:
	cd "/Users/maxlord/Google Drive/Computer Vision/Coins/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/maxlord/Google Drive/Computer Vision/Coins" "/Users/maxlord/Google Drive/Computer Vision/Coins" "/Users/maxlord/Google Drive/Computer Vision/Coins/cmake-build-debug" "/Users/maxlord/Google Drive/Computer Vision/Coins/cmake-build-debug" "/Users/maxlord/Google Drive/Computer Vision/Coins/cmake-build-debug/CMakeFiles/Coins.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Coins.dir/depend

