// ----------------------------------------------------------------------------
//  Filename: game.cpp
//  Author: Lucas Suggs
//
//  Date Created: 11 February 2008
//  Date Modified: 20 March 2008
//
//  This file is part of "Minesweeper".
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "game.h"
#include "debug.h"
#include "types.h"
#include "protocol.h"




// ----------------------------------------------------------------------------
//  Name: CGame
//
//  Desc: Constructor
// ----------------------------------------------------------------------------
CGame::CGame()
{
	m_pGraphics = NULL;
	m_pDevice = NULL;
	m_pInput = NULL;
	m_pText = NULL;
	m_pBoard = NULL;
}




// ----------------------------------------------------------------------------
//  Name: ~CGame
//
//  Desc: Destructor
// ----------------------------------------------------------------------------
CGame::~CGame()
{
	m_pGraphics = NULL;
	m_pDevice = NULL;
	m_pInput = NULL;
	m_pText = NULL;
	m_pBoard = NULL;
}




// ----------------------------------------------------------------------------
//  Name: Initialize
//
//  Desc: Initializes the main game engine.
// ----------------------------------------------------------------------------
HRESULT CGame::Initialize( HWND hWnd, HINSTANCE hInstance )
{
	HRESULT hr;

	// Save the window handle and program instance.
	m_hWnd = hWnd;
	m_hInstance = hInstance;

	// Initialize the graphics engine.
	m_pGraphics = new CGraphics();

	hr = m_pGraphics->Initialize( hWnd );
	if( FAILED( hr ) ) return hr;

	// Get the graphics device.
	m_pDevice = m_pGraphics->GetDevice();

	// Initialize the input engine.
	m_pInput = new CInput();

	hr = m_pInput->Create( m_hInstance, m_hWnd, (INPUT_CREATE_MOUSE | INPUT_CREATE_KEYBOARD) );
	if( FAILED( hr ) ) return hr;

	m_pInput->Restore();

	// Initialize the text drawing engine.
	m_pText = new CText();

	hr = m_pText->Initialize( m_pDevice );
	if( FAILED( hr ) ) return hr;

	m_bGameOver = false;

	// Load Textures;
	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\blank.bmp", &m_pBlank );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file blank.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\0.bmp", &m_pZero );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 0.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\1.bmp", &m_pOne );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 1.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\2.bmp", &m_pTwo );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 2.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\3.bmp", &m_pThree );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 3.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\4.bmp", &m_pFour );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 4.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\5.bmp", &m_pFive );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 5.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\6.bmp", &m_pSix );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 6.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\7.bmp", &m_pSeven );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 7.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\8.bmp", &m_pEight);
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file 8.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\bomb.bmp", &m_pBomb );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file bomb.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\unsure.bmp", &m_pUnsure );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file unsure.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\flagged.bmp", &m_pFlagged );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file flagged.bmp" );
		return hr;
	}

	hr = D3DXCreateTextureFromFile( m_pDevice, L".\\Data\\Images\\cursor.bmp", &m_pMouse );
	if( FAILED( hr ) )
	{
		PrintDebug( "Unable to load image file cursor.bmp" );
		return hr;
	}

	m_nMouseX = 512;
	m_nMouseY = 383;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Run
//
//  Desc: Contains the main game loop.
// ----------------------------------------------------------------------------
HRESULT CGame::Run()
{
	MSG msg;

	ZeroMemory( &msg, sizeof(MSG) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if( FAILED( Update() ) )
			{
				PostMessage( NULL, WM_CLOSE, 0U, 0U );

				return D3D_OK;
			}
			if( FAILED( Render() ) )
			{
				PostMessage( NULL, WM_CLOSE, 0U, 0U );

				return D3D_OK;
			}
		}
	}

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Shutdown
//
//  Desc: Shuts down the main game engine.
// ----------------------------------------------------------------------------
HRESULT CGame::Shutdown()
{
	if( m_pBlank ) m_pBlank->Release();
	if( m_pZero ) m_pZero->Release();
	if( m_pOne ) m_pOne->Release();
	if( m_pTwo ) m_pTwo->Release();
	if( m_pThree ) m_pThree->Release();
	if( m_pFour ) m_pFour->Release();
	if( m_pFive ) m_pFive->Release();
	if( m_pSix ) m_pSix->Release();
	if( m_pSeven ) m_pSeven->Release();
	if( m_pEight ) m_pEight->Release();
	if( m_pBomb ) m_pBomb->Release();
	if( m_pUnsure ) m_pUnsure->Release();
	if( m_pFlagged ) m_pFlagged->Release();
	if( m_pMouse ) m_pMouse->Release();

	if( m_pInput ) m_pInput->Delete();
	if( m_pText ) m_pText->Shutdown();
	if( m_pGraphics ) m_pGraphics->Shutdown();
	
	if( m_pBoard ) delete m_pBoard;

	delete m_pGraphics;
	delete m_pInput;
	delete m_pText;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Update
//
//  Desc: Updates the scene.
// ----------------------------------------------------------------------------
HRESULT CGame::Update()
{
	BOOL bL, bR;
	FLOAT nX, nY;
	char temp[255];

	// If the user presses escape, the game is over.
	if( m_pInput->KeyDown( DIK_ESCAPE ) ) return E_FAIL;

	if( m_bGameOver ) return E_FAIL;

	m_pInput->GetMouse( &bL, &bR, &nX, &nY );

	m_nMouseX += nX;
	m_nMouseY += nY;

	if( m_nMouseX < 0 ) m_nMouseX = 0;
	if( m_nMouseY < 0 ) m_nMouseY = 0;
	if( m_nMouseX > 1023 ) m_nMouseX = 1023;
	if( m_nMouseY > 767 ) m_nMouseY = 767;

	if( m_pBoard )
	{
		if( (m_nMouseX >= 100) && (m_nMouseY >= 100) && (m_nMouseX <= (100 + m_nBoardWidth * 16)) && (m_nMouseY <= (100 + m_nBoardHeight * 16)) )
		{
			int tempX = (m_nMouseX - 100) / 16;
			int tempY = (m_nMouseY - 100) / 16;

			if( bL )
			{
				if( !m_bML )
				{
					SendMove( tempX, tempY, MINE_MOVE_UNCOVER );

					sprintf( temp, "Clicked square: %d %d", tempX, tempY );
					PrintDebug( temp );

					m_bML = true;
				}
			}
			else if( bR )
			{
				if( !m_bMR )
				{
					SendMove( tempX, tempY, MINE_MOVE_FLAG );

					sprintf( temp, "Clicked square: %d %d", tempX, tempY );
					PrintDebug( temp );

					m_bMR = true;
				}
			}
		}
	}

	if( !bL ) m_bML = false;
	if( !bR ) m_bMR = false;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Render
//
//  Desc: Renders the scene.
// ----------------------------------------------------------------------------
HRESULT CGame::Render()
{
	HRESULT hr;

	// Check to make sure the device is render-able;
	hr = m_pDevice->TestCooperativeLevel();
	switch( hr )
	{
	case D3DERR_DRIVERINTERNALERROR:
		return hr;

	case D3DERR_DEVICELOST:
		return D3D_OK;

	case D3DERR_DEVICENOTRESET:
		return m_pGraphics->Repair();

	default:
		break;
	}

	// Clear the backbuffer.
	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );

	// Process the scene.
	if( SUCCEEDED( m_pDevice->BeginScene() ) )
	{
		m_pText->Print( 10, 600, 0xFF0000FF, L"Press \"esc\" to exit the game." );

		TVERTEX2D v[6];

		v[0].x = m_nMouseX;
		v[0].y = m_nMouseY;
		v[0].z = 0.3f;
		v[0].rhw = 1.0f;
		v[0].color = 0xFFFFFFFF;
		v[0].tu = 0.0f;
		v[0].tv = 0.0f;

		v[1].x = m_nMouseX + 21;
		v[1].y = m_nMouseY;
		v[1].z = 0.3f;
		v[1].rhw = 1.0f;
		v[1].color = 0xFFFFFFFF;
		v[1].tu = 1.0f;
		v[1].tv = 0.0f;

		v[2].x = m_nMouseX;
		v[2].y = m_nMouseY + 27;
		v[2].z = 0.3f;
		v[2].rhw = 1.0f;
		v[2].color = 0xFFFFFFFF;
		v[2].tu = 0.0f;
		v[2].tv = 1.0f;

		v[3].x = m_nMouseX + 21;
		v[3].y = m_nMouseY;
		v[3].z = 0.3f;
		v[3].rhw = 1.0f;
		v[3].color = 0xFFFFFFFF;
		v[3].tu = 1.0f;
		v[3].tv = 0.0f;

		v[4].x = m_nMouseX + 21;
		v[4].y = m_nMouseY + 27;
		v[4].z = 0.3f;
		v[4].rhw = 1.0f;
		v[4].color = 0xFFFFFFFF;
		v[4].tu = 1.0f;
		v[4].tv = 1.0f;

		v[5].x = m_nMouseX;
		v[5].y = m_nMouseY + 27;
		v[5].z = 0.3f;
		v[5].rhw = 1.0f;
		v[5].color = 0xFFFFFFFF;
		v[5].tu = 0.0f;
		v[5].tv = 1.0f;

		m_pDevice->SetFVF( D3DFVF_TVERTEX2D );
		m_pDevice->SetTexture( 0, m_pMouse );
		m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &v, sizeof(TVERTEX2D) );

		if( m_pBoard )
		{
			m_pText->Print( 10, 10, 0xFF0000FF, L"Test?" );

			for( int r = 0; r < m_nBoardHeight; r++ )
			{
				for( int c = 0; c < m_nBoardWidth; c++ )
				{
					v[0].x = 100 + c * 16;
					v[0].y = 100 + r * 16;
					v[0].z = 0.5f;
					v[0].rhw = 1.0f;
					v[0].color = 0xFFFFFFFF;
					v[0].tu = 0.0f;
					v[0].tv = 0.0f;

					v[1].x = 100 + c * 16 + 16;
					v[1].y = 100 + r * 16;
					v[1].z = 0.5f;
					v[1].rhw = 1.0f;
					v[1].color = 0xFFFFFFFF;
					v[1].tu = 1.0f;
					v[1].tv = 0.0f;

					v[2].x = 100 + c * 16;
					v[2].y = 100 + r * 16 + 16;
					v[2].z = 0.5f;
					v[2].rhw = 1.0f;
					v[2].color = 0xFFFFFFFF;
					v[2].tu = 0.0f;
					v[2].tv = 1.0f;

					v[3].x = 100 + c * 16 + 16;
					v[3].y = 100 + r * 16;
					v[3].z = 0.5f;
					v[3].rhw = 1.0f;
					v[3].color = 0xFFFFFFFF;
					v[3].tu = 1.0f;
					v[3].tv = 0.0f;

					v[4].x = 100 + c * 16 + 16;
					v[4].y = 100 + r * 16 + 16;
					v[4].z = 0.5f;
					v[4].rhw = 1.0f;
					v[4].color = 0xFFFFFFFF;
					v[4].tu = 1.0f;
					v[4].tv = 1.0f;

					v[5].x = 100 + c * 16;
					v[5].y = 100 + r * 16 + 16;
					v[5].z = 0.5f;
					v[5].rhw = 1.0f;
					v[5].color = 0xFFFFFFFF;
					v[5].tu = 0.0f;
					v[5].tv = 1.0f;

					if( m_bGameOver )
					{
						if( m_pBoard[r * m_nBoardHeight + c].IsFlagged )
						{
							m_pDevice->SetTexture( 0, m_pFlagged );
						}
						if( m_pBoard[r * m_nBoardHeight + c].IsUnsure )
						{
							m_pDevice->SetTexture( 0, m_pUnsure );
						}
						else
						{
							m_pDevice->SetTexture( 0, m_pBomb );
						}
					}
					else
					{
						if( m_pBoard[r * m_nBoardHeight + c].IsFlagged )
						{
							m_pDevice->SetTexture( 0, m_pFlagged );
						}
						else if( m_pBoard[r * m_nBoardHeight + c].IsUnsure )
						{
							m_pDevice->SetTexture( 0, m_pUnsure );
						}
						else if( m_pBoard[r * m_nBoardHeight + c].IsUncovered )
						{
							if( m_pBoard[r * m_nBoardHeight + c].IsBomb )
							{
								m_pDevice->SetTexture( 0, m_pBomb );
							}
							else
							{
								switch( m_pBoard[r * m_nBoardHeight + c].NumAdjBombs )
								{
								case 0:
									m_pDevice->SetTexture( 0, m_pZero );
									break;
									
								case 1:
									m_pDevice->SetTexture( 0, m_pOne );
									break;

								case 2:
									m_pDevice->SetTexture( 0, m_pTwo );
									break;

								case 3:
									m_pDevice->SetTexture( 0, m_pThree );
									break;

								case 4:
									m_pDevice->SetTexture( 0, m_pFour );
									break;

								case 5:
									m_pDevice->SetTexture( 0, m_pFive );
									break;

								case 6:
									m_pDevice->SetTexture( 0, m_pSix );
									break;

								case 7:
									m_pDevice->SetTexture( 0, m_pSeven );
									break;

								case 8:
									m_pDevice->SetTexture( 0, m_pEight );
									break;
								}
							}
						}
						else
						{
							m_pDevice->SetTexture( 0, m_pBlank );
						}
					}

					m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &v, sizeof(TVERTEX2D) );
				}
			}
		}

		m_pDevice->EndScene();
	}

	// Present the backbuffer to the screen.
	m_pDevice->Present( NULL, NULL, NULL, NULL );

	return D3D_OK;
}




HRESULT CGame::EndGame()
{
	m_bGameOver = true;

	return D3D_OK;
}




HRESULT	CGame::RefreshGameBoard( DWORD nWidth, DWORD nHeight, SQUARE* pBoard )
{
	char temp[255];

	if( m_pBoard ) delete m_pBoard;

	m_nBoardWidth = nWidth;
	m_nBoardHeight = nHeight;

	m_pBoard = new SQUARE[m_nBoardWidth * m_nBoardHeight];

	memcpy( m_pBoard, pBoard, (m_nBoardWidth * m_nBoardHeight * sizeof(SQUARE)) );

	sprintf( temp, "Gameboard refresh: %d %d 0x%X", m_nBoardWidth, m_nBoardHeight, m_pBoard );

	PrintDebug( temp );

	return D3D_OK;
}




HRESULT CGame::UpdateBoard( DWORD x, DWORD y, DWORD Type )
{
	char temp[255];

	if( Type == MINE_MOVE_FLAG )
	{
		if( m_pBoard[y * m_nBoardHeight + x].IsFlagged == TRUE )
		{
			m_pBoard[y * m_nBoardHeight + x].IsFlagged = FALSE;
			m_pBoard[y * m_nBoardHeight + x].IsUnsure = TRUE;
		}
		else if( m_pBoard[y * m_nBoardHeight + x].IsUnsure == TRUE )
		{
			m_pBoard[y * m_nBoardHeight + x].IsUnsure = FALSE;
		}
		else
		{
			m_pBoard[y * m_nBoardHeight + x].IsFlagged = TRUE;
		}
	}
	else
	{
		m_pBoard[y * m_nBoardHeight + x].IsUncovered = TRUE;

		sprintf( temp, "Received board update for %d %d. Number of bombs %d", x, y, m_pBoard[y * m_nBoardHeight + x].NumAdjBombs );
		PrintDebug( temp );
	}

	return D3D_OK;
}
