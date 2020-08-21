#include "engine/core.h"
#include "engine/tile.h"
tDE_S_Core *g_pEngineCore; // 엔진코어
SDL_Texture *g_pTileSet;

Uint16 g_worldMap_Layer_1[64]; // 월드맵 크기
Uint16 g_nSelectTileIndex = 0; // 선택한 타일번호 

int main(int argc, char *argv[])
{
    for (int i = 0; i < 64; i++) // 월드맵 초기화
    {
        g_worldMap_Layer_1[i] = -1;
    }

    g_pEngineCore = tDE_setup1("Maptool", 960, 720, 0); // window setup
    

    
    { // 이미지소스 surface에 긁어온거 rendering 하기
        SDL_Surface *pSurface;
        pSurface = IMG_Load("res/basictiles.png"); // size 128 * 240  tiles 8 * 15  tile size 16 * 16 
        SDL_Texture *pTexture = SDL_CreateTextureFromSurface(g_pEngineCore->m_pRender, pSurface);
        g_pTileSet = pTexture;
        SDL_FreeSurface(pSurface);
    }

    SDL_bool bLoop = SDL_TRUE;
    while (bLoop)
    {
        SDL_SetRenderDrawColor(g_pEngineCore->m_pRender, 0, 0, 0, 1);
        SDL_RenderClear(g_pEngineCore->m_pRender);

        // 선택된 타일 이미지 팔레트 위에 출력 x 좌표 : 16 x 32 , y 좌표 : 1 * 32  , g_nSelectTileIndex를 rendercopy해서 나타내준다.
        putTile(g_pEngineCore->m_pRender, g_pTileSet, 16, 1, g_nSelectTileIndex);

        // 704, 100 위치에 팔레트를 렌더링 한다.
        {
            // 팔레트 렌더링 48 * 4 의 넓이와 72 * 4의 높이로 렌더링
            SDL_Rect dstRect = {704, 100, 128, 240 };
            SDL_RenderCopy(g_pEngineCore->m_pRender, g_pTileSet, NULL, &dstRect);
        }

        // render world map
        {
            for (int i = 0; i < 64; i++)
            {
                int _index = g_worldMap_Layer_1[i];
                if (_index != -1)
                {
                    putTile(g_pEngineCore->m_pRender, g_pTileSet, i % 8, i / 8, _index);
                }
            }
        }

        SDL_RenderPresent(g_pEngineCore->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            switch (_event.type)
            {
            case SDL_MOUSEMOTION:
            {
                printf("%4d%4d\r", _event.motion.x, _event.motion.y); // 마우스 좌표 표시
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                printf("%8d\r", _event.button.button);
                if (_event.button.button == 1) // left click
                {
                    
                    // 팔레트 처리
                    {                                          
                        int _x = (_event.motion.x - 704) / 16; // 팔레트의 x 인덱스  (704,100 의 위치에 팔레트가 존재) (타일의 사이즈인 16크기로 나눠줌)
                        int _y = (_event.motion.y - 100) / 16; // 팔레트의 y 인덱스

                        if ((_x >= 0 && _y >= 0) && (_x < 8 && _y < 15))
                        {
                            g_nSelectTileIndex = _y * 8 + _x;
                        }

                        printf("%4d%4d\r", _x, _y); // 선택된 타일의 x,y 좌표 출력
                    }
                    //월드맵처리
                    {
                        int _x = (_event.motion.x) / 32;
                        int _y = (_event.motion.y) / 32;

                        if (_x < 8 && _y < 15)
                        {
                            int _tileIndex = _y * 8 + _x;
                            //printf("%8d \r",_tileIndex);
                            g_worldMap_Layer_1[_tileIndex] = g_nSelectTileIndex;
                            printf("%4d%4d%4d\r", _x, _y, _tileIndex);
                        }
                    }
                }
                else if (_event.button.button == 3) // right click
                {
                    {
                        int _x =(_event.motion.x) / 32;
                        int _y = (_event.motion.y) / 32;

                        if(_x < 8 && _y<15)
                        {
                            int _tileIndex = _y * 8 + _x;
                            g_worldMap_Layer_1[_tileIndex] = -1;
                        }
                    }
                }
            }
            break;
            case SDL_KEYDOWN:
                printf("%d \n", _event.key.keysym.scancode);
                break;
            case SDL_QUIT:
                bLoop = SDL_FALSE;
                break;
            default:
                break;
            }
        }
    }
    
    SDL_DestroyRenderer(g_pEngineCore->m_pRender);
    SDL_DestroyWindow(g_pEngineCore->m_pWin);
    SDL_Quit();
    return 0;
}