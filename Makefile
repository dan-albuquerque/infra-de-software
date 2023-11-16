CC = gcc
CFLAGS = -Wall -pthread

SRCS = banker.c
OBJS = $(SRCS:.c=.o)

TARGET = banker

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)