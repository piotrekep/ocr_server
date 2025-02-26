CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -I/usr/local/include/opencv5
LDFLAGS  = -lboost_system -lpthread -L/usr/local/lib -Wl,-rpath,/usr/local/lib

SRCS     = main.cpp src/http_server.cpp src/datastore.cpp
OBJS     = $(SRCS:.cpp=.o)
TARGET   = http_server

# List of OpenCV libraries to link against
OPENCV_LIBS = \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
	-lopencv_imgcodecs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(OPENCV_LIBS)
	chmod +x $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)