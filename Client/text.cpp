// ----------------------------------------------------------------------------
//  Filename: text.cpp
//  Author: Lucas Suggs
//
//  Date Created: 6 March 2008
//  Date Modified: 14 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "text.h"




// ----------------------------------------------------------------------------
//  Name: CText
//
//  Desc: Constructor
// ----------------------------------------------------------------------------
CText::CText()
{
}




// ----------------------------------------------------------------------------
//  Name: ~CText
//
//  Desc: Destructor
// ----------------------------------------------------------------------------
CText::~CText()
{
}




// ----------------------------------------------------------------------------
//  Name: Initialize
//
//  Desc: Initialize the text drawing engine.
// ----------------------------------------------------------------------------
HRESULT CText::Initialize( IDirect3DDevice9* pDevice )
{
	HRESULT	hr;
	HDC		hDC = GetDC( NULL );
    int		nLogPixelsY = GetDeviceCaps( hDC, LOGPIXELSY );
	int		nHeight = -12 * nLogPixelsY / 72;

	// Store the device pointer.
	m_pDevice = pDevice;

	// Attempt to create the font for drawing text.
    hr = D3DXCreateFont( m_pDevice,             // D3D device
                         nHeight,               // Height
                         0,                     // Width
                         FW_BOLD,               // Weight
                         1,                     // MipLevels, 0 = autogen mipmaps
                         FALSE,                 // Italic
                         DEFAULT_CHARSET,       // CharSet
                         OUT_DEFAULT_PRECIS,    // OutputPrecision
                         DEFAULT_QUALITY,       // Quality
                         DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
                         _T("Verdana"),              // pFaceName
                         &m_pFont );              // ppFont
    if( FAILED( hr ) ) return hr;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Shutdown
//
//  Desc: Shuts down the text drawing engine.
// ----------------------------------------------------------------------------
HRESULT CText::Shutdown()
{
	if( m_pFont ) m_pFont->Release();

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Print
//
//  Desc: Draw text to the device buffer.
// ----------------------------------------------------------------------------
VOID CText::Print( int x, int y, DWORD color, LPCWSTR sText )
{
	RECT rc;

	SetRect( &rc, x, y, 0, 0 );

	m_pFont->DrawText( NULL, sText, -1, &rc, DT_NOCLIP, D3DXCOLOR(color) );
}
