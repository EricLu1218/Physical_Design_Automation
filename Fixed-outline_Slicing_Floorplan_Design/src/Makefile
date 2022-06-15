CXX := g++
CXXFLAGS := -std=c++11 -O3 -Wall -Wextra
LIBS := -lm
EXEC := ../bin/hw3
SRC_DIRS := Parser\
			ResultWriter\
			Solver\
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
  TESTCASE_NAME := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(TESTCASE_NAME):;@:)
endif

test: $(EXEC)
	@echo test on $(TESTCASE_NAME) 0.2
	./$(EXEC) ../testcase/$(TESTCASE_NAME).hardblocks ../testcase/$(TESTCASE_NAME).nets ../testcase/$(TESTCASE_NAME).pl ../output/$(TESTCASE_NAME).floorplan 0.2
	../verifier/verifier ../testcase/$(TESTCASE_NAME).hardblocks ../testcase/$(TESTCASE_NAME).nets ../testcase/$(TESTCASE_NAME).pl ../output/$(TESTCASE_NAME).floorplan 0.2
	@echo test on $(TESTCASE_NAME) 0.15
	./$(EXEC) ../testcase/$(TESTCASE_NAME).hardblocks ../testcase/$(TESTCASE_NAME).nets ../testcase/$(TESTCASE_NAME).pl ../output/$(TESTCASE_NAME).floorplan 0.15
	../verifier/verifier ../testcase/$(TESTCASE_NAME).hardblocks ../testcase/$(TESTCASE_NAME).nets ../testcase/$(TESTCASE_NAME).pl ../output/$(TESTCASE_NAME).floorplan 0.15

.PHONY: all clean test
-include $(DEPS)
