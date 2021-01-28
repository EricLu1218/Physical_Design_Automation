CXX := g++
CXXFLAGS := -std=c++11 -O3 -Wall -Wextra
LIBS := -lm
EXEC := ../bin/FM_Partitioner
SRC_DIRS := Parser\
			Partitioner\
			ResultWriter\
			Structure
SRCS := main.cpp $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	rm -rf $(EXEC) $(OBJS) $(DEPS)

ifeq (test,$(firstword $(MAKECMDGOALS)))
  TEST_CASE_ID := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(TEST_CASE_ID):;@:)
endif

test: $(EXEC)
	@echo test on p2-$(TEST_CASE_ID)
	./$(EXEC) ../testcases/p2-$(TEST_CASE_ID).nets ../testcases/p2-$(TEST_CASE_ID).cells ../output/p2-$(TEST_CASE_ID).out
	../verifier/verify ../testcases/p2-$(TEST_CASE_ID).nets ../testcases/p2-$(TEST_CASE_ID).cells ../output/p2-$(TEST_CASE_ID).out

.PHONY: all clean test
-include $(DEPS)
