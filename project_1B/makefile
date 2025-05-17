# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -Wall -D_GNU_SOURCE
LDFLAGS = -lrt

# Source files
SRCS = *.c

# Header files
HEADERS = display.h find.h hop.h reveal.h proclore.h input.h log.h background.h man.h ping.h pipes.h redirection.h signals.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = shell

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to build object files with header dependencies
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the shell after building
run: $(TARGET)
	./$(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild run
