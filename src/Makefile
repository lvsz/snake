OBJ     := main.o game.o window.o io.o snake.o score.o
CFLAGS  += $(shell sdl-config --cflags)
LDFLAGS += $(shell sdl-config --libs)
LDFLAGS += -lSDL_ttf

snake: $(OBJ)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

game.o: game.c game.h window.h io.h snake.h score.h defaults.h types.h
	$(CC) -c $<

window.o: window.c window.h defaults.h
	$(CC) -c $<

io.o: io.c io.h
	$(CC) -c $<

snake.o: snake.c snake.h types.h
	$(CC) -c $<

score.o: score.c score.h window.h io.h defaults.h types.h
	$(CC) -c $<

.PHONY: clean

clean:
	$(RM) $(OBJ)
