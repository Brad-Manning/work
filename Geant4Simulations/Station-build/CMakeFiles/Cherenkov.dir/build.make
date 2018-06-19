# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /remote/tesla/bmanning/auger/software/ApeInstall/cmake/3.11.1/bin/cmake

# The command to remove a file.
RM = /remote/tesla/bmanning/auger/software/ApeInstall/cmake/3.11.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /remote/tesla/bmanning/work/Geant4Simulations/Station

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /remote/tesla/bmanning/work/Geant4Simulations/Station-build

# Include any dependencies generated for this target.
include CMakeFiles/Cherenkov.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Cherenkov.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Cherenkov.dir/flags.make

CMakeFiles/Cherenkov.dir/Cherenkov.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/Cherenkov.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/Cherenkov.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Cherenkov.dir/Cherenkov.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/Cherenkov.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/Cherenkov.cc

CMakeFiles/Cherenkov.dir/Cherenkov.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/Cherenkov.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/Cherenkov.cc > CMakeFiles/Cherenkov.dir/Cherenkov.cc.i

CMakeFiles/Cherenkov.dir/Cherenkov.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/Cherenkov.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/Cherenkov.cc -o CMakeFiles/Cherenkov.dir/Cherenkov.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovDetectorConstruction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovDetectorConstruction.cc

CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovDetectorConstruction.cc > CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovDetectorConstruction.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovEventAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovEventAction.cc

CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovEventAction.cc > CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovEventAction.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsList.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsList.cc

CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsList.cc > CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsList.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsListMessenger.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsListMessenger.cc

CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsListMessenger.cc > CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPhysicsListMessenger.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorAction.cc

CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorAction.cc > CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorAction.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorMessenger.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorMessenger.cc

CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorMessenger.cc > CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovPrimaryGeneratorMessenger.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovRunAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovRunAction.cc

CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovRunAction.cc > CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovRunAction.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovStackingAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovStackingAction.cc

CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovStackingAction.cc > CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovStackingAction.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.s

CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovSteppingVerbose.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovSteppingVerbose.cc

CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovSteppingVerbose.cc > CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.i

CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/CherenkovSteppingVerbose.cc -o CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.s

CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.o: CMakeFiles/Cherenkov.dir/flags.make
CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.o: /remote/tesla/bmanning/work/Geant4Simulations/Station/src/StationSteppingAction.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.o -c /remote/tesla/bmanning/work/Geant4Simulations/Station/src/StationSteppingAction.cc

CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /remote/tesla/bmanning/work/Geant4Simulations/Station/src/StationSteppingAction.cc > CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.i

CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /remote/tesla/bmanning/work/Geant4Simulations/Station/src/StationSteppingAction.cc -o CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.s

# Object files for target Cherenkov
Cherenkov_OBJECTS = \
"CMakeFiles/Cherenkov.dir/Cherenkov.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.o" \
"CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.o" \
"CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.o"

# External object files for target Cherenkov
Cherenkov_EXTERNAL_OBJECTS =

Cherenkov: CMakeFiles/Cherenkov.dir/Cherenkov.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovDetectorConstruction.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovEventAction.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsList.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovPhysicsListMessenger.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorAction.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovPrimaryGeneratorMessenger.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovRunAction.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovStackingAction.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/CherenkovSteppingVerbose.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/src/StationSteppingAction.cc.o
Cherenkov: CMakeFiles/Cherenkov.dir/build.make
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4Tree.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4GMocren.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4visHepRep.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4RayTracer.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4VRML.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4OpenGL.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4gl2ps.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4interfaces.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4persistency.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4error_propagation.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4readout.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4physicslists.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4parmodels.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4FR.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4vis_management.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4modeling.so
Cherenkov: /usr/lib64/libGL.so
Cherenkov: /remote/tesla/bmanning/Qt5.7.0/5.7/gcc_64/lib/libQt5OpenGL.so.5.7.0
Cherenkov: /remote/tesla/bmanning/Qt5.7.0/5.7/gcc_64/lib/libQt5PrintSupport.so.5.7.0
Cherenkov: /remote/tesla/bmanning/Qt5.7.0/5.7/gcc_64/lib/libQt5Widgets.so.5.7.0
Cherenkov: /remote/tesla/bmanning/Qt5.7.0/5.7/gcc_64/lib/libQt5Gui.so.5.7.0
Cherenkov: /remote/tesla/bmanning/Qt5.7.0/5.7/gcc_64/lib/libQt5Core.so.5.7.0
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4run.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4event.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4tracking.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4processes.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4analysis.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4expat.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4zlib.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4digits_hits.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4track.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4particles.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4geometry.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4materials.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4graphics_reps.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4intercoms.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/geant4/10.04.p01/lib64/libG4global.so
Cherenkov: /remote/tesla/bmanning/auger/software/ApeInstall/External/clhep/2.4.0.4/lib/libCLHEP-2.4.0.4.so
Cherenkov: CMakeFiles/Cherenkov.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable Cherenkov"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Cherenkov.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Cherenkov.dir/build: Cherenkov

.PHONY : CMakeFiles/Cherenkov.dir/build

CMakeFiles/Cherenkov.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Cherenkov.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Cherenkov.dir/clean

CMakeFiles/Cherenkov.dir/depend:
	cd /remote/tesla/bmanning/work/Geant4Simulations/Station-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /remote/tesla/bmanning/work/Geant4Simulations/Station /remote/tesla/bmanning/work/Geant4Simulations/Station /remote/tesla/bmanning/work/Geant4Simulations/Station-build /remote/tesla/bmanning/work/Geant4Simulations/Station-build /remote/tesla/bmanning/work/Geant4Simulations/Station-build/CMakeFiles/Cherenkov.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Cherenkov.dir/depend

