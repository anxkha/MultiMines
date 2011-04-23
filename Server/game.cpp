#include "game.h"
#include "debug.h"
#include "protocol.h"
#include <math.h>




DWORD		g_nWidth, g_nHeight, g_nNumBombs;

SQUARE**	g_ppBoard;




// ----------------------------------------------------------------------------
//  Name: TrueRandNum
//
//  Desc: Statistically, the C++ standard library rand() function does not
//        return a truly random number. This function uses a different
//        procedure to generate a truly random number (or as close as it can
//        possibly get).
// ----------------------------------------------------------------------------
DWORD TrueRandNum( int nLowBound, int nHighBound )
{
		MMTIME now;
		int i;
		DWORD seed = 0;
		PUCHAR p;

		// Get the system time.
		timeGetSystemTime( &now, sizeof(MMTIME) );

		p = (PUCHAR)&now.u.ms;

		// Create a hash of the system time for the random seed.
		for( i = 0; i < sizeof(now.u.ms); i++ ) seed = seed * (UCHAR_MAX + 2U) + p[i];

		// Seed the random number generator.
		srand( seed );

		// This fixes the lower/upper bound issue with rand().
		return nLowBound + (rand() * (1.0 / (RAND_MAX + 1.0))) * nHighBound;
}




VOID InitializeGame( DWORD nWidth, DWORD nHeight, DWORD nNumBombs )
{
	int nTempCount;
	char temp[255];

	g_nWidth = nWidth;
	g_nHeight = nHeight;
	g_nNumBombs = nNumBombs;

	g_ppBoard = new SQUARE*[g_nHeight];

	for( int i = 0; i < g_nHeight; i++ ) g_ppBoard[i] = new SQUARE[g_nWidth];

	nTempCount = g_nNumBombs;

	for( int i = 0; i < g_nHeight; i++ )
	{
		for( int j = 0; j < g_nWidth; j++ )
		{
			g_ppBoard[i][j].IsFlagged = FALSE;
			g_ppBoard[i][j].IsUnsure = FALSE;
			g_ppBoard[i][j].IsUncovered = FALSE;
			g_ppBoard[i][j].IsBomb = FALSE;
			g_ppBoard[i][j].NumAdjBombs = 0;
			
			if( TrueRandNum( 1, 100 ) < 20 )
			{
				if( nTempCount > 0 )
				{
					g_ppBoard[i][j].IsBomb = TRUE;
					nTempCount--;
				}
			}
		}
	}

	for( int i = 0; i < g_nHeight; i++ )
	{
		for( int j = 0; j < g_nWidth; j++ )
		{
			if( !g_ppBoard[i][j].IsBomb )
			{
				if( (i == 0) && (j == 0) )
				{
					if( g_ppBoard[i + 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 0][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else if( (i == 0) && (j == (g_nWidth - 1)) )
				{
					if( g_ppBoard[i + 0][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j - 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else if( (i == (g_nHeight - 1)) && (j == 0) )
				{
					if( g_ppBoard[i - 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 0][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else if( (i == (g_nHeight - 1)) && (j == (g_nWidth - 1)) )
				{
					if( g_ppBoard[i - 1][j - 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 0][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else if( (i == 0) && (j > 0) && (j < (g_nWidth - 1)) )
				{
					if( g_ppBoard[i + 0][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 0][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else if( (i == (g_nHeight - 1)) && (j > 0) && (j < (g_nWidth - 1)) )
				{
					if( g_ppBoard[i - 0][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 0][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else if( (i > 0) && (i < (g_nHeight - 1)) && (j == 0) )
				{
					if( g_ppBoard[i - 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 0][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else if( (i > 0) && (i < (g_nHeight - 1)) && (j == (g_nWidth - 1)) )
				{
					if( g_ppBoard[i - 1][j - 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 0][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j - 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
				else
				{
					if( g_ppBoard[i - 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i - 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 0][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 0][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j - 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j + 0].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
					if( g_ppBoard[i + 1][j + 1].IsBomb ) g_ppBoard[i][j].NumAdjBombs++;
				}
			}
		}
	}
}




SQUARE* GetBoard()
{
	SQUARE* temp;

	temp = new SQUARE[g_nWidth * g_nHeight];

	int k = 0;

	for( int i = 0; i < g_nHeight; i++ )
	{
		for( int j = 0; j < g_nWidth; j++ )
		{
			memcpy( &temp[k++], &g_ppBoard[i][j], sizeof(SQUARE) );
		}
	}

	return temp;
}




int GetBoardWidth()
{
	return g_nWidth;
}




int GetBoardHeight()
{
	return g_nHeight;
}




int GameMove( int x, int y, int Type )
{
	if( Type == MINE_MOVE_FLAG )
	{
		if( g_ppBoard[y][x].IsFlagged == TRUE )
		{
			g_ppBoard[y][x].IsFlagged = FALSE;
			g_ppBoard[y][x].IsUnsure = TRUE;
		}
		else if( g_ppBoard[y][x].IsUnsure == TRUE )
		{
			g_ppBoard[y][x].IsUnsure = FALSE;
		}
		else
		{
			g_ppBoard[y][x].IsFlagged = TRUE;
		}
	}
	else
	{
		g_ppBoard[y][x].IsUncovered = TRUE;

		if( g_ppBoard[y][x].IsBomb ) return 1;
	}

	return 0;
}
