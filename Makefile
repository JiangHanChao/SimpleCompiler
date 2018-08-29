# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /media/script/Windows/Users/angerol/Desktop/git/Compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/script/Windows/Users/angerol/Desktop/git/Compiler

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /media/script/Windows/Users/angerol/Desktop/git/Compiler/CMakeFiles /media/script/Windows/Users/angerol/Desktop/git/Compiler/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /media/script/Windows/Users/angerol/Desktop/git/Compiler/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Compiler

# Build rule for target.
Compiler: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Compiler
.PHONY : Compiler

# fast build rule for target.
Compiler/fast:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/build
.PHONY : Compiler/fast

src/analysis.o: src/analysis.cpp.o

.PHONY : src/analysis.o

# target to build an object file
src/analysis.cpp.o:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/analysis.cpp.o
.PHONY : src/analysis.cpp.o

src/analysis.i: src/analysis.cpp.i

.PHONY : src/analysis.i

# target to preprocess a source file
src/analysis.cpp.i:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/analysis.cpp.i
.PHONY : src/analysis.cpp.i

src/analysis.s: src/analysis.cpp.s

.PHONY : src/analysis.s

# target to generate assembly for a file
src/analysis.cpp.s:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/analysis.cpp.s
.PHONY : src/analysis.cpp.s

src/basic.o: src/basic.cpp.o

.PHONY : src/basic.o

# target to build an object file
src/basic.cpp.o:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/basic.cpp.o
.PHONY : src/basic.cpp.o

src/basic.i: src/basic.cpp.i

.PHONY : src/basic.i

# target to preprocess a source file
src/basic.cpp.i:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/basic.cpp.i
.PHONY : src/basic.cpp.i

src/basic.s: src/basic.cpp.s

.PHONY : src/basic.s

# target to generate assembly for a file
src/basic.cpp.s:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/basic.cpp.s
.PHONY : src/basic.cpp.s

src/codemk.o: src/codemk.cpp.o

.PHONY : src/codemk.o

# target to build an object file
src/codemk.cpp.o:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/codemk.cpp.o
.PHONY : src/codemk.cpp.o

src/codemk.i: src/codemk.cpp.i

.PHONY : src/codemk.i

# target to preprocess a source file
src/codemk.cpp.i:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/codemk.cpp.i
.PHONY : src/codemk.cpp.i

src/codemk.s: src/codemk.cpp.s

.PHONY : src/codemk.s

# target to generate assembly for a file
src/codemk.cpp.s:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/codemk.cpp.s
.PHONY : src/codemk.cpp.s

src/compiler.o: src/compiler.cpp.o

.PHONY : src/compiler.o

# target to build an object file
src/compiler.cpp.o:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/compiler.cpp.o
.PHONY : src/compiler.cpp.o

src/compiler.i: src/compiler.cpp.i

.PHONY : src/compiler.i

# target to preprocess a source file
src/compiler.cpp.i:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/compiler.cpp.i
.PHONY : src/compiler.cpp.i

src/compiler.s: src/compiler.cpp.s

.PHONY : src/compiler.s

# target to generate assembly for a file
src/compiler.cpp.s:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/compiler.cpp.s
.PHONY : src/compiler.cpp.s

src/lexay.o: src/lexay.cpp.o

.PHONY : src/lexay.o

# target to build an object file
src/lexay.cpp.o:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/lexay.cpp.o
.PHONY : src/lexay.cpp.o

src/lexay.i: src/lexay.cpp.i

.PHONY : src/lexay.i

# target to preprocess a source file
src/lexay.cpp.i:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/lexay.cpp.i
.PHONY : src/lexay.cpp.i

src/lexay.s: src/lexay.cpp.s

.PHONY : src/lexay.s

# target to generate assembly for a file
src/lexay.cpp.s:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/lexay.cpp.s
.PHONY : src/lexay.cpp.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/syntax.o: src/syntax.cpp.o

.PHONY : src/syntax.o

# target to build an object file
src/syntax.cpp.o:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/syntax.cpp.o
.PHONY : src/syntax.cpp.o

src/syntax.i: src/syntax.cpp.i

.PHONY : src/syntax.i

# target to preprocess a source file
src/syntax.cpp.i:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/syntax.cpp.i
.PHONY : src/syntax.cpp.i

src/syntax.s: src/syntax.cpp.s

.PHONY : src/syntax.s

# target to generate assembly for a file
src/syntax.cpp.s:
	$(MAKE) -f CMakeFiles/Compiler.dir/build.make CMakeFiles/Compiler.dir/src/syntax.cpp.s
.PHONY : src/syntax.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... Compiler"
	@echo "... src/analysis.o"
	@echo "... src/analysis.i"
	@echo "... src/analysis.s"
	@echo "... src/basic.o"
	@echo "... src/basic.i"
	@echo "... src/basic.s"
	@echo "... src/codemk.o"
	@echo "... src/codemk.i"
	@echo "... src/codemk.s"
	@echo "... src/compiler.o"
	@echo "... src/compiler.i"
	@echo "... src/compiler.s"
	@echo "... src/lexay.o"
	@echo "... src/lexay.i"
	@echo "... src/lexay.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/syntax.o"
	@echo "... src/syntax.i"
	@echo "... src/syntax.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
