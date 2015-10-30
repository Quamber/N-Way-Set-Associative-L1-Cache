# Usage: just run make, it will pick up this Makefile as the default

# Adding new Files to the Makefile
# If u have different filenames or want to add any more sources add
# them to this list of SRCS
SRCS = main.cc funcs.cc cacheline.cpp common.cc CacheSet.cpp

CC = g++
LD = g++

# Compiler options
# IF u want to add any other compiler options add them to CPPFLAGS
# Debug information
# The -g option in CPPFLAGS will generate debug information,
# this is required if u want to use a debugger like gdb
# Hint if you are used to VC style of debugging a visual debugger 'ddd'
# is available on linux which should be very much like VC and make life simpler
# while debugging. It actually uses the gdb but you dont have to know the
# commands of gdb to use it.
CPPFLAGS = -g

IFLAGS =
LDFLAGS =

# Using extra libraries
# If u are using any extra libraries u will have to update the LIBS =
LIBS =

# Output of the make
# The Makefile produces your final binary as cache,
# Changing output filename
PROGRAM = Cache

OBJS = $(SRCS:.cc=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)


# Recompiling all sources
# If u want to recompile all your sourses run
# 	make clean
# and then make
clean:
	rm -f $(OBJS) $(PROGRAM)
