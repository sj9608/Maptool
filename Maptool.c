#include "engine/core.h"
#include "engine/tile.h"
#include "engine/cmd_parser.h"

tDE_S_Core *g_pEngineCore; // 엔진코어
SDL_Texture *g_pTileSet;

Uint16 g_worldMap_Layer_1[256]; // 월드맵 크기
Uint16 g_nSelectTileIndex = 0;  // 선택한 타일번호

SDL_Rect g_rtWorldMap = {0, 0, 512, 512};  // 월드맵 영역 구분하는 사각형
SDL_Rect g_rtPallete = {704, 0, 256, 620}; // 팔레트 영역 구분 사각형

int main(int argc, char *argv[])
{
    for (int i = 0; i < 256; i++) // 월드맵 초기화
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
        PrintSelectedTile(g_pEngineCore->m_pRender, g_pTileSet, 25, 1, g_nSelectTileIndex);

        { // 팔레트 영역, 팔레트타일 렌더링
            SDL_SetRenderDrawColor(g_pEngineCore->m_pRender, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderDrawRect(g_pEngineCore->m_pRender, &g_rtPallete);

            // 704, 100 위치에 팔레트를 렌더링 한다.
            // 팔레트 렌더링 128 *2 의 넓이와 240 * 2의 높이로 렌더링
            SDL_Rect dstRect = {704, 100, 128 * 2, 240 * 2};
            SDL_RenderCopy(g_pEngineCore->m_pRender, g_pTileSet, NULL, &dstRect);
        }

        // 월드맵 영역, 월드맵타일 렌더링
        {
            SDL_SetRenderDrawColor(g_pEngineCore->m_pRender, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderDrawRect(g_pEngineCore->m_pRender, &g_rtWorldMap);

            for (int i = 0; i < 256; i++)
            {
                int _index = g_worldMap_Layer_1[i];
                if (_index != -1)
                {
                    putTile(g_pEngineCore->m_pRender, g_pTileSet, i % 16, i / 16, _index); // world map size 16 x 16
                }
            }
        }

        SDL_RenderPresent(g_pEngineCore->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            switch (_event.type)
            {
                static Uint16 nInputFSM = 0;
                static char szInputBuf[32];

            case SDL_MOUSEMOTION: // 마우스 모션
            {
                SDL_Point _pt = {_event.motion.x, _event.motion.y}; // 현재 마우스 좌표
                printf("%4d%4d\r", _pt.x, _pt.y);                   // 마우스 좌표 표시

                if (_event.button.button == 1) // 좌클릭한 상태로 모션
                {

                    if (SDL_PointInRect(&_pt, &g_rtWorldMap)) // 현재 마우스 포인터가 월드맵 영역 안에 있다.
                    {
                        int _x = (_event.motion.x) / 32; // 월드맵이 0,0 에서 시작함 x,y 에서 시작할경우 현재값(_event.motion.x,y) 에서 x,y값 뺴주면 됌. 타일사이즈 32에 한칸
                        int _y = (_event.motion.y) / 32;

                        int _tileIndex = _y * 16 + _x;

                        g_worldMap_Layer_1[_tileIndex] = g_nSelectTileIndex;
                        printf("%4d%4d%4d\r", _x, _y, _tileIndex);
                    }
                }
            }
            break;

            case SDL_MOUSEBUTTONDOWN: //마우스 클릭
            {
                printf("%8d\r", _event.button.button);
                if (_event.button.button == 1) // left click
                {

                    // 팔레트 처리
                    {
                        int _x = (_event.motion.x - 704) / 32; // 팔레트의 x 인덱스  (704,100 의 위치에 팔레트가 존재) (타일의 사이즈인 32크기로 나눠줌)
                        int _y = (_event.motion.y - 100) / 32; // 팔레트의 y 인덱스

                        if ((_x >= 0 && _y >= 0) && (_x < 8 && _y < 15)) //
                        {
                            g_nSelectTileIndex = _y * 8 + _x; //  팔레트의 2차원 배열 1차원으로 변환해서 현재 선택된 타일 인덱스값에 넣어줌.
                        }

                        printf("%4d%4d\r", _x, _y); // 선택된 타일의 x,y 좌표 출력
                    }
                    //월드맵처리
                    {
                        int _x = (_event.motion.x) / 32; // 월드맵이 0,0 에서 시작함 x,y 에서 시작할경우 현재값(_event.motion.x,y) 에서 x,y값 뺴주면 됌. 타일사이즈 32에 한칸
                        int _y = (_event.motion.y) / 32;

                        if (_x < 15 && _y < 15) // _x,_y 인덱스가 15 이내 일 경우만 월드맵으로 인식하고 _tileIndex에 2차원 배열 1차원으로 변환해서 넣어줌
                        {
                            int _tileIndex = _y * 16 + _x;

                            g_worldMap_Layer_1[_tileIndex] = g_nSelectTileIndex;
                            printf("%4d%4d%4d\r", _x, _y, _tileIndex);
                        }
                    }
                }
                else if (_event.button.button == 3) // right click
                {
                    {
                        int _x = (_event.motion.x) / 32;
                        int _y = (_event.motion.y) / 32;

                        if (_x < 8 && _y < 15)
                        {
                            int _tileIndex = _y * 16 + _x;
                            g_worldMap_Layer_1[_tileIndex] = -1; // 지우개
                        }
                    }
                }
            }
            break;

            case SDL_KEYDOWN: // 키보드 클릭
            {
                switch (nInputFSM)
                {
                case 0: // 대기상태
                {
                    if (_event.key.keysym.sym == SDLK_RETURN)
                    {
                        printf("input Command \n");
                        nInputFSM = 1; // 입력상태로 전이
                    }
                    break;
                }

                case 1:
                {
                    if (_event.key.keysym.sym == SDLK_RETURN)
                    {
                        nInputFSM = 0; // 대기상태로 전이
                        parseCmd(szInputBuf);
                        szInputBuf[0] = 0x00;                        
                    }
                    
                    else if(_event.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        int _len = strlen(szInputBuf);
                        szInputBuf[_len-1] = 0;
                        printf("%s    \r",szInputBuf);
                    }
                }
                break;
                }
            }
            break;

            case SDL_TEXTINPUT: // 텍스트 입력
            {
                if (nInputFSM == 1)
                {
                    strcat(szInputBuf, _event.text.text); // 현재 이벤트에 입력되는 텍스트 szInputBuf에 카피
                    printf("%s    \r",szInputBuf); // 텍스트 입력이 있을 때 마다 입력된 값을 cmd  창에 띄워줌
                }
            }
            break;

            case SDL_USEREVENT: // 넘겨받은 유저이벤트
            {
                if (strcmp(_event.user.data1, "save") == 0)
                {
                    char *pFileName = ((char *)_event.user.data1 + 16);
                    saveMap(pFileName, g_worldMap_Layer_1);
                }
                // else if 선언하고 user.data1 넘겨 받은거 load 일 경우 함수 짜기
            }
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