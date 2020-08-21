#include "../engine/core.h"

#define MAX_TOKEN_SIZE 32
int doTokenize(char *szBuf, char (*szBufToken)[MAX_TOKEN_SIZE]) // 인자로 char *형 버퍼 , 2차원 버퍼 
{

  char *szTemp;                             // char형 포인터 szTemp 생성
  const char *pszDelimiter = " ";           // 구분자로 " " 사용
  szTemp = strtok(szBuf, pszDelimiter);     // " " 로 입력버퍼 구분해서 szTemp에 저장

  int _nTokenIndex = 0;                     // 토큰 카운트 + 토큰에 저장할 인덱스값 증가
  while (szTemp != NULL)    // szTemp 버퍼의 끝까지 읽음
  {
    strcpy(szBufToken[_nTokenIndex], szTemp);    // szBufToken의 인덱스에 szTemp에 저장된값 저장
    _nTokenIndex++;
    szTemp = strtok(NULL, pszDelimiter);    // " " 앞까지 끊어서 버리고 다음 인덱스에 szTemp값 넣기위해 strtok사용
  }
  return _nTokenIndex; // 오류 검출을 위해 토크나이즈 인덱스를 반환해서 토큰 카운트
}

void parseCmd(char *_szCmd)
{
  static char szCmd[64];        // 입력버퍼 받은거 저장하기위한 배열
  static char szTokens[8][32];  // 토큰화 시켜서 넣을 배열

  strcpy(szCmd, _szCmd);    // 입력받은버퍼 복사

  printf("token count %s\n", szCmd); // 입력받은 버퍼가 잘 복사되었나 확인 ( 오류검출 )
  int _numToken = doTokenize(szCmd, szTokens); // tokenize시 오류 검출을 위해 token count를 해보는게 정석
  printf("token count %d\n", _numToken);
  
  if (strcmp(szTokens[0], "save") == 0) // 첫 토큰이 save 일 경우
  {
    
    char *pFileName = szTokens[1]; // 두번째 토큰에 들어온 값을 파일 이름변수로 저장
    

    static char pMsg[32];           // 이벤트 값으로 넘겨줄 pMsg 문자열배열 선언
    strcpy(pMsg, "save");           // pMsg에 "save" 값 넘겨주고 
    strcpy(pMsg + 16, pFileName);   // 뒤 16바이트에는 파일 이름으로한다.
    SDL_Event evt;                  // main에 넘겨줄 evt 객체선언
    evt.type = SDL_USEREVENT;       // 이벤트 타입은 USEREVENT
    evt.user.data1 = pMsg;          // user가 넘겨줄 data1 은 pMsg에 저장된 값
    evt.user.timestamp = SDL_GetTicks();    // 잘 모르겠다
    SDL_PushEvent(&evt);            // 이벤트 푸쉬해서 메인으로 넘겨줘야함 (중요)
  }

  else if(strcmp(szTokens[0], "load") == 0)
  {
      char *pFileName = szTokens[1];

      static char pMsg[32];
      strcpy(pMsg, "load");
      strcpy(pMsg + 16, pFileName);
      SDL_Event evt;
      evt.type = SDL_USEREVENT;
      evt.user.data1 = pMsg;
      evt.user.timestamp = SDL_GetTicks();
      SDL_PushEvent(&evt);
  }
 
}