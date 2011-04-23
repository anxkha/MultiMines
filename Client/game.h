// ----------------------------------------------------------------------------
//  Filename: game.h
//  Author: Lucas Suggs
//
//  Date Created: 11 February 2008
//  Date Modified: 14 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------
#ifndef _GAME_H
#define _GAME_H

#include <d3dx9.h>
#include <tchar.h>
#include "graphics.h"
#include "input.h"
#include "text.h"

struct SQUARE
{
	DWORD	IsBomb;
	DWORD	NumAdjBombs;
	DWORD	IsFlagged;
	DWORD	IsUnsure;
	DWORD	IsUncovered;
};

enum GameState 
{
	GAME_STATE_MAINMENU = 0,
	TitleScreen
};

class CGame
{
protected:
	IDirect3DDevice9*	m_pDevice;

	IDirect3DTexture9*	m_pBlank;
	IDirect3DTexture9*	m_pZero;
	IDirect3DTexture9*	m_pOne;
	IDirect3DTexture9*	m_pTwo;
	IDirect3DTexture9*	m_pThree;
	IDirect3DTexture9*	m_pFour;
	IDirect3DTexture9*	m_pFive;
	IDirect3DTexture9*	m_pSix;
	IDirect3DTexture9*	m_pSeven;
	IDirect3DTexture9*	m_pEight;
	IDirect3DTexture9*	m_pBomb;
	IDirect3DTexture9*	m_pUnsure;
	IDirect3DTexture9*	m_pFlagged;
	IDirect3DTexture9*	m_pMouse;

	HWND				m_hWnd;
	HINSTANCE			m_hInstance;

	CGraphics*			m_pGraphics;
	CInput*				m_pInput;
	CText*				m_pText;

	DWORD				m_nBoardWidth;
	DWORD				m_nBoardHeight;

	DWORD				m_nMouseX;
	DWORD				m_nMouseY;

	BOOL				m_bML;
	BOOL				m_bMR;

	SQUARE*				m_pBoard;

	BOOL				m_bGameOver;

public:
	CGame();
	virtual ~CGame();

	HRESULT	Initialize( HWND hWnd, HINSTANCE hInstance );
	HRESULT	Shutdown();

	HRESULT Run();

	HRESULT	EndGame();
	HRESULT	RefreshGameBoard( DWORD nWidth, DWORD nHeight, SQUARE* pBoard );
	HRESULT	UpdateBoard( DWORD x, DWORD y, DWORD Type );

protected:
	HRESULT Update();
	HRESULT Render();
};

#endif // _GAME_H
