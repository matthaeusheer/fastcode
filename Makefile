# ==================================================================================== #
# = Executable
# ==================================================================================== #

BIN_RUN_BENCHMARK = run_benchmark
BIN_INTEGRATION_TEST = test_integration

OBJ_DIR = obj/
SRC_DIR = src/
BIN_DIR = bin/

INCLUDES = -I./include

# ==================================================================================== #
# = GIT RECORD
# ==================================================================================== #

GIT_VERSION := $(shell git --no-pager describe --tags --always)
GIT_COMMIT  := $(shell git rev-parse --verify HEAD)
GIT_DATE    := $(firstword $(shell git --no-pager show --date=iso-strict --format="%ad" --name-only))

# ==================================================================================== #
# = Compiler settings
# ==================================================================================== #

CC        = g++
CPPFLAGS += -O3 -fno-tree-vectorize -std=c++11 \
			-DGIT_VERSION=\"$(GIT_VERSION)\" \
			-DGIT_COMMIT=\"$(GIT_COMMIT)\" \
			-DGIT_DATE=\"$(GIT_DATE)\" \

LIBS     += -lm -lcriterion

# ==================================================================================== #
# = Object / Assembly Files
# ==================================================================================== #

%.o : %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

%.s : %.c
	$(CC) $(CPPFLAGS) -S $< -o $@

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

%.s : %.cpp
	$(CC) $(CPPFLAGS) -S $< -o $@


# TODO: Does not work for some reason. Doesn't get included.

SRCS=$($(SRC_DIR)/wildcard *.c) $($(SRC_DIR)/wildcard *.cpp)


OBJS=$(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)
ASMS=$(C_SRCS:.c=.s) $(CPP_SRCS:.cpp=.s)

SRCS=$(addprefix $(SourceDir),$(Sources))
CObjects=$(addprefix $(ObjectDir),$(Objects))
CExecutable=$(addprefix $(BinDir),$(Executable))


# ==================================================================================== #
# = Targets
# ==================================================================================== #

all: test_integration test_objectives

# TODO: Super ugly but I was fighting hard with it idk why it does not include it using the CPP_SRCS...
test_integration: $(OBJS) $(ASMS) ../benchmark/utils.o
	$(CC) $(OBJS) ../benchmark/utils.o -o $(BIN_INTEGRATION_TEST) $(LIBS)

test_objectives: ../objectives/objectives.o
	$(CC) ../objectives/objectives.o -o $(BIN_OBJECTIVES_TEST) $(LIBS)

test:
	./$(BIN_OBJECTIVES_TEST)
	./$(BIN_INTEGRATION_TEST)

clean:
	rm -rf  $(OBJS) $(ASMS)
	rm -rf $(BIN_INTEGRATION_TEST) $(BIN_OBJECTIVES_TEST)