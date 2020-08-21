# Maptool 제작

## TODO:
01. Windowsize Change ( 960 * 720 ) DONE:

02. create core engine (window create, renderer) DONE:

03. Change Maptool pallete position DONE:

04. change pallete size twice DONE:

05. change printing position of selected tile, size 1.5 zoom DONE:

06. WorldMap size change 8x8 to 16x16 DONE:

07. create func putTile click and drag DONE:

08. classificate worldmap section , pallte section DONE:

09. save map realization DONE:  
    + 맵 저장 구현하기전에 텍스트 입력 이벤트 발생시 처리 DONE:
    + 입력상태 전환하는 상태기계 생성 (nInputFSM) Enterkey로 전환 DONE:
    + 입력 텍스트 저장하는 버퍼 생성 (szBuf) DONE:
    + 저장된 입력버퍼를 parsing하는 함수 tile.c에 생성 DONE:

10. load map realization DONE:

11. Create MapLayer WORKNOW:

11. 팔레트 좌단, 상단에 보이지 않는 인덱스 존재 FIXME
    + 원인 : 마우스 클릭 이벤트에서 팔레트 처리를 할 때 (현재 마우스의 위치 - 팔레트의 시작위치)/ 타일사이즈 로 나누었기 떄문에 -31~31/32 일 경우 x,y 인덱스의 값이 0이 된다. --> 나중에 PointInRect 함수로 처리하면 해결됌