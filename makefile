CC = g++
CCFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic
RELEASE = -O2 -DNDEBUG $(CCFLAGS)
DEBUG = -O0 -g $(CCFLAGS)
AR = ar
ARFLAGS = rcs


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

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
NC = \033[0m

all: $(MAINBIN)

$(MAINBIN): $(MAINOBJ) $(LIB)
	@printf "${GREEN}Creating binary $@...${NC}\n"
	@mkdir -p $(BINDIR)
	@$(CC) $(RELEASE) -o $@ $< -L$(LIBDIR) -l$(NAME)

$(MAINOBJ): $(MAINSRC) $(INCS)
	@printf "${BLUE}Compiling $<...${NC}\n"
	@mkdir -p $(OBJDIR)
	@$(CC) $(RELEASE) -c -o $@ $< -I$(INCDIR)

$(LIB): $(OBJS)
	@printf "${YELLOW}Creating library $@...${NC}\n"
	@mkdir -p $(LIBDIR)
	@$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCS)
	@printf "${BLUE}Compiling $<...${NC}\n"
	@mkdir -p $(OBJDIR)
	@$(CC) $(RELEASE) -c -o $@ $< -I$(INCDIR)

DEBUGOBJDIR = $(OBJDIR)/debug

DEBUGBIN = $(BINDIR)/debug.out
DEBUGMAINOBJ = $(DEBUGOBJDIR)/main.o
DEBUGOBJS = $(patsubst $(OBJDIR)/%.o, $(DEBUGOBJDIR)/%.o, $(OBJS))
DEBUGLIB = $(LIBDIR)/lib$(NAME)_debug.a

debug: $(DEBUGBIN)

$(DEBUGBIN): $(DEBUGMAINOBJ) $(DEBUGLIB)
	@printf "${GREEN}Creating debug binary $@...${NC}\n"
	@mkdir -p $(BINDIR)
	@$(CC) $(DEBUG) -o $@ $< -L$(LIBDIR) -l$(NAME)_debug

$(DEBUGMAINOBJ): $(MAINSRC) $(INCS)
	@printf "${BLUE}Compiling $<...${NC}\n"
	@mkdir -p $(DEBUGOBJDIR)
	@$(CC) $(DEBUG) -c -o $@ $< -I$(INCDIR)

$(DEBUGLIB): $(DEBUGOBJS)
	@printf "${YELLOW}Creating debug library $@...${NC}\n"
	@mkdir -p $(LIBDIR)
	@$(AR) $(ARFLAGS) $@ $^

$(DEBUGOBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCS)
	@printf "${BLUE}Compiling $<...${NC}\n"
	@mkdir -p $(DEBUGOBJDIR)
	@$(CC) $(DEBUG) -c -o $@ $< -I$(INCDIR)

clean:
	@printf "${RED}Cleaning...${NC}\n"
	@rm -rf $(OBJDIR) $(LIBDIR) $(DEBUGOBJDIR)

fclean: clean
	@rm -rf $(BINDIR)

run: $(MAINBIN)
	@printf "${GREEN}Running $<...${NC}\n"
	@./$(MAINBIN)

debugrun: $(DEBUGBIN)
	@printf "${GREEN}Running $<...${NC}\n"
	@./$(DEBUGBIN)

re: fclean all

.PHONY: all debug clean fclean re run debugrun
