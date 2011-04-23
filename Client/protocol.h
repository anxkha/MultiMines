#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#define MINE_SIGNATURE	0xDEADBEEF

#define MINE_TYPE_INVALID	0
#define MINE_TYPE_ACK		1
#define MINE_TYPE_DENY		2
#define MINE_TYPE_ADD		3
#define MINE_TYPE_CHAT		4
#define MINE_TYPE_SYNC		5
#define MINE_TYPE_MOVE		6
#define MINE_TYPE_ENDGAME	7
#define MINE_TYPE_IDENT		8

struct MINE_PACKET
{
	long	Type;
	long	DataSize;
	long	PartsRemaining;
	char*	Data;
};

#define MINE_MOVE_FLAG		1
#define MINE_MOVE_UNCOVER	2

struct MINE_MOVE
{
	long	x;
	long	y;
	long	Type;
};

void SendMove( DWORD x, DWORD y, DWORD Type );

#endif // _PROTOCOL_H
