# ==================================================================================== #
# = Executable
# ==================================================================================== #

BIN = benchmark
BIN_UTILS_TEST = utils_test

# ==================================================================================== #
# = GIT RECORD
# ==================================================================================== #

GIT_VERSION := $(shell git --no-pager describe --tags --always)
GIT_COMMIT  := $(shell git rev-parse --verify HEAD)
GIT_DATE    := $(firstword $(shell git --no-pager show --date=iso-strict --format="%ad" --name-only))

# ==================================================================================== #
# = Compiler settings
# ==================================================================================== #

CC      = g++
CPPFLAGS += -O3 -fno-tree-vectorize -std=c++11 \
			-DGIT_VERSION=\"$(GIT_VERSION)\" \
			-DGIT_COMMIT=\"$(GIT_COMMIT)\" \
			-DGIT_DATE=\"$(GIT_DATE)\" \

LIBS   += -lm -lcriterion

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

C_SRCS=$(wildcard *.c \
../HGWOSCA/main.c \
../objectives/objectives.c \
)

CPP_SRCS=$(wildcard *.cpp)

OBJS=$(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)

ASMS=$(C_SRCS:.c=.s) $(CPP_SRCS:.cpp=.s)


# ==================================================================================== #
# = Targets
# ==================================================================================== #


all: $(OBJS) $(ASMS)
	$(CC) $(OBJS) -o $(BIN) $(LIBS)

test: utils.o
	$(CC) utils.o -o $(BIN_UTILS_TEST) $(LIBS)
	./$(BIN_UTILS_TEST)


clean:
	rm -rf  $(OBJS) $(ASMS)
	rm -rf $(BIN) $(BIN_UTILS_TEST)
	rm -rf *.csv
