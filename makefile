CC = gcc
CFLAGS = -I include
LIB = -lSDL2
VPATH = build src

SRCDIR = src
OUTDIR = build
TARGET = c8emu


SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.o, $(SRC))

.PHONY: all clean

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LIB)

$(OBJ) : $(OUTDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@-rm -rf $(TARGET)
	@-rm -rf $(OUTDIR)

print-% : ; @echo $* = $($*)
