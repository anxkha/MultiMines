// ----------------------------------------------------------------------------
//  Filename: graphics.h
//  Author: Lucas Suggs
//
//  Date Created: 5 March 2008
//  Date Modified: 9 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------
#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <d3d9.h>

class CGraphics
{
protected:
	HWND	m_hWnd;

	IDirect3D9*				m_pD3D;
	IDirect3DDevice9*		m_pD3DDevice;

	D3DPRESENT_PARAMETERS	m_D3DPP;

public:
	CGraphics();
	virtual ~CGraphics();

	HRESULT	Initialize( HWND hWnd );
	HRESULT Shutdown();
	HRESULT Repair();

	IDirect3DDevice9*	GetDevice();
};

#endif // _GRAPHICS_H
