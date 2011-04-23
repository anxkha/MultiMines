// Preprocessor directives.
#include <windows.h>
#include <winsock.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <map>
#include "debug.h"
#include "game.h"
#include "protocol.h"
using namespace std;




// Global declarations.
SOCKET	g_tServerSocket;

HWND	g_hWnd;

bool	g_bGameOver = false;

string	g_sGameName;

bool	InitializeNetworking( int nServerPort );
VOID	BroadcastNetwork( MINE_PACKET p );
VOID	ShutdownNetworking();

DWORD WINAPI	ServerThread( LPVOID lpParam );
DWORD WINAPI	ClientThread( LPVOID lpParam );

map<SOCKET, CLIENT>	g_lGameClients;




int main( int argc, char** argv )
{
	
	int nServerPort;
	int nWidth, nHeight, nNumBombs;
	char temp[255];

	// Initialize the debug engine.
	InitializeDebug();

	PrintDebug( "Booting up the minesweeper multiplayer server." );

	// Find out some basic information.
	cout << "Enter the port the server will use: ";
	cin >> nServerPort;

	sprintf( temp, "Using port %d for the server.", nServerPort );
	PrintDebug( temp );
	
	cout << "Enter the name of this game: ";
	cin >> g_sGameName;

	PrintDebug( "Using " + g_sGameName + " as the name of this game instance." );

	cout << "Enter the width of the board: ";
	cin >> nWidth;

	sprintf( temp, "Using width of %d for the board.", nWidth );
	PrintDebug( temp );

	cout << "Enter the height of the board: ";
	cin >> nHeight;

	sprintf( temp, "Using height of %d for the board.", nHeight );
	PrintDebug( temp );

	cout << "Enter the number of bombs on the board: ";
	cin >> nNumBombs;

	sprintf( temp, "Using number of bombs %d for the board.", nNumBombs );
	PrintDebug( temp );

	cout << "\nThank you. To end the game prematurely at any time, press Ctrl+C in this window. Enjoy your game.\n";

	InitializeGame( nWidth, nHeight, nNumBombs );
	InitializeNetworking( nServerPort );

	while( !g_bGameOver );

	// Terminate.
	PrintDebug( "Program terminated gracefully." );

	ShutdownNetworking();
	ShutdownDebug();

	return 0;
}




VOID ShutdownNetworking()
{
	map<SOCKET, CLIENT>::iterator it;

	MINE_PACKET p;

	p.Type = MINE_TYPE_ENDGAME;
	p.DataSize = 0;
	memset( p.Data, 0x00, 500 );

	BroadcastNetwork( p );

	for( it = g_lGameClients.begin(); it != g_lGameClients.end(); it++ )
	{
		closesocket( it->first );
	}
}




VOID BroadcastNetwork( MINE_PACKET p )
{
	char* b = new char[32768];

	map<SOCKET, CLIENT>::iterator it;

	((u_long*)b)[0] = htonl( p.Type );
	((u_long*)b)[1] = htonl( p.DataSize );
	((u_long*)b)[2] = htonl( p.PartsRemaining );

	if( p.Data ) memcpy( (void*)((u_long)b + 12), p.Data, p.DataSize );

	for( it = g_lGameClients.begin(); it != g_lGameClients.end(); it++ )
	{
		send( it->first, b, 32768, 0 );
	}

	delete b;
}




bool InitializeNetworking( int nServerPort )
{
	SOCKADDR_IN	addr;
	WSADATA		wsd;
	int			result;

	ZeroMemory( &g_tServerSocket, sizeof(SOCKET) );

	// Attempt to start winsock.
	result = WSAStartup( 0x0202, &wsd );
	if( result ) return false;

	// Check to make sure we're using the right version of winsock.
	if( wsd.wVersion != 0x0202 )
	{
		WSACleanup();
		return false;
	}

	// Set up the socket address structure.
	addr.sin_family = AF_INET;
	addr.sin_port = htons( nServerPort );
	addr.sin_addr.s_addr = htonl( INADDR_ANY );

	// Attempt to create a socket.
	g_tServerSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( g_tServerSocket == INVALID_SOCKET ) return false;

	// Attempt to bind the socket to the server port.
	if( bind( g_tServerSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR_IN) ) == SOCKET_ERROR )
	{
		closesocket( g_tServerSocket );
		return false;
	}

	CreateThread( NULL, 0, ServerThread, (LPVOID)&g_tServerSocket, 0, NULL );

	return true;
}




DWORD WINAPI ServerThread( LPVOID lpParam )
{
	SOCKET		s = *((SOCKET*)lpParam);
	SOCKET		temp = INVALID_SOCKET;
	SOCKADDR_IN	from;
	char*		b = new char[512];
	int			fromlen = sizeof(from);

	// Listen on the port for connections.
	listen( s, 10 );

	do
	{
		// Accept the incoming connection.
		temp = accept( s, (sockaddr*)&from, &fromlen );
		if( temp != INVALID_SOCKET )
		{
			cout << "New socket: " << temp << endl;

			cout << "Accepting connection from: " << inet_ntoa( from.sin_addr ) << endl;
			PrintDebug( "Accepting connection from: " + (string)inet_ntoa( from.sin_addr ) );

			// Request that the client identify itself.
			MINE_PACKET p;

			p.Type = MINE_TYPE_IDENT;
			p.DataSize = 0;
			p.PartsRemaining = 0;

			((u_long*)b)[0] = htonl( p.Type );
			((u_long*)b)[1] = htonl( p.DataSize );
			((u_long*)b)[2] = htonl( p.PartsRemaining );

			send( temp, b, 12, 0 );

			CLIENT c;

			c.Active = FALSE;
			c.Socket = s;

			g_lGameClients[s] = c;

			CreateThread( NULL, 0, ClientThread, (LPVOID)&temp, 0, NULL );
		}
	} while( temp != INVALID_SOCKET );

	return 0;
}




DWORD WINAPI ClientThread( LPVOID lpParam )
{
	SOCKET		s = *((SOCKET*)lpParam);
	SOCKADDR_IN	from;
	MINE_MOVE	m;
	MINE_PACKET	p;
	int			fromlen = sizeof(from);
	int			nResult = 0;
	char*		buffer = new char[32768];
	char*		b = new char[32768];

	do
	{
		memset( buffer, 0x00, 32768 );
		memset( b, 0x00, 32768 );
		memset( &p, 0x00, sizeof(MINE_PACKET) );

		// Recieve data from the input buffer.
		nResult = recv( s, buffer, 32768, 0 );
		getpeername( s, (sockaddr*)&from, &fromlen );
		if( nResult > 0 )
		{
			CLIENT c;

			c = g_lGameClients[s];
			
			p.Type = ntohl( ((u_long*)buffer)[0] );
			p.DataSize = ntohl( ((u_long*)buffer)[1] );
			p.PartsRemaining = ntohl( ((u_long*)buffer)[2] );

			p.Data = new char[p.DataSize];
			memcpy( p.Data, (void*)((u_long)buffer + 12 ), p.DataSize );

			if( !c.Active )
			{
				if( p.Type = MINE_TYPE_ADD )
				{
					c.Active = TRUE;

					g_lGameClients[s] = c;

					PrintDebug( "Client " + (string)inet_ntoa( from.sin_addr ) + " added to game " + g_sGameName + "." );
				}
				else
				{
					MINE_PACKET p2;

					p2.Type = MINE_TYPE_DENY;
					p2.DataSize = 0;
					p2.PartsRemaining = 0;

					((u_long*)b)[0] = htonl( p2.Type );
					((u_long*)b)[1] = htonl( p2.DataSize );
					((u_long*)b)[2] = htonl( p2.PartsRemaining );
					memset( (void*)((u_long)b + 12), 0x00, 32756 );

					send( s, b, 32768, 0);

					closesocket( s );

					PrintDebug( "Force closing client " + (string)inet_ntoa( from.sin_addr ) + " due to invalid response." );

					delete buffer;
					delete b;
					delete p.Data;

					return 0;
				}
			}
			else
			{
				switch( p.Type )
				{
				case MINE_TYPE_CHAT:
					PrintDebug( "Received chat message \"" + (string)p.Data + "\"." );

					BroadcastNetwork( p );

					break;

				case MINE_TYPE_MOVE:
					PrintDebug( "Received move request. Broadcasting data to clients." );

					BroadcastNetwork( p );

					if( p.DataSize == sizeof(MINE_MOVE) )
					{
						memcpy( b, p.Data, sizeof(MINE_MOVE) );

						if( GameMove( ntohl( ((u_long*)b)[0] ), ntohl( ((u_long*)b)[1] ), ntohl( ((u_long*)b)[2] ) ) )
						{
							MINE_PACKET p2;

							p2.Type = MINE_TYPE_ENDGAME;
							p2.DataSize = 0;
							p2.PartsRemaining = 0;
							p2.Data = NULL;

							BroadcastNetwork( p2 );

							g_bGameOver = true;

							delete buffer;
							delete b;
							delete p.Data;

							return 0;
						}
					}

					break;

				case MINE_TYPE_SYNC:
					MINE_PACKET p2;
					int nTempSize = 0;
					int nPos = 0;

					PrintDebug( "Received sync request from " + (string)inet_ntoa( from.sin_addr ) + "." );

					SQUARE* temp = GetBoard();

					p2.Type = MINE_TYPE_SYNC;
					p2.DataSize = GetBoardWidth() * GetBoardHeight() * sizeof(SQUARE) + sizeof(MINE_MOVE);
					p2.PartsRemaining = (nTempSize / 500) + 1;

					m.x = GetBoardWidth();
					m.y = GetBoardHeight();
					m.Type = p2.DataSize - sizeof(MINE_MOVE);

					((u_long*)b)[0] = htonl( p2.Type );
					((u_long*)b)[1] = htonl( p2.DataSize );
					((u_long*)b)[2] = htonl( p2.PartsRemaining );
					((u_long*)b)[3] = htonl( m.x );
					((u_long*)b)[4] = htonl( m.y );
					((u_long*)b)[5] = htonl( m.Type );

					memcpy( (void*)((u_long)b + 24), temp, p2.DataSize );

					send( s, b, 32768, 0 );

					delete temp;

					PrintDebug( "Sent sync data to " + (string)inet_ntoa( from.sin_addr ) + "." );

					break;
				}
			}

			delete p.Data;
		}
		else if( !nResult )
		{
			closesocket( s );
			cout << "Connection closed.\n";
		}
	} while( nResult > 0 );

	delete buffer;
	delete b;

	return 0;
}
