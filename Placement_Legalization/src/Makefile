CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -MMD
LIBS     := -lm
EXEC     := ../bin/hw4
SRC_DIRS := .\
			Legalizer\
			Parser\
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

ifeq (test, $(firstword $(MAKECMDGOALS)))
  TESTCASE        := $(word 2, $(MAKECMDGOALS))
  AUX_FILEPATH    := $(wildcard ../testcase/$(TESTCASE)/*.aux)
  OUTPUT_FILEPATH := ../output/$(basename $(notdir $(AUX_FILEPATH))).result
  $(eval $(TESTCASE):;@:)
  $(eval $(AUX_FILEPATH):;@:)
  $(eval $(OUTPUT_FILEPATH):;@:)
endif

test: $(EXEC)
	@echo test on $(TESTCASE)
	./$(EXEC) $(AUX_FILEPATH)
	../verifier/verify $(AUX_FILEPATH) $(OUTPUT_FILEPATH)

.PHONY: all clean test
-include $(DEPS)
