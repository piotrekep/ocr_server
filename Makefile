CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS  = -lboost_system -lpthread

SRCS     = main.cpp src/http_server.cpp src/datastore.cpp
OBJS     = $(SRCS:.cpp=.o)
TARGET   = http_server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	chmod +x $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)