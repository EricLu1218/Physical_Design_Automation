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
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(EXEC) $(OBJS) $(DEPS)

ifeq (test, $(firstword $(MAKECMDGOALS)))
  TESTCASE := $(word 2, $(MAKECMDGOALS))
  $(eval $(TESTCASE):;@:)
endif

test: $(EXEC)
	@echo test on $(TESTCASE)
	./$(EXEC) ../testcase/$(TESTCASE).nets ../testcase/$(TESTCASE).cells ../output/$(TESTCASE).out
	../verifier/verify ../testcase/$(TESTCASE).nets ../testcase/$(TESTCASE).cells ../output/$(TESTCASE).out

.PHONY: all clean test
-include $(DEPS)
