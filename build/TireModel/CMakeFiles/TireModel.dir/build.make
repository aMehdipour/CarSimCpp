# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/amehdipour/Repos/CarSimCpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amehdipour/Repos/CarSimCpp/build

# Include any dependencies generated for this target.
include TireModel/CMakeFiles/TireModel.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include TireModel/CMakeFiles/TireModel.dir/compiler_depend.make

# Include the progress variables for this target.
include TireModel/CMakeFiles/TireModel.dir/progress.make

# Include the compile flags for this target's objects.
include TireModel/CMakeFiles/TireModel.dir/flags.make

TireModel/CMakeFiles/TireModel.dir/codegen:
.PHONY : TireModel/CMakeFiles/TireModel.dir/codegen

TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.o: TireModel/CMakeFiles/TireModel.dir/flags.make
TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.o: /home/amehdipour/Repos/CarSimCpp/TireModel/MFCoefficients.cpp
TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.o: TireModel/CMakeFiles/TireModel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amehdipour/Repos/CarSimCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.o"
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.o -MF CMakeFiles/TireModel.dir/MFCoefficients.cpp.o.d -o CMakeFiles/TireModel.dir/MFCoefficients.cpp.o -c /home/amehdipour/Repos/CarSimCpp/TireModel/MFCoefficients.cpp

TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TireModel.dir/MFCoefficients.cpp.i"
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amehdipour/Repos/CarSimCpp/TireModel/MFCoefficients.cpp > CMakeFiles/TireModel.dir/MFCoefficients.cpp.i

TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TireModel.dir/MFCoefficients.cpp.s"
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amehdipour/Repos/CarSimCpp/TireModel/MFCoefficients.cpp -o CMakeFiles/TireModel.dir/MFCoefficients.cpp.s

TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.o: TireModel/CMakeFiles/TireModel.dir/flags.make
TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.o: /home/amehdipour/Repos/CarSimCpp/TireModel/MFTireModel.cpp
TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.o: TireModel/CMakeFiles/TireModel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amehdipour/Repos/CarSimCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.o"
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.o -MF CMakeFiles/TireModel.dir/MFTireModel.cpp.o.d -o CMakeFiles/TireModel.dir/MFTireModel.cpp.o -c /home/amehdipour/Repos/CarSimCpp/TireModel/MFTireModel.cpp

TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TireModel.dir/MFTireModel.cpp.i"
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amehdipour/Repos/CarSimCpp/TireModel/MFTireModel.cpp > CMakeFiles/TireModel.dir/MFTireModel.cpp.i

TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TireModel.dir/MFTireModel.cpp.s"
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amehdipour/Repos/CarSimCpp/TireModel/MFTireModel.cpp -o CMakeFiles/TireModel.dir/MFTireModel.cpp.s

# Object files for target TireModel
TireModel_OBJECTS = \
"CMakeFiles/TireModel.dir/MFCoefficients.cpp.o" \
"CMakeFiles/TireModel.dir/MFTireModel.cpp.o"

# External object files for target TireModel
TireModel_EXTERNAL_OBJECTS =

TireModel/libTireModel.a: TireModel/CMakeFiles/TireModel.dir/MFCoefficients.cpp.o
TireModel/libTireModel.a: TireModel/CMakeFiles/TireModel.dir/MFTireModel.cpp.o
TireModel/libTireModel.a: TireModel/CMakeFiles/TireModel.dir/build.make
TireModel/libTireModel.a: TireModel/CMakeFiles/TireModel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/amehdipour/Repos/CarSimCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libTireModel.a"
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && $(CMAKE_COMMAND) -P CMakeFiles/TireModel.dir/cmake_clean_target.cmake
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TireModel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
TireModel/CMakeFiles/TireModel.dir/build: TireModel/libTireModel.a
.PHONY : TireModel/CMakeFiles/TireModel.dir/build

TireModel/CMakeFiles/TireModel.dir/clean:
	cd /home/amehdipour/Repos/CarSimCpp/build/TireModel && $(CMAKE_COMMAND) -P CMakeFiles/TireModel.dir/cmake_clean.cmake
.PHONY : TireModel/CMakeFiles/TireModel.dir/clean

TireModel/CMakeFiles/TireModel.dir/depend:
	cd /home/amehdipour/Repos/CarSimCpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amehdipour/Repos/CarSimCpp /home/amehdipour/Repos/CarSimCpp/TireModel /home/amehdipour/Repos/CarSimCpp/build /home/amehdipour/Repos/CarSimCpp/build/TireModel /home/amehdipour/Repos/CarSimCpp/build/TireModel/CMakeFiles/TireModel.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : TireModel/CMakeFiles/TireModel.dir/depend

