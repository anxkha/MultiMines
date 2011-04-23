// ----------------------------------------------------------------------------
// Filename: types.h
// Author: Lucas Suggs
//
// Date Created: 9 March 2008
// Date Modified: 9 March 2008
//
// This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------
#ifndef _TYPES_H
#define _TYPES_H

#include <d3dx9.h>

#define D3DFVF_VERTEX	D3DFVF_XYZ | D3DFVF_DIFFUSE
#define D3DFVF_LVERTEX	D3DFVF_XYZ | D3DFVF_NORMAL
#define D3DFVF_TVERTEX	D3DFVF_XYZ | D3DFVF_TEX1
#define D3DFVF_TLVERTEX	D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
#define D3DFVF_TVERTEX2D	D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE

struct TVERTEX2D
{
	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT tu, tv;
};

struct TLVERTEX
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT tu, tv;
};

#endif // _TYPES_H
