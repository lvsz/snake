OBJ     := main.o window.o input.o snake.o game.o queue.o
CFLAGS  += $(shell sdl-config --cflags)
LDFLAGS += $(shell sdl-config --libs)
LDFLAGS += -lSDL_ttf

snake: $(OBJ)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

window.o: window.c window.h defaults.h
	$(CC) -c $<

input.o: input.c input.h
	$(CC) -c $<

snake.o: snake.c snake.h queue.h types.h
	$(CC) -c $<

game.o: game.c game.h window.h input.h snake.h  defaults.h types.h
	$(CC) -c $<

queue.o: queue.c queue.h
	$(CC) -c $<

.PHONY: clean

clean:
	$(RM) $(OBJ)
