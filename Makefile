SDLCFLAGS = -I/usr/include/SDL2 -D_REENTRANT
SDLLDFLAGS = -L/usr/lib/x86_64-linux-gnu -lSDL2

CC = gcc
CFLAGS = -Wall -O2 $(SDLCFLAGS)
LDFLAGS = -lm $(SDLLDFLAGS)
OBJDIR = obj
TARGET = $(OBJDIR)/sdl2base

OBJS =	$(OBJDIR)/sdl2base.o

all: $(TARGET)

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

########################################################################

$(OBJDIR)/sdl2base.o: sdl2base.c
	$(CC) -c $(CFLAGS) $? -o $@
