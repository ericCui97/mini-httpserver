# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cui/unix_env_programming/http_parser

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cui/unix_env_programming/http_parser/build

# Include any dependencies generated for this target.
include CMakeFiles/epollserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/epollserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/epollserver.dir/flags.make

CMakeFiles/epollserver.dir/epoll-server.cpp.o: CMakeFiles/epollserver.dir/flags.make
CMakeFiles/epollserver.dir/epoll-server.cpp.o: ../epoll-server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cui/unix_env_programming/http_parser/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/epollserver.dir/epoll-server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epollserver.dir/epoll-server.cpp.o -c /home/cui/unix_env_programming/http_parser/epoll-server.cpp

CMakeFiles/epollserver.dir/epoll-server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epollserver.dir/epoll-server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cui/unix_env_programming/http_parser/epoll-server.cpp > CMakeFiles/epollserver.dir/epoll-server.cpp.i

CMakeFiles/epollserver.dir/epoll-server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epollserver.dir/epoll-server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cui/unix_env_programming/http_parser/epoll-server.cpp -o CMakeFiles/epollserver.dir/epoll-server.cpp.s

CMakeFiles/epollserver.dir/epoll-server.cpp.o.requires:

.PHONY : CMakeFiles/epollserver.dir/epoll-server.cpp.o.requires

CMakeFiles/epollserver.dir/epoll-server.cpp.o.provides: CMakeFiles/epollserver.dir/epoll-server.cpp.o.requires
	$(MAKE) -f CMakeFiles/epollserver.dir/build.make CMakeFiles/epollserver.dir/epoll-server.cpp.o.provides.build
.PHONY : CMakeFiles/epollserver.dir/epoll-server.cpp.o.provides

CMakeFiles/epollserver.dir/epoll-server.cpp.o.provides.build: CMakeFiles/epollserver.dir/epoll-server.cpp.o


CMakeFiles/epollserver.dir/http.cpp.o: CMakeFiles/epollserver.dir/flags.make
CMakeFiles/epollserver.dir/http.cpp.o: ../http.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cui/unix_env_programming/http_parser/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/epollserver.dir/http.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epollserver.dir/http.cpp.o -c /home/cui/unix_env_programming/http_parser/http.cpp

CMakeFiles/epollserver.dir/http.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epollserver.dir/http.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cui/unix_env_programming/http_parser/http.cpp > CMakeFiles/epollserver.dir/http.cpp.i

CMakeFiles/epollserver.dir/http.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epollserver.dir/http.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cui/unix_env_programming/http_parser/http.cpp -o CMakeFiles/epollserver.dir/http.cpp.s

CMakeFiles/epollserver.dir/http.cpp.o.requires:

.PHONY : CMakeFiles/epollserver.dir/http.cpp.o.requires

CMakeFiles/epollserver.dir/http.cpp.o.provides: CMakeFiles/epollserver.dir/http.cpp.o.requires
	$(MAKE) -f CMakeFiles/epollserver.dir/build.make CMakeFiles/epollserver.dir/http.cpp.o.provides.build
.PHONY : CMakeFiles/epollserver.dir/http.cpp.o.provides

CMakeFiles/epollserver.dir/http.cpp.o.provides.build: CMakeFiles/epollserver.dir/http.cpp.o


# Object files for target epollserver
epollserver_OBJECTS = \
"CMakeFiles/epollserver.dir/epoll-server.cpp.o" \
"CMakeFiles/epollserver.dir/http.cpp.o"

# External object files for target epollserver
epollserver_EXTERNAL_OBJECTS =

epollserver: CMakeFiles/epollserver.dir/epoll-server.cpp.o
epollserver: CMakeFiles/epollserver.dir/http.cpp.o
epollserver: CMakeFiles/epollserver.dir/build.make
epollserver: CMakeFiles/epollserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cui/unix_env_programming/http_parser/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable epollserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/epollserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/epollserver.dir/build: epollserver

.PHONY : CMakeFiles/epollserver.dir/build

CMakeFiles/epollserver.dir/requires: CMakeFiles/epollserver.dir/epoll-server.cpp.o.requires
CMakeFiles/epollserver.dir/requires: CMakeFiles/epollserver.dir/http.cpp.o.requires

.PHONY : CMakeFiles/epollserver.dir/requires

CMakeFiles/epollserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/epollserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/epollserver.dir/clean

CMakeFiles/epollserver.dir/depend:
	cd /home/cui/unix_env_programming/http_parser/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cui/unix_env_programming/http_parser /home/cui/unix_env_programming/http_parser /home/cui/unix_env_programming/http_parser/build /home/cui/unix_env_programming/http_parser/build /home/cui/unix_env_programming/http_parser/build/CMakeFiles/epollserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/epollserver.dir/depend

