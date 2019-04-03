# ==================================================================================== #
# BINARY NAMES
# ==================================================================================== #

BIN_BENCHMARK 		 = benchmark
BIN_TEST_INTEGRATION = test_integration

# ==================================================================================== #
# CONFIG
# ==================================================================================== #

CC       = g++
CPPEXT   = cpp
CEXT     = c

SRCDIR   = src
TESTDIR  = tests
OBJDIR   = obj
BINDIR   = bin

DEBUG    = -g
INCLUDES = -I./include
LIBS     = -lm -lcriterion
CFLAGS   = -Wall -std=c++11 -c $(GIT_HASH) $(DEBUG) $(INCLUDES) $(LIBS)

# Filter out files which have a main function from test dependencies
FILTER_MAINS = run_benchmark.o

# ==================================================================================== #
# OBJECTS HANDLING
# ==================================================================================== #

# FOLDERS HOLDING SRC FILES
SRCDIRS = $(SRCDIR) $(TESTDIR)

# SOURCES
C_SRCS         = $(wildcard $(SRCDIR)/*.c)
CPP_SRCS       = $(wildcard $(SRCDIR)/*.cpp)
C_OBJS        := $(patsubst %.$(CEXT),   $(OBJDIR)/%.o, $(C_SRCS))
CPP_OBJS      := $(patsubst %.$(CPPEXT), $(OBJDIR)/%.o, $(CPP_SRCS))
OBJS           = $(C_OBJS) $(CPP_OBJS)

# TEST SOURCES
TEST_C_SRCS    = $(wildcard $(TESTDIR)/*.c)
TEST_CPP_SRCS  = $(wildcard $(TESTDIR)/*.cpp)
TEST_C_OBJS   := $(patsubst %.$(CEXT),   $(OBJDIR)/%.o, $(TEST_C_SRCS))
TEST_CPP_OBJS := $(patsubst %.$(CPPEXT), $(OBJDIR)/%.o, $(TEST_CPP_SRCS))
FILTER_MAIN_OBJS = $(addprefix $(OBJDIR)/$(SRCDIR)/, $(FILTER_MAINS))
TEST_OBJS      = $(filter-out $(FILTER_MAIN_OBJS), $(OBJS)) $(TEST_C_OBJS) $(TEST_CPP_OBJS)

GIT_HASH       = -DGIT_VERSION=\"$(GIT_VERSION)\" \
			     -DGIT_COMMIT=\"$(GIT_COMMIT)\"   \
			     -DGIT_DATE=\"$(GIT_DATE)\"       \

# ==================================================================================== #
# TARGETS
# ==================================================================================== #

# Compiles all tests and runs them
test: $(BINDIR)/$(BIN_TEST_INTEGRATION)
	$(BINDIR)/$(BIN_TEST_INTEGRATION)

# Compiles the benchmark executable based of run_benchmark.cpp
benchmark: $(BINDIR)/$(BIN_BENCHMARK)

$(BINDIR)/$(BIN_BENCHMARK): buildrepo $(OBJS)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(OBJS) $(LIBS) -o $@

$(BINDIR)/$(BIN_TEST_INTEGRATION): buildrepo $(TEST_OBJS)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(TEST_OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: %.$(CEXT)
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.$(CPPEXT)
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -r $(OBJDIR)

distclean: clean
	$(RM) -r $(BINDIR)

cleanall: clean distclean

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef

# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFLAGS) \
        $1
endef

.PHONY: all clean distclean

# For debugging the Makefile, prints variables, use on command line like: make print-GIT-HASH
print-%  : ; @echo $* = $($*)