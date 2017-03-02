OBJ     := main.o game.o window.o sound.o io.o snake.o
CFLAGS  += $(shell sdl-config --cflags)
LDFLAGS += $(shell sdl-config --libs)
LDFLAGS += -lSDL_ttf
LDFLAGS += -lSDL_mixer

snake: $(OBJ)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

main.o: src/main.c src/game.h src/window.h
	$(CC) -c $<

game.o: src/game.c src/game.h src/window.h src/io.h src/snake.h src/types.h src/defaults.h
	$(CC) -c $<

window.o: src/window.c src/window.h src/sound.o src/io.h src/types.h src/defaults.h
	$(CC) -c $<

sound.o: src/sound.c src/sound.h
	$(CC) -c $<

io.o: src/io.c src/io.h src/game.h src/window.h src/snake.h src/types.h src/defaults.h
	$(CC) -c $<

snake.o: src/snake.c src/snake.h src/game.h src/types.h src/defaults.h
	$(CC) -c $<

.PHONY: clean

clean:
	$(RM) $(OBJ)

