#ifndef __TILE_H
#define __TILE_H
void putTile(SDL_Renderer *pRenderer, SDL_Texture *pTex,Uint16 _x,Uint16 _y, Uint16 _index);
void PrintSelectedTile(SDL_Renderer *pRenderer, SDL_Texture *pTex, Uint16 _x, Uint16 _y, Uint16 _index);
SDL_bool saveMap(const char *filename, Sint16 *map);
SDL_bool loadMap(const char *filename, Sint16 *map);
#endif