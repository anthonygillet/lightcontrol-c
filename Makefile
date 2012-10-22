CC=gcc
CFLAGS=-c -Wall
SRCDIR=src
OBJDIR=bin
EXECUTABLE=insteon
_SOURCES=main.c rs232.c printer.c insteon.c
SOURCES = $(patsubst %,$(SRCDIR)/%,$(_SOURCES))
_OBJECTS=$(_SOURCES:.c=.o)
OBJECTS = $(patsubst %,$(OBJDIR)/%,$(_OBJECTS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)
