CXX = g++
CXXFLAGS = -g -std=c++14 -Wall -Wextra -pedantic -I../source -I../external

PRGM  = test
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(PRGM)
	./test

$(PRGM): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(OBJS) $(DEPS) $(PRGM)

-include $(DEPS)
