# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\school\Visualization

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\school\Visualization\build

# Utility rule file for update_mappings.

# Include any custom commands dependencies for this target.
include extern/glfw/src/CMakeFiles/update_mappings.dir/compiler_depend.make

# Include the progress variables for this target.
include extern/glfw/src/CMakeFiles/update_mappings.dir/progress.make

extern/glfw/src/CMakeFiles/update_mappings:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Updating gamepad mappings from upstream repository"
	cd /d D:\school\Visualization\extern\glfw\src && "C:\Program Files\CMake\bin\cmake.exe" -P D:/school/Visualization/extern/glfw/CMake/GenerateMappings.cmake mappings.h.in mappings.h

update_mappings: extern/glfw/src/CMakeFiles/update_mappings
update_mappings: extern/glfw/src/CMakeFiles/update_mappings.dir/build.make
.PHONY : update_mappings

# Rule to build all files generated by this target.
extern/glfw/src/CMakeFiles/update_mappings.dir/build: update_mappings
.PHONY : extern/glfw/src/CMakeFiles/update_mappings.dir/build

extern/glfw/src/CMakeFiles/update_mappings.dir/clean:
	cd /d D:\school\Visualization\build\extern\glfw\src && $(CMAKE_COMMAND) -P CMakeFiles\update_mappings.dir\cmake_clean.cmake
.PHONY : extern/glfw/src/CMakeFiles/update_mappings.dir/clean

extern/glfw/src/CMakeFiles/update_mappings.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\school\Visualization D:\school\Visualization\extern\glfw\src D:\school\Visualization\build D:\school\Visualization\build\extern\glfw\src D:\school\Visualization\build\extern\glfw\src\CMakeFiles\update_mappings.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : extern/glfw/src/CMakeFiles/update_mappings.dir/depend

