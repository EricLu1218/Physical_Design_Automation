CXX := g++
CXXFLAGS := -std=c++11 -O3 -Wall -Wextra
LIBS := -lm
EXEC := ../bin/hw4
SRC_DIRS := Legalizer\
			Parser\
			ResultWriter
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

ifeq ($(TESTCASE_NAME),adaptec1)
  AUX_FILENAME := adaptec1
else ifeq ($(TESTCASE_NAME),adaptec3)
  AUX_FILENAME := adaptec3
else ifeq ($(TESTCASE_NAME),ibm01)
  AUX_FILENAME := ibm01-cu85
else ifeq ($(TESTCASE_NAME),ibm07)
  AUX_FILENAME := ibm07-cu90
else ifeq ($(TESTCASE_NAME),ibm09)
  AUX_FILENAME := ibm09-cu90
else ifeq ($(TESTCASE_NAME),sample)
  AUX_FILENAME := sample
endif

test: $(EXEC)
	@echo test on $(TESTCASE_NAME)
	./$(EXEC) ../testcase/$(TESTCASE_NAME)/$(AUX_FILENAME).aux
	../verifier/verify ../testcase/$(TESTCASE_NAME)/$(AUX_FILENAME).aux ../output/$(AUX_FILENAME).result

.PHONY: all clean test
-include $(DEPS)
