CC = g++
CCFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic
RELEASE = -O2 -DNDEBUG $(CCFLAGS)
DEBUG = -O0 -g $(CCFLAGS)
AR = ar
ARFLAGS = rcss


SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = inc
LIBDIR = lib

NAME = CSE312_HW2
MAINSRC = main.cpp
MAINOBJ = $(OBJDIR)/main.o
MAINBIN = $(BINDIR)/main.out
LIB = $(LIBDIR)/lib$(NAME).a

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
INCS = $(wildcard $(INCDIR)/*.hpp)


all: $(MAINBIN)

$(MAINBIN): $(MAINOBJ) $(LIB)
	@mkdir -p $(BINDIR)
	$(CC) $(RELEASE) -o $@ $< -L$(LIBDIR) -l$(NAME)

$(MAINOBJ): $(MAINSRC) $(INCS)
	@mkdir -p $(OBJDIR)
	$(CC) $(RELEASE) -c -o $@ $< -I$(INCDIR)

$(LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCS)
	@mkdir -p $(OBJDIR)
	$(CC) $(RELEASE) -c -o $@ $< -I$(INCDIR)


DEBUGOBJDIR = $(OBJDIR)/debug

DEBUGBIN = $(BINDIR)/debug.out
DEBUGMAINOBJ = $(DEBUGOBJDIR)/main.o
DEBUGOBJS = $(patsubst $(OBJDIR)/%.o, $(DEBUGOBJDIR)/%.o, $(OBJS))
DEBUGLIB = $(LIBDIR)/lib$(NAME)_debug.a

debug: $(DEBUGBIN)

$(DEBUGBIN): $(DEBUGMAINOBJ) $(DEBUGLIB)
	@mkdir -p $(BINDIR)
	$(CC) $(DEBUG) -o $@ $< -L$(LIBDIR) -l$(NAME)_debug

$(DEBUGMAINOBJ): $(MAINSRC) $(INCS)
	@mkdir -p $(DEBUGOBJDIR)
	$(CC) $(DEBUG) -c -o $@ $< -I$(INCDIR)

$(DEBUGLIB): $(DEBUGOBJS)
	@mkdir -p $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $^

$(DEBUGOBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCS)
	@mkdir -p $(DEBUGOBJDIR)
	$(CC) $(DEBUG) -c -o $@ $< -I$(INCDIR)

clean:
	rm -rf $(OBJDIR) $(LIBDIR)

fclean: clean
	rm -rf $(BINDIR)

re: fclean all

.PHONY: all debug clean fclean re
