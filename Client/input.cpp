// ----------------------------------------------------------------------------
//  Filename: input.cpp
//  Author: Lucas Suggs
//
//  Date Created: 9 March 2008
//  Date Modified: 9 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------




// Preprocessor directives.
#define DIRECTINPUT_VERSION 0x0800

#include "input.h"
#include "debug.h"




//-----------------------------------------------------------------------------
// Name: CInput
//
// Desc: Constructor
//-----------------------------------------------------------------------------
CInput::CInput()
{
	m_pDI					= NULL;
	m_pDIKDev				= NULL;
	m_pDIMDev				= NULL;
	m_bKeyboardInitialized	= FALSE;
	m_bMouseInitialized		= FALSE;
}




//-----------------------------------------------------------------------------
// Name: ~CInput
//
// Desc: Destructor
//-----------------------------------------------------------------------------
CInput::~CInput()
{
	m_pDI					= NULL;
	m_pDIKDev				= NULL;
	m_pDIMDev				= NULL;
	m_bKeyboardInitialized	= FALSE;
	m_bMouseInitialized		= FALSE;
}




//-----------------------------------------------------------------------------
// Name: InitDeviceObjects
//
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CInput::Create( HINSTANCE hInst, HWND hWnd, DWORD dwCreateFlags )
{
	HRESULT hr;

	// Create a DirectInput object.
	if( FAILED( hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL ) ) )
		return hr;

	// Create a Keyboard Device if desired.
	if( dwCreateFlags && INPUT_CREATE_KEYBOARD )
	{
        if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pDIKDev, NULL ) ) )
			return hr;

		if( FAILED( hr = m_pDIKDev->SetDataFormat( &c_dfDIKeyboard ) ) )
			return hr;

		if( FAILED( hr = m_pDIKDev->SetCooperativeLevel( hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE ) ) )
			return hr;

		m_bKeyboardInitialized = TRUE;
	}

	// Create a Mouse Device if desired.
	if( dwCreateFlags && INPUT_CREATE_MOUSE )
	{
		if( FAILED( hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pDIMDev, NULL ) ) )
			return hr;

		if( FAILED( hr = m_pDIMDev->SetDataFormat( &c_dfDIMouse ) ) )
			return hr;

		if( FAILED( hr = m_pDIMDev->SetCooperativeLevel( hWnd, DISCL_FOREGROUND|DISCL_EXCLUSIVE ) ) )
			return hr;

		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(DIPROPDWORD);
		diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = 16;

		if( FAILED(hr = m_pDIMDev->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph ) ) )
			return hr;

		m_bMouseInitialized = TRUE;
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects
//
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CInput::Restore()
{
	//HRESULT hr;

	// Acquire/Reacquire the keyboard device.
	if( m_bKeyboardInitialized == TRUE )
		m_pDIKDev->Acquire();

	// Acquire/Reacquire the mouse device.
	if( m_bMouseInitialized == TRUE )
		m_pDIMDev->Acquire();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects
//
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CInput::InvalidateDeviceObjects()
{
	//HRESULT hr;

	// Unacquire the keyboard and mouse devices.
	if( m_bKeyboardInitialized )
		m_pDIKDev->Unacquire();

	if( m_bMouseInitialized )
		m_pDIMDev->Unacquire();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects
//
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CInput::Delete()
{
	//HRESULT hr;

	// Destroy the keyboard and mouse devices.
	if( m_pDIKDev != NULL )
	{
		m_pDIKDev->Unacquire();
		m_pDIKDev->Release();
	}

	if( m_pDIMDev != NULL )
	{
		m_pDIMDev->Unacquire();
		m_pDIMDev->Release();
	}

	// Destroy the DirectInput object.
	if( m_pDI != NULL )
		m_pDI->Release();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: GetMouse
//
// Desc: 
//-----------------------------------------------------------------------------
VOID CInput::GetMouse( BOOL *l, BOOL *r, FLOAT *x, FLOAT *y )
{
	*l = *r = FALSE;
	*x = *y = 0.0f;

	if( !m_bMouseInitialized ) return;

	DIMOUSESTATE od;
	HRESULT hr;

	if( FAILED( hr = m_pDIMDev->GetDeviceState( sizeof(DIMOUSESTATE), &od ) ) )
	{
		switch( hr )
		{
		case DIERR_INPUTLOST:
			Restore();
			break;
		}
		return;
	}

	*x = od.lX;
	*y = od.lY;
	*l = od.rgbButtons[0];
	*r = od.rgbButtons[1];
}




//-----------------------------------------------------------------------------
// Name: KeyDown
//
// Desc: 
//-----------------------------------------------------------------------------
BOOL CInput::KeyDown( UCHAR key )
{
	char buffer[256];
	HRESULT hr;

	if( !m_bKeyboardInitialized ) return FALSE;

	// Retrieve the keyboard data.
	hr = m_pDIKDev->GetDeviceState( sizeof(buffer), (LPVOID)&buffer );
	if( FAILED( hr ) )
	{
		switch( hr )
		{
		case DIERR_INPUTLOST:
			// Device was lost, attempt to reacquire it.
			Restore();
			break;
		}
		return FALSE;
	}

	// Check if the key wanted is down.
	if( buffer[key] & 0x80 )
		return TRUE;

	return FALSE;
}
