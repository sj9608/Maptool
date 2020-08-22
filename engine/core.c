#include "core.h"
#include <time.h>

// Window setup
tDE_S_Core *tDE_setup1(const char *szTitle, int window_width, int window_height, Uint32 flags)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL : %s\n", SDL_GetError());
        return NULL;
    }

    tDE_S_Core *pCore = SDL_malloc(sizeof(tDE_S_Core));

    pCore->m_pWin = SDL_CreateWindow(szTitle,
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     window_width,
                                     window_height,
                                     flags);

    pCore->m_ScreenWidth = window_width;
    pCore->m_ScreenHeight = window_height;


    pCore->m_pRender = SDL_CreateRenderer(pCore->m_pWin, -1 , SDL_RENDERER_ACCELERATED); //create renderer
    if(!pCore->m_pRender)
    {
        printf("redner init Error : %s\n",SDL_GetError());
        return NULL;
    }


    if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) // image Error
    {
        printf("sdl_image dll init error : %s\n",SDL_GetError());
        return NULL;
    }

    if(TTF_Init() == -1)
    {
        printf("sdl ttf dll inti Error : %s\n",SDL_GetError());
        return NULL;
    }

    pCore->m_pFont = TTF_OpenFont("res/nmf.ttf", 24); // Set Default Font

    if(!pCore->m_pFont)
    {
        printf("load default font error : %s \n",SDL_GetError());
        return NULL;
    }

    printf("init ok\n");
    printf("engine start\n");

    return pCore;

}

void tDE_closeCore(tDE_S_Core *pCore)
{
    TTF_CloseFont(pCore->m_pFont);
    SDL_DestroyRenderer(pCore->m_pRender);
    SDL_DestroyWindow(pCore->m_pWin);
}

SDL_Texture *tDE_loadTexture(tDE_S_Core *pCore, const char *filename)
{
    SDL_Surface *surface;
    surface = IMG_Load(filename);

    if(surface == NULL)
    {
        printf("file load error : %s\n", SDL_GetError());
        return NULL;
    }
    printf("IMG load Success\n");

    SDL_Texture *pTex = SDL_CreateTextureFromSurface(pCore->m_pRender, surface);
    SDL_FreeSurface(surface);
    
    return pTex;
}