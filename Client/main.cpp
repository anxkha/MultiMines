// Preprocessor directives.
#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <string>
#include "protocol.h"
#include "debug.h"
#include "game.h"
using namespace std;




// Global declarations.
SOCKET		g_tSocket;

CGame*		g_pGame;

HWND		g_hWnd;
HINSTANCE	g_hInstance;

bool		g_bInitNetDone = false;
bool		g_bInitNetSuccess = false;
bool		g_bIdentified = false;




// ----------------------------------------------------------------------------
//  Name: MsgProc
//
//  Desc: Handles system messages.
// ----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	default:
		return DefWindowProc( hWnd, msg, wp, lp );
	}
}




// ----------------------------------------------------------------------------
//  Name: ClientThread
//
//  Desc: Handles all network communication for the game.
// ----------------------------------------------------------------------------
DWORD WINAPI ClientThread( LPVOID lpParam )
{
	SOCKADDR_IN*	addr = (SOCKADDR_IN*)lpParam;
	int				nResult;
	char*			buffer = new char[32768];
	char*			b = new char[32768];
	MINE_MOVE		m;
	MINE_PACKET		p;
	MINE_PACKET		p2;
	int				nCurPacket = 0;
	int				nPos = 0;
	int				nTempBoardW;
	int				nTempBoardH;
	char*			pTempBoard;

	// Attempt to connect to the server.
	if( connect( g_tSocket, (SOCKADDR*)addr, sizeof(SOCKADDR) ) == SOCKET_ERROR )
	{
		nResult = WSAGetLastError();

		cout << "Failure!\n";

		sprintf( buffer, "Unable to connect to server using IP %s with error code %d\n", inet_ntoa( addr->sin_addr ), nResult );
		cout << buffer;
		PrintDebug( buffer );

		delete buffer;
		delete b;
		delete addr;

		return 0;
	}

	delete addr;

	PrintDebug( "Connection established." );

	g_bInitNetSuccess = true;
	g_bInitNetDone = true;

	do
	{
		memset( buffer, 0x00, 32768 );
		memset( b, 0x00, 32768 );
		memset( &p, 0x00, sizeof(MINE_PACKET) );

		nResult = recv( g_tSocket, buffer, 32768, 0 );
		if( nResult > 0 )
		{
			p.Type = ntohl( ((u_long*)buffer)[0] );
			p.DataSize = ntohl( ((u_long*)buffer)[1] );
			p.PartsRemaining = ntohl( ((u_long*)buffer)[2] );

			p.Data = new char[p.DataSize];
			memcpy( p.Data, (void*)((u_long)buffer + 12), p.DataSize );

			switch( p.Type )
			{
			case MINE_TYPE_IDENT:
				PrintDebug( "Host asked for identification." );

				p2.Type = MINE_TYPE_ADD;
				p2.DataSize = 0;
				p2.PartsRemaining = 0;

				((u_long*)b)[0] = htonl( p2.Type );
				((u_long*)b)[1] = htonl( p2.DataSize );
				((u_long*)b)[2] = htonl( p2.PartsRemaining );
				memset( (void*)((u_long)b + 12), 0x00, 32756 );

				send( g_tSocket, b, 32768, 0 );

				PrintDebug( "Identification sent." );

				g_bIdentified = true;

				break;

			case MINE_TYPE_DENY:
				g_pGame->EndGame();

				cout << "Host denied connection request.\n";
				PrintDebug( "Host denied connection request." );

				delete buffer;
				delete b;
				delete p.Data;

				return 0;

			case MINE_TYPE_MOVE:
				g_pGame->UpdateBoard( ntohl( ((u_long*)buffer)[3] ), ntohl( ((u_long*)buffer)[4] ), ntohl( ((u_long*)buffer)[5] ) );

				cout << "Received move action.\n";
				PrintDebug( "Received move action." );

				break;

			case MINE_TYPE_CHAT:
				cout << p.Data;

				PrintDebug( "Received chat message: \"" + (string)p.Data + "\"." );

				break;

			case MINE_TYPE_SYNC:
				memcpy( b, p.Data, p.DataSize );

				pTempBoard = new char[ntohl( ((u_long*)b)[2] )];
				nTempBoardW = ntohl( ((u_long*)b)[0] );
				nTempBoardH = ntohl( ((u_long*)b)[1] );

				memcpy( pTempBoard, (void*)((u_long)b + 12), p.DataSize - sizeof(MINE_MOVE) );

				g_pGame->RefreshGameBoard( nTempBoardW, nTempBoardH, (SQUARE*)pTempBoard );

				delete pTempBoard;

				cout << "Received sync command.\n";
				PrintDebug( "Received sync command." );

				break;

			case MINE_TYPE_ENDGAME:
				cout << "Game was terminated by host.\n";
				PrintDebug( "Game was terminated by host." );

				g_pGame->EndGame();

				delete buffer;
				delete b;
				delete p.Data;

				return 0;
			}

			delete p.Data;
		}
		else
		{
			g_pGame->EndGame();
		}
	} while( nResult > 0 );

	delete buffer;
	delete b;

	return 0;
}




void SendMove( DWORD x, DWORD y, DWORD Type )
{
	char* b = new char[32768];

	MINE_PACKET p;
	MINE_MOVE m;

	p.Type = MINE_TYPE_MOVE;
	p.DataSize = sizeof(MINE_MOVE);
	p.PartsRemaining = 0;
	
	m.x = x;
	m.y = y;
	m.Type = Type;

	((u_long*)b)[0] = htonl( p.Type );
	((u_long*)b)[1] = htonl( p.DataSize );
	((u_long*)b)[2] = htonl( p.PartsRemaining );
	((u_long*)b)[3] = htonl( m.x );
	((u_long*)b)[4] = htonl( m.y );
	((u_long*)b)[5] = htonl( m.Type );
	memset( (void*)((u_long)b + 24), 0x00, 32744 );

	send( g_tSocket, b, 32768, 0 );

	delete b;
}




bool InitializeNetworking( int nPort, char* sIP )
{
	SOCKADDR_IN*	addr = new SOCKADDR_IN;
	WSADATA		wsd;
	int			result;

	ZeroMemory( &g_tSocket, sizeof(SOCKET) );

	// Attempt to start winsock.
	result = WSAStartup( 0x0202, &wsd );
	if( result ) return false;

	// Check to make sure we're using the right version of winsock.
	if( wsd.wVersion != 0x0202 )
	{
		WSACleanup();
		return false;
	}

	// Attempt to create a socket.
	g_tSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( g_tSocket == INVALID_SOCKET ) return false;

	// Set up the socket address structure.
	addr->sin_family = AF_INET;
	addr->sin_port = htons( nPort );
	addr->sin_addr.s_addr = inet_addr( sIP );

	if( NULL == CreateThread( NULL, 0, ClientThread, addr, 0, NULL ) )
	{
		PrintDebug( "CreateThread failed in main.cpp:267" );
		delete addr;
		return false;
	}

	return true;
}

void SyncGame()
{
	MINE_PACKET p;
	char*		b = new char[32768];

	PrintDebug( "Waiting for server/client handshake to commence." );

	while( !g_bIdentified );

	PrintDebug( "Requesting sync from server." );

	p.Type = MINE_TYPE_SYNC;
	p.DataSize = sizeof(MINE_PACKET);
	p.PartsRemaining = 0;

	((u_long*)b)[0] = htonl( p.Type );
	((u_long*)b)[1] = htonl( p.DataSize );
	((u_long*)b)[2] = htonl( p.PartsRemaining );
	memset( (void*)((u_long)b + 12), 0x00, 32756 );

	send( g_tSocket, b, 32768, 0 );

	delete b;

	PrintDebug( "Sync request sent." );
}

int main( int argc, char** argv )
{
	int nPort;
	char sIP[16];

	InitializeDebug();

	cout << "Enter the IP of the server: ";
	cin >> sIP;

	cout << "Using IP " << sIP;

	PrintDebug( "Using host IP of " + (string)sIP + "." );

	cout << "Enter the port to use for the connection: ";
	cin >> nPort;

	cout << "Attempting connection to " << sIP << " on port " << nPort << ": ";

	if( !InitializeNetworking( nPort, sIP ) )
	{
		cout << "Failure!\n";

		PrintDebug( "Unable to establish a socket." );

		ShutdownDebug();
		WSACleanup();

		return 0;
	}

	PrintDebug( "Waiting for network initialization to complete." );

	while( !g_bInitNetDone );

	if( !g_bInitNetSuccess )
	{
		ShutdownDebug();
		WSACleanup();

		return 0;
	}

	cout << "Success!\n";

	PrintDebug( "Creating game window." );

	g_hInstance = GetModuleHandle( NULL );

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, g_hInstance, NULL, NULL, NULL, NULL, _T("MultiMines"), NULL };

	// Register the window class.
	RegisterClassEx( &wcex );

	// Create a window for the game.
	g_hWnd = CreateWindow( _T("MultiMines"), _T("MultiMines"), WS_POPUP | WS_VISIBLE, 0, 0, 1024, 768, NULL, NULL, g_hInstance, NULL );
	if( !g_hWnd )
	{
		MessageBox( NULL, _T("Unable to create the game window!"), _T("Error!"), MB_OK );
		PrintDebug( "Unable to create the game window!" );

		return 0L;
	}

	ShowWindow( g_hWnd, SW_SHOW );
	UpdateWindow( g_hWnd );

	g_pGame = new CGame();

	SyncGame();

	// Initialize the game.
	if( FAILED( g_pGame->Initialize( g_hWnd, g_hInstance ) ) )
	{
		DestroyWindow( g_hWnd );
		return 0L;
	}

	// Begin the main game loop.
	g_pGame->Run();

	// We are done, shut down.
	g_pGame->Shutdown();

	delete g_pGame;

	closesocket( g_tSocket );

	WSACleanup();

	ShutdownDebug();

	return 0;
}
