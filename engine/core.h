#ifndef __CORE_H__
#define __CORE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>       // SDL헤더
#include <SDL2/SDL_image.h> // 이미지 관련헤더
#include <SDL2/SDL_ttf.h>   // 문자출력 관련헤더
#include <SDL2/SDL_mixer.h> // 사운드 관련헤더

typedef struct __core
{
    SDL_Window *m_pWin;
    SDL_Renderer *m_pRender;
    TTF_Font *m_pFont;

    Uint16 m_ScreenWidth;
    Uint16 m_ScreenHeight;
} tDE_S_Core ; // 엔진 구동을 위한 코어 구조체

tDE_S_Core *tDE_setup1(const char *szTitle, int window_width, int window_height, Uint32 flags);


#endif