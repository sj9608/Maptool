maptool : Maptool.c engine/tile.c engine/core.c engine/cmd_parser.c
	gcc -o $@ $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g
maptool1 : Maptool_copy.c engine/tile.c engine/core.c
	gcc -o $@ $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g