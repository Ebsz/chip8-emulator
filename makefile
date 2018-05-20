CC = gcc

# compiler flags
CCFLAGS = 
#CCFLAGS += -O3 # Optimize for speed

OBJS = src/*.c
INC = -I include

LIB = -lSDL2

TARGET = c8emu

all: exec

test: CCFLAGS += -Wall -Wextra -pedantic
test: exec

debug: CCFLAGS += -g
debug: exec

exec:
	$(CC) $(CCFLAGS) $(OBJS) $(INC) $(LIB) -o $(TARGET)
