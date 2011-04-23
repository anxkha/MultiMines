// ----------------------------------------------------------------------------
//  Filename: text.h
//  Author: Lucas Suggs
//
//  Date Created: 6 March 2008
//  Date Modified: 6 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------
#ifndef _TEXT_H
#define _TEXT_H

#include <d3dx9.h>
#include <tchar.h>

class CText
{
protected:
	IDirect3DDevice9*	m_pDevice;
	ID3DXFont*			m_pFont;

public:
	CText();
	virtual ~CText();

	HRESULT	Initialize( IDirect3DDevice9* pDevice );
	HRESULT	Shutdown();

	VOID	Print( int x, int y, DWORD color, LPCWSTR sText );
};

#endif // _TEXT_H
