# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRCS = display.c find.c hop.c reveal.c proclore.c find.c input.c log.c main.c 

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = shell

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything
rebuild: clean all