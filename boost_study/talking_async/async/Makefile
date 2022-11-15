CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -fno-inline -I../asio-1.24.0/include -g -DASIO_ENABLE_HANDLER_TRACKING -DASIO_STANDALONE
SOURCE=$(wildcard *.cpp)
PROGRAMS=$(SOURCE:.cpp=)
DSYM=$(SOURCE:.cpp=.dSYM)

all: $(PROGRAMS)

clean:
	rm -rf $(PROGRAMS) $(DSYM)