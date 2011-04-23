// ----------------------------------------------------------------------------
//  Filename: graphics.cpp
//  Author: Lucas Suggs
//
//  Date Created: 5 March 2008
//  Date Modified: 9 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "graphics.h"




// ----------------------------------------------------------------------------
//  Name: CGraphics
//
//  Desc: Constructor
// ----------------------------------------------------------------------------
CGraphics::CGraphics()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	ZeroMemory( &m_D3DPP, sizeof(D3DPRESENT_PARAMETERS) );
}




// ----------------------------------------------------------------------------
//  Name: ~CGraphics
//
//  Desc: Destructor
// ----------------------------------------------------------------------------
CGraphics::~CGraphics()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	ZeroMemory( &m_D3DPP, sizeof(D3DPRESENT_PARAMETERS) );
}




// ----------------------------------------------------------------------------
//  Name: Initialize
//
//  Desc: Initializes the graphics engine.
// ----------------------------------------------------------------------------
HRESULT CGraphics::Initialize( HWND hWnd )
{
	HRESULT hr;

	// Save the window handle.
	m_hWnd = hWnd;

	// Attempt to create a D3D object.
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3D == NULL ) return E_FAIL;

	// Fill in the presentation parameter block.
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
	m_D3DPP.EnableAutoDepthStencil = TRUE;
	m_D3DPP.BackBufferCount = 1;
	m_D3DPP.BackBufferFormat = D3DFMT_R5G6B5;
	m_D3DPP.BackBufferHeight = 768;
	m_D3DPP.BackBufferWidth = 1024;
	m_D3DPP.hDeviceWindow = m_hWnd;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.Windowed = FALSE;

	// Create the Direct3D device.
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_D3DPP, &m_pD3DDevice );
	if( FAILED( hr ) )
	{
		m_pD3D->Release();
		return hr;
	}

	// Set up some initial render states.
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Shutdown
//
//  Desc: Destroys the graphics engine.
// ----------------------------------------------------------------------------
HRESULT CGraphics::Shutdown()
{
	if( m_pD3DDevice ) m_pD3DDevice->Release();
	if( m_pD3D ) m_pD3D->Release();

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Repair
//
//  Desc: Resets the device if it was lost.
// ----------------------------------------------------------------------------
HRESULT CGraphics::Repair()
{
	return m_pD3DDevice->Reset( &m_D3DPP );
}




// ----------------------------------------------------------------------------
//  Name: GetDevice
//
//  Desc: Returns the device object for use by other engines.
// ----------------------------------------------------------------------------
IDirect3DDevice9* CGraphics::GetDevice()
{
	return m_pD3DDevice;
}
