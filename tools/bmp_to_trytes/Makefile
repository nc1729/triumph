# --This is a Makefile from StackOverflow I adapted--
# First define variables used in the Makefile, then start defining rules
# this Makefile assumes main.cpp as 'first amongst equals'

# Usage
# make

# Notation (for my reference)
# $@ - macro that refers to the target (the rule name)
# $^ - macro that refers to all dependencies
# $< - macro that refers to first dependency
# % -  macro to make a pattern that we want to watch in both the target and the dependency
# @ at the start of a command - stop the console from repeating it


#
# Compiler flags
#
CC = g++
CFLAGS = -std=c++17 -Wall -Werror -Wextra

#
# Project files
#
SRCS = Tryte.cpp Bitmap.cpp
#TESTS = float_tests.cpp trint_tests.cpp
HEADERDIR = ./include
OBJS = $(SRCS:.cpp=.o)
EXE = bmp_to_trytes

#
# Debug build settings
#
DBGDIR = build/debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -DDEBUG

#
# Release build settings
#
RELDIR = build/release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O2 -DNDEBUG

#
# Test executable build settings
#
TESTDIR = build/test
TESTEXE = $(TESTDIR)/test
TESTOBJS = $(addprefix $(TESTDIR)/, $(TESTS:.cpp=.o))
TESTCFLAGS = -g -DDEBUG

# Makes Makefile always see these as tasks, rather than potential files
.PHONY: all clean debug prep test debug_prep release_prep release remake

# Default build
all: release_prep release

#
# Debug rules
#
debug: debug_prep $(DBGEXE)

$(DBGEXE): $(DBGOBJS) $(DBGDIR)/main.o
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: src/%.cpp src/main.cpp
	$(CC) -I $(HEADERDIR) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS) $(RELDIR)/main.o
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

$(RELDIR)/%.o: src/%.cpp src/main.cpp
	$(CC) -I $(HEADERDIR) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

#
# Test rules
#
test: debug_prep test_prep $(DBGOBJS) $(TESTEXE)

$(DBGDIR)/%.o: src/%.cpp src/main.cpp
	$(CC) -I $(HEADERDIR) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

$(TESTEXE): $(DBGOBJS) $(TESTOBJS) $(TESTDIR)/test.o
	$(CC) $(CFLAGS) $(TESTCFLAGS) -o $(TESTEXE) $^

$(TESTDIR)/%.o: test/%.cpp test/test.cpp
	$(CC) -I $(HEADERDIR) -c $(CFLAGS) $(TESTCFLAGS) -o $@ $<
	
#
# Other rules
#
debug_prep:
	@mkdir -p $(DBGDIR)

release_prep:
	@mkdir -p $(RELDIR)

test_prep:
	@mkdir -p $(TESTDIR)

prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS) $(TESTOBJS) $(TESTEXE) build/test/test.o \
	build/debug/main.o build/release/main.o