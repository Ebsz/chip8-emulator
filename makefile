CC = gcc
CFLAGS = -I include -Wall -Werror
LIB = -lSDL2 -lncurses
VPATH = build src

SRCDIR = src
OUTDIR = build
TARGET = c8emu


SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.o, $(SRC))

.PHONY: all clean

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LIB)

debug: CFLAGS += -g -DDEBUG
debug: clean all

$(OBJ) : $(OUTDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@-rm -rf $(TARGET)
	@-rm -rf $(OUTDIR)

print-% : ; @echo $* = $($*)
