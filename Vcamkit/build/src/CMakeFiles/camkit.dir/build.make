# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_SOURCE_DIR = /home/pi/ROV/Vcamkit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/ROV/Vcamkit/build

# Include any dependencies generated for this target.
include src/CMakeFiles/camkit.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/camkit.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/camkit.dir/flags.make

src/CMakeFiles/camkit.dir/soft_convert.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/soft_convert.c.o: ../src/soft_convert.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/soft_convert.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/soft_convert.c.o   -c /home/pi/ROV/Vcamkit/src/soft_convert.c

src/CMakeFiles/camkit.dir/soft_convert.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/soft_convert.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/soft_convert.c > CMakeFiles/camkit.dir/soft_convert.c.i

src/CMakeFiles/camkit.dir/soft_convert.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/soft_convert.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/soft_convert.c -o CMakeFiles/camkit.dir/soft_convert.c.s

src/CMakeFiles/camkit.dir/soft_convert.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/soft_convert.c.o.requires

src/CMakeFiles/camkit.dir/soft_convert.c.o.provides: src/CMakeFiles/camkit.dir/soft_convert.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/soft_convert.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/soft_convert.c.o.provides

src/CMakeFiles/camkit.dir/soft_convert.c.o.provides.build: src/CMakeFiles/camkit.dir/soft_convert.c.o

src/CMakeFiles/camkit.dir/omx_encode.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/omx_encode.c.o: ../src/omx_encode.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/omx_encode.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/omx_encode.c.o   -c /home/pi/ROV/Vcamkit/src/omx_encode.c

src/CMakeFiles/camkit.dir/omx_encode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/omx_encode.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/omx_encode.c > CMakeFiles/camkit.dir/omx_encode.c.i

src/CMakeFiles/camkit.dir/omx_encode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/omx_encode.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/omx_encode.c -o CMakeFiles/camkit.dir/omx_encode.c.s

src/CMakeFiles/camkit.dir/omx_encode.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/omx_encode.c.o.requires

src/CMakeFiles/camkit.dir/omx_encode.c.o.provides: src/CMakeFiles/camkit.dir/omx_encode.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/omx_encode.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/omx_encode.c.o.provides

src/CMakeFiles/camkit.dir/omx_encode.c.o.provides.build: src/CMakeFiles/camkit.dir/omx_encode.c.o

src/CMakeFiles/camkit.dir/v4l_capture.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/v4l_capture.c.o: ../src/v4l_capture.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/v4l_capture.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/v4l_capture.c.o   -c /home/pi/ROV/Vcamkit/src/v4l_capture.c

src/CMakeFiles/camkit.dir/v4l_capture.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/v4l_capture.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/v4l_capture.c > CMakeFiles/camkit.dir/v4l_capture.c.i

src/CMakeFiles/camkit.dir/v4l_capture.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/v4l_capture.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/v4l_capture.c -o CMakeFiles/camkit.dir/v4l_capture.c.s

src/CMakeFiles/camkit.dir/v4l_capture.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/v4l_capture.c.o.requires

src/CMakeFiles/camkit.dir/v4l_capture.c.o.provides: src/CMakeFiles/camkit.dir/v4l_capture.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/v4l_capture.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/v4l_capture.c.o.provides

src/CMakeFiles/camkit.dir/v4l_capture.c.o.provides.build: src/CMakeFiles/camkit.dir/v4l_capture.c.o

src/CMakeFiles/camkit.dir/rtp_pack.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/rtp_pack.c.o: ../src/rtp_pack.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/rtp_pack.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/rtp_pack.c.o   -c /home/pi/ROV/Vcamkit/src/rtp_pack.c

src/CMakeFiles/camkit.dir/rtp_pack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/rtp_pack.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/rtp_pack.c > CMakeFiles/camkit.dir/rtp_pack.c.i

src/CMakeFiles/camkit.dir/rtp_pack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/rtp_pack.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/rtp_pack.c -o CMakeFiles/camkit.dir/rtp_pack.c.s

src/CMakeFiles/camkit.dir/rtp_pack.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/rtp_pack.c.o.requires

src/CMakeFiles/camkit.dir/rtp_pack.c.o.provides: src/CMakeFiles/camkit.dir/rtp_pack.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/rtp_pack.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/rtp_pack.c.o.provides

src/CMakeFiles/camkit.dir/rtp_pack.c.o.provides.build: src/CMakeFiles/camkit.dir/rtp_pack.c.o

src/CMakeFiles/camkit.dir/network.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/network.c.o: ../src/network.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/network.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/network.c.o   -c /home/pi/ROV/Vcamkit/src/network.c

src/CMakeFiles/camkit.dir/network.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/network.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/network.c > CMakeFiles/camkit.dir/network.c.i

src/CMakeFiles/camkit.dir/network.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/network.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/network.c -o CMakeFiles/camkit.dir/network.c.s

src/CMakeFiles/camkit.dir/network.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/network.c.o.requires

src/CMakeFiles/camkit.dir/network.c.o.provides: src/CMakeFiles/camkit.dir/network.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/network.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/network.c.o.provides

src/CMakeFiles/camkit.dir/network.c.o.provides.build: src/CMakeFiles/camkit.dir/network.c.o

src/CMakeFiles/camkit.dir/timestamp.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/timestamp.c.o: ../src/timestamp.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/timestamp.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/timestamp.c.o   -c /home/pi/ROV/Vcamkit/src/timestamp.c

src/CMakeFiles/camkit.dir/timestamp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/timestamp.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/timestamp.c > CMakeFiles/camkit.dir/timestamp.c.i

src/CMakeFiles/camkit.dir/timestamp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/timestamp.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/timestamp.c -o CMakeFiles/camkit.dir/timestamp.c.s

src/CMakeFiles/camkit.dir/timestamp.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/timestamp.c.o.requires

src/CMakeFiles/camkit.dir/timestamp.c.o.provides: src/CMakeFiles/camkit.dir/timestamp.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/timestamp.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/timestamp.c.o.provides

src/CMakeFiles/camkit.dir/timestamp.c.o.provides.build: src/CMakeFiles/camkit.dir/timestamp.c.o

src/CMakeFiles/camkit.dir/rtmp_send.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/rtmp_send.c.o: ../src/rtmp_send.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/rtmp_send.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/rtmp_send.c.o   -c /home/pi/ROV/Vcamkit/src/rtmp_send.c

src/CMakeFiles/camkit.dir/rtmp_send.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/rtmp_send.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/rtmp_send.c > CMakeFiles/camkit.dir/rtmp_send.c.i

src/CMakeFiles/camkit.dir/rtmp_send.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/rtmp_send.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/rtmp_send.c -o CMakeFiles/camkit.dir/rtmp_send.c.s

src/CMakeFiles/camkit.dir/rtmp_send.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/rtmp_send.c.o.requires

src/CMakeFiles/camkit.dir/rtmp_send.c.o.provides: src/CMakeFiles/camkit.dir/rtmp_send.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/rtmp_send.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/rtmp_send.c.o.provides

src/CMakeFiles/camkit.dir/rtmp_send.c.o.provides.build: src/CMakeFiles/camkit.dir/rtmp_send.c.o

src/CMakeFiles/camkit.dir/simple_image.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/simple_image.c.o: ../src/simple_image.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/simple_image.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/simple_image.c.o   -c /home/pi/ROV/Vcamkit/src/simple_image.c

src/CMakeFiles/camkit.dir/simple_image.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/simple_image.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/src/simple_image.c > CMakeFiles/camkit.dir/simple_image.c.i

src/CMakeFiles/camkit.dir/simple_image.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/simple_image.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/src/simple_image.c -o CMakeFiles/camkit.dir/simple_image.c.s

src/CMakeFiles/camkit.dir/simple_image.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/simple_image.c.o.requires

src/CMakeFiles/camkit.dir/simple_image.c.o.provides: src/CMakeFiles/camkit.dir/simple_image.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/simple_image.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/simple_image.c.o.provides

src/CMakeFiles/camkit.dir/simple_image.c.o.provides.build: src/CMakeFiles/camkit.dir/simple_image.c.o

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o: ../third-party/ilclient/ilclient.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o   -c /home/pi/ROV/Vcamkit/third-party/ilclient/ilclient.c

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/third-party/ilclient/ilclient.c > CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.i

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/third-party/ilclient/ilclient.c -o CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.s

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.requires

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.provides: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.provides

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.provides.build: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o: src/CMakeFiles/camkit.dir/flags.make
src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o: ../third-party/ilclient/ilcore.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/ROV/Vcamkit/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o   -c /home/pi/ROV/Vcamkit/third-party/ilclient/ilcore.c

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.i"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/ROV/Vcamkit/third-party/ilclient/ilcore.c > CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.i

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.s"
	cd /home/pi/ROV/Vcamkit/build/src && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/ROV/Vcamkit/third-party/ilclient/ilcore.c -o CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.s

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.requires:
.PHONY : src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.requires

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.provides: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.requires
	$(MAKE) -f src/CMakeFiles/camkit.dir/build.make src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.provides.build
.PHONY : src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.provides

src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.provides.build: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o

# Object files for target camkit
camkit_OBJECTS = \
"CMakeFiles/camkit.dir/soft_convert.c.o" \
"CMakeFiles/camkit.dir/omx_encode.c.o" \
"CMakeFiles/camkit.dir/v4l_capture.c.o" \
"CMakeFiles/camkit.dir/rtp_pack.c.o" \
"CMakeFiles/camkit.dir/network.c.o" \
"CMakeFiles/camkit.dir/timestamp.c.o" \
"CMakeFiles/camkit.dir/rtmp_send.c.o" \
"CMakeFiles/camkit.dir/simple_image.c.o" \
"CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o" \
"CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o"

# External object files for target camkit
camkit_EXTERNAL_OBJECTS =

src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/soft_convert.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/omx_encode.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/v4l_capture.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/rtp_pack.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/network.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/timestamp.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/rtmp_send.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/simple_image.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/build.make
src/libcamkit.so.0.2: /opt/vc/lib/libvcos.so
src/libcamkit.so.0.2: /opt/vc/lib/libvcsm.so
src/libcamkit.so.0.2: /opt/vc/lib/libbcm_host.so
src/libcamkit.so.0.2: /opt/vc/lib/libopenmaxil.so
src/libcamkit.so.0.2: /opt/vc/lib/libvchiq_arm.so
src/libcamkit.so.0.2: /usr/local/lib/librtmp.so
src/libcamkit.so.0.2: src/CMakeFiles/camkit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library libcamkit.so"
	cd /home/pi/ROV/Vcamkit/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/camkit.dir/link.txt --verbose=$(VERBOSE)
	cd /home/pi/ROV/Vcamkit/build/src && $(CMAKE_COMMAND) -E cmake_symlink_library libcamkit.so.0.2 libcamkit.so.0 libcamkit.so

src/libcamkit.so.0: src/libcamkit.so.0.2

src/libcamkit.so: src/libcamkit.so.0.2

# Rule to build all files generated by this target.
src/CMakeFiles/camkit.dir/build: src/libcamkit.so
.PHONY : src/CMakeFiles/camkit.dir/build

src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/soft_convert.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/omx_encode.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/v4l_capture.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/rtp_pack.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/network.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/timestamp.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/rtmp_send.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/simple_image.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilclient.c.o.requires
src/CMakeFiles/camkit.dir/requires: src/CMakeFiles/camkit.dir/__/third-party/ilclient/ilcore.c.o.requires
.PHONY : src/CMakeFiles/camkit.dir/requires

src/CMakeFiles/camkit.dir/clean:
	cd /home/pi/ROV/Vcamkit/build/src && $(CMAKE_COMMAND) -P CMakeFiles/camkit.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/camkit.dir/clean

src/CMakeFiles/camkit.dir/depend:
	cd /home/pi/ROV/Vcamkit/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/ROV/Vcamkit /home/pi/ROV/Vcamkit/src /home/pi/ROV/Vcamkit/build /home/pi/ROV/Vcamkit/build/src /home/pi/ROV/Vcamkit/build/src/CMakeFiles/camkit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/camkit.dir/depend

