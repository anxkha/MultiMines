#ifndef _GAME_H
#define _GAME_H

#include <d3d9.h>

struct SQUARE
{
	DWORD	IsBomb;
	DWORD	NumAdjBombs;
	DWORD	IsFlagged;
	DWORD	IsUnsure;
	DWORD	IsUncovered;
};

VOID InitializeGame( DWORD nWidth, DWORD nHeight, DWORD nNumBombs );

SQUARE* GetBoard();
int GetBoardWidth();
int GetBoardHeight();
int GameMove( int x, int y, int Type );

#endif // _GAME_H
