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

# Include any dependencies generated for this target.
include src/CMakeFiles/TEST.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/TEST.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/TEST.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/TEST.dir/flags.make

src/CMakeFiles/TEST.dir/main.cpp.obj: src/CMakeFiles/TEST.dir/flags.make
src/CMakeFiles/TEST.dir/main.cpp.obj: src/CMakeFiles/TEST.dir/includes_CXX.rsp
src/CMakeFiles/TEST.dir/main.cpp.obj: D:/school/Visualization/src/main.cpp
src/CMakeFiles/TEST.dir/main.cpp.obj: src/CMakeFiles/TEST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/TEST.dir/main.cpp.obj"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/TEST.dir/main.cpp.obj -MF CMakeFiles\TEST.dir\main.cpp.obj.d -o CMakeFiles\TEST.dir\main.cpp.obj -c D:\school\Visualization\src\main.cpp

src/CMakeFiles/TEST.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEST.dir/main.cpp.i"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\school\Visualization\src\main.cpp > CMakeFiles\TEST.dir\main.cpp.i

src/CMakeFiles/TEST.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/main.cpp.s"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\school\Visualization\src\main.cpp -o CMakeFiles\TEST.dir\main.cpp.s

src/CMakeFiles/TEST.dir/Shader.cpp.obj: src/CMakeFiles/TEST.dir/flags.make
src/CMakeFiles/TEST.dir/Shader.cpp.obj: src/CMakeFiles/TEST.dir/includes_CXX.rsp
src/CMakeFiles/TEST.dir/Shader.cpp.obj: D:/school/Visualization/src/Shader.cpp
src/CMakeFiles/TEST.dir/Shader.cpp.obj: src/CMakeFiles/TEST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/TEST.dir/Shader.cpp.obj"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/TEST.dir/Shader.cpp.obj -MF CMakeFiles\TEST.dir\Shader.cpp.obj.d -o CMakeFiles\TEST.dir\Shader.cpp.obj -c D:\school\Visualization\src\Shader.cpp

src/CMakeFiles/TEST.dir/Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEST.dir/Shader.cpp.i"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\school\Visualization\src\Shader.cpp > CMakeFiles\TEST.dir\Shader.cpp.i

src/CMakeFiles/TEST.dir/Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/Shader.cpp.s"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\school\Visualization\src\Shader.cpp -o CMakeFiles\TEST.dir\Shader.cpp.s

src/CMakeFiles/TEST.dir/Volume.cpp.obj: src/CMakeFiles/TEST.dir/flags.make
src/CMakeFiles/TEST.dir/Volume.cpp.obj: src/CMakeFiles/TEST.dir/includes_CXX.rsp
src/CMakeFiles/TEST.dir/Volume.cpp.obj: D:/school/Visualization/src/Volume.cpp
src/CMakeFiles/TEST.dir/Volume.cpp.obj: src/CMakeFiles/TEST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/TEST.dir/Volume.cpp.obj"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/TEST.dir/Volume.cpp.obj -MF CMakeFiles\TEST.dir\Volume.cpp.obj.d -o CMakeFiles\TEST.dir\Volume.cpp.obj -c D:\school\Visualization\src\Volume.cpp

src/CMakeFiles/TEST.dir/Volume.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEST.dir/Volume.cpp.i"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\school\Visualization\src\Volume.cpp > CMakeFiles\TEST.dir\Volume.cpp.i

src/CMakeFiles/TEST.dir/Volume.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/Volume.cpp.s"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\school\Visualization\src\Volume.cpp -o CMakeFiles\TEST.dir\Volume.cpp.s

src/CMakeFiles/TEST.dir/Const.cpp.obj: src/CMakeFiles/TEST.dir/flags.make
src/CMakeFiles/TEST.dir/Const.cpp.obj: src/CMakeFiles/TEST.dir/includes_CXX.rsp
src/CMakeFiles/TEST.dir/Const.cpp.obj: D:/school/Visualization/src/Const.cpp
src/CMakeFiles/TEST.dir/Const.cpp.obj: src/CMakeFiles/TEST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/TEST.dir/Const.cpp.obj"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/TEST.dir/Const.cpp.obj -MF CMakeFiles\TEST.dir\Const.cpp.obj.d -o CMakeFiles\TEST.dir\Const.cpp.obj -c D:\school\Visualization\src\Const.cpp

src/CMakeFiles/TEST.dir/Const.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEST.dir/Const.cpp.i"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\school\Visualization\src\Const.cpp > CMakeFiles\TEST.dir\Const.cpp.i

src/CMakeFiles/TEST.dir/Const.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/Const.cpp.s"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\school\Visualization\src\Const.cpp -o CMakeFiles\TEST.dir\Const.cpp.s

src/CMakeFiles/TEST.dir/Camera.cpp.obj: src/CMakeFiles/TEST.dir/flags.make
src/CMakeFiles/TEST.dir/Camera.cpp.obj: src/CMakeFiles/TEST.dir/includes_CXX.rsp
src/CMakeFiles/TEST.dir/Camera.cpp.obj: D:/school/Visualization/src/Camera.cpp
src/CMakeFiles/TEST.dir/Camera.cpp.obj: src/CMakeFiles/TEST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/TEST.dir/Camera.cpp.obj"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/TEST.dir/Camera.cpp.obj -MF CMakeFiles\TEST.dir\Camera.cpp.obj.d -o CMakeFiles\TEST.dir\Camera.cpp.obj -c D:\school\Visualization\src\Camera.cpp

src/CMakeFiles/TEST.dir/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEST.dir/Camera.cpp.i"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\school\Visualization\src\Camera.cpp > CMakeFiles\TEST.dir\Camera.cpp.i

src/CMakeFiles/TEST.dir/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/Camera.cpp.s"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\school\Visualization\src\Camera.cpp -o CMakeFiles\TEST.dir\Camera.cpp.s

src/CMakeFiles/TEST.dir/ModelManager.cpp.obj: src/CMakeFiles/TEST.dir/flags.make
src/CMakeFiles/TEST.dir/ModelManager.cpp.obj: src/CMakeFiles/TEST.dir/includes_CXX.rsp
src/CMakeFiles/TEST.dir/ModelManager.cpp.obj: D:/school/Visualization/src/ModelManager.cpp
src/CMakeFiles/TEST.dir/ModelManager.cpp.obj: src/CMakeFiles/TEST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/TEST.dir/ModelManager.cpp.obj"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/TEST.dir/ModelManager.cpp.obj -MF CMakeFiles\TEST.dir\ModelManager.cpp.obj.d -o CMakeFiles\TEST.dir\ModelManager.cpp.obj -c D:\school\Visualization\src\ModelManager.cpp

src/CMakeFiles/TEST.dir/ModelManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEST.dir/ModelManager.cpp.i"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\school\Visualization\src\ModelManager.cpp > CMakeFiles\TEST.dir\ModelManager.cpp.i

src/CMakeFiles/TEST.dir/ModelManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/ModelManager.cpp.s"
	cd /d D:\school\Visualization\build\src && C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\school\Visualization\src\ModelManager.cpp -o CMakeFiles\TEST.dir\ModelManager.cpp.s

# Object files for target TEST
TEST_OBJECTS = \
"CMakeFiles/TEST.dir/main.cpp.obj" \
"CMakeFiles/TEST.dir/Shader.cpp.obj" \
"CMakeFiles/TEST.dir/Volume.cpp.obj" \
"CMakeFiles/TEST.dir/Const.cpp.obj" \
"CMakeFiles/TEST.dir/Camera.cpp.obj" \
"CMakeFiles/TEST.dir/ModelManager.cpp.obj"

# External object files for target TEST
TEST_EXTERNAL_OBJECTS =

src/TEST.exe: src/CMakeFiles/TEST.dir/main.cpp.obj
src/TEST.exe: src/CMakeFiles/TEST.dir/Shader.cpp.obj
src/TEST.exe: src/CMakeFiles/TEST.dir/Volume.cpp.obj
src/TEST.exe: src/CMakeFiles/TEST.dir/Const.cpp.obj
src/TEST.exe: src/CMakeFiles/TEST.dir/Camera.cpp.obj
src/TEST.exe: src/CMakeFiles/TEST.dir/ModelManager.cpp.obj
src/TEST.exe: src/CMakeFiles/TEST.dir/build.make
src/TEST.exe: extern/glfw/src/libglfw3.a
src/TEST.exe: extern/glm/glm/libglm.a
src/TEST.exe: extern/glad/libglad.a
src/TEST.exe: extern/imgui/libimgui.a
src/TEST.exe: extern/glfw/src/libglfw3.a
src/TEST.exe: src/CMakeFiles/TEST.dir/linkLibs.rsp
src/TEST.exe: src/CMakeFiles/TEST.dir/objects1.rsp
src/TEST.exe: src/CMakeFiles/TEST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\school\Visualization\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable TEST.exe"
	cd /d D:\school\Visualization\build\src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\TEST.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/TEST.dir/build: src/TEST.exe
.PHONY : src/CMakeFiles/TEST.dir/build

src/CMakeFiles/TEST.dir/clean:
	cd /d D:\school\Visualization\build\src && $(CMAKE_COMMAND) -P CMakeFiles\TEST.dir\cmake_clean.cmake
.PHONY : src/CMakeFiles/TEST.dir/clean

src/CMakeFiles/TEST.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\school\Visualization D:\school\Visualization\src D:\school\Visualization\build D:\school\Visualization\build\src D:\school\Visualization\build\src\CMakeFiles\TEST.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/TEST.dir/depend

