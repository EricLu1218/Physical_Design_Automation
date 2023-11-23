CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -MMD
LIBS     := -lm
EXEC     := ../bin/hw3
SRC_DIRS := .\
			Parser\
			ResultWriter\
			Solver\
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
  RATIO    := $(word 3, $(MAKECMDGOALS))
  $(eval $(TESTCASE):;@:)
  $(eval $(RATIO):;@:)
endif

test: $(EXEC)
	@echo test on $(TESTCASE) $(RATIO)
	./$(EXEC) ../testcase/$(TESTCASE).hardblocks ../testcase/$(TESTCASE).nets ../testcase/$(TESTCASE).pl ../output/$(TESTCASE).floorplan $(RATIO)
	../verifier/verifier ../testcase/$(TESTCASE).hardblocks ../testcase/$(TESTCASE).nets ../testcase/$(TESTCASE).pl ../output/$(TESTCASE).floorplan $(RATIO)

.PHONY: all clean test
-include $(DEPS)
