#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// 던전 타일용으로 소스가 8x8 만큼 긁어오는거..  --> fix basic tile 용 16 16 긁어오기
// pRenderer, texture ,
void putTile(SDL_Renderer *pRenderer, SDL_Texture *pTex, Uint16 _x, Uint16 _y, Uint16 _index)
{
  SDL_Rect _tmpSrcRt;              // 원본 텍스쳐에서 긁어올 좌표 x,y  긁어올 크기 w,h
  _tmpSrcRt.x = (_index % 8) * 16; // 맵 소스를 받아올 x 의 위치 (인덱스 값으로 계산할거)
  _tmpSrcRt.y = (_index / 8) * 16; // 맵 소스를 받아올y 의 위치  x가 8이 넘어갈때마다 y 값이 증가해야함.
  _tmpSrcRt.w = 16;
  _tmpSrcRt.h = 16;

  SDL_Rect _tmpDstRt; //
  _tmpDstRt.x = _x * 32;
  _tmpDstRt.y = _y * 32;
  _tmpDstRt.w = 16 * 2;
  _tmpDstRt.h = 16 * 2;

  SDL_RenderCopy(pRenderer, pTex, &_tmpSrcRt, &_tmpDstRt);
}

// 선택된 타일 1.5배로 출력
void PrintSelectedTile(SDL_Renderer *pRenderer, SDL_Texture *pTex, Uint16 _x, Uint16 _y, Uint16 _index)
{
  SDL_Rect _tmpSrcRt;              // 원본 텍스쳐에서 긁어올 좌표 x,y  긁어올 크기 w,h
  _tmpSrcRt.x = (_index % 8) * 16; // 맵 소스를 받아올 x 의 위치 (인덱스 값으로 계산할거)
  _tmpSrcRt.y = (_index / 8) * 16; // 맵 소스를 받아올y 의 위치  x가 8이 넘어갈때마다 y 값이 증가해야함.
  _tmpSrcRt.w = 16;
  _tmpSrcRt.h = 16;

  SDL_Rect _tmpDstRt; //
  _tmpDstRt.x = _x * 32;
  _tmpDstRt.y = _y * 32;
  _tmpDstRt.w = 16 * 3; // 2배크기로 출력
  _tmpDstRt.h = 16 * 3;

  SDL_RenderCopy(pRenderer, pTex, &_tmpSrcRt, &_tmpDstRt);
}

// savemap
SDL_bool saveMap(const char *filename, Sint16 *map)
{
  SDL_RWops *rw = SDL_RWFromFile(filename, "wb"); //Read Write operation structure / SDL_RWFromFile = fread
  if (!rw)
    return SDL_FALSE; // 파일 쓰기 실패시 FALSE 반환 하고 마침

  SDL_RWwrite(rw, map, sizeof(Uint16), 256); // map은 16 * 16 = 256 byte만큼 차지함
  SDL_RWclose(rw);
  return SDL_TRUE;
}