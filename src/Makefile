OBJ     := main.o game.o window.o sound.o io.o snake.o
CFLAGS  += $(shell sdl-config --cflags)
LDFLAGS += $(shell sdl-config --libs)
LDFLAGS += -lSDL_ttf
LDFLAGS += -lSDL_mixer

snake: $(OBJ)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

game.o: game.c game.h window.h io.h snake.h types.h defaults.h
	$(CC) -c $<

window.o: window.c window.h sound.o io.h types.h defaults.h
	$(CC) -c $<

sound.o: sound.c sound.h
	$(CC) -c $<

io.o: io.c io.h game.h window.h snake.h types.h defaults.h
	$(CC) -c $<

snake.o: snake.c snake.h game.h types.h defaults.h
	$(CC) -c $<

.PHONY: clean

clean:
	$(RM) $(OBJ)

