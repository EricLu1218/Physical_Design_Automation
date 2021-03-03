CXX := g++
CXXFLAGS := -std=c++11 -O3 -Wall -Wextra
LIBS := -lm
EXEC := ../bin/hw5
SRC_DIRS := Parser\
			ResultWriter\
			Router\
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
	@echo test on $(TESTCASE_NAME)
	./$(EXEC) ../testcase/$(TESTCASE_NAME).modified.txt ../output/$(TESTCASE_NAME).result
	../verifier/verify ../testcase/$(TESTCASE_NAME).modified.txt ../output/$(TESTCASE_NAME).result

.PHONY: all clean test
-include $(DEPS)
