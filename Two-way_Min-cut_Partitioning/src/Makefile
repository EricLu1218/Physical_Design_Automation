CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -MMD
LIBS     := -lm
EXEC     := ../bin/FM_Partitioner
SRC_DIRS := .\
			Parser\
			Partitioner\
			ResultWriter\
			Structure
SRCS     := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS     := $(SRCS:.cpp=.o)
DEPS     := $(OBJS:.o=.d)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(EXEC) $(OBJS) $(DEPS)

ifeq (test,$(firstword $(MAKECMDGOALS)))
  TESTCASE_NAME := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(TESTCASE_NAME):;@:)
endif

test: $(EXEC)
	@echo test on $(TESTCASE_NAME)
	./$(EXEC) ../testcases/$(TESTCASE_NAME).nets ../testcases/$(TESTCASE_NAME).cells ../output/$(TESTCASE_NAME).out
	../verifier/verify ../testcases/$(TESTCASE_NAME).nets ../testcases/$(TESTCASE_NAME).cells ../output/$(TESTCASE_NAME).out

.PHONY: all clean test
-include $(DEPS)
