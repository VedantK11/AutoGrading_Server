CC = g++
CFLAGS = -pthread
SRCS = server.cpp server_main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = server

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
