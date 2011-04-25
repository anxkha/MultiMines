// ----------------------------------------------------------------------------
//  Filename: input.h
//  Author: Lucas Suggs
//
//  Date Created: 9 March 2008
//  Date Modified: 9 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------
#ifndef _INPUT_H
#define _INPUT_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#define INPUT_CREATE_KEYBOARD	0x01
#define INPUT_CREATE_MOUSE		0x02

class CInput
{
protected:
	IDirectInput8* m_pDI;

	IDirectInputDevice8* m_pDIKDev;
	IDirectInputDevice8* m_pDIMDev;

	BOOL m_bKeyboardInitialized;
	BOOL m_bMouseInitialized;

public:
	CInput();
	virtual ~CInput();

	HRESULT	Create( HINSTANCE hInst, HWND hWnd, DWORD dwCreateFlags );
	HRESULT	Restore();
	HRESULT	InvalidateDeviceObjects();
	HRESULT	Delete();

	VOID GetMouse( BOOL *l, BOOL *r, FLOAT *x, FLOAT *y );
	BOOL KeyDown( UCHAR key );
};

#endif /* _INPUT_H */
