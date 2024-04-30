
CC=gcc
CFLAGS=$(shell sdl2-config --cflags) -ggdb3 -O0 --std=c99 -Wall
LDFLAGS=$(shell sdl2-config --libs) -lSDL2_gfx -lSDL2_ttf -lm


SRCS=main.c heavenlyBodies.c graphics.c physics.c ui.c
HDRS=heavenlyBodies.h graphics.h physics.h ui.h
OBJS=$(SRCS:.c=.o)

TARGET=sim

all:	$(TARGET)
	@echo "Simple compiler named $(TARGET) has been compiled"

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

