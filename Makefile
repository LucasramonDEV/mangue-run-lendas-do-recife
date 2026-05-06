CC=gcc
CFLAGS=-Iinclude
LIBS=-lraylib -lopengl32 -lgdi32 -lwinmm

SRC=src/main.c src/game.c src/player.c src/map.c src/enemy.c src/projectile.c src/item.c

all:
	$(CC) $(SRC) $(CFLAGS) -o jogo.exe $(LIBS)

run:
	./jogo.exe

clean:
	del jogo.exe