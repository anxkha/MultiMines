// ----------------------------------------------------------------------------
//  Filename: camera.h
//  Author: Lucas Suggs
//
//  Date Created: 14 March 2008
//  Date Modified: 14 March 2008
//
//  This file is part of "Old-Fart Racing".
// ----------------------------------------------------------------------------
#ifndef _CAMERA_H
#define _CAMERA_H

#include <d3dx9.h>
#include <math.h>

class CCamera
{
protected:
	D3DVECTOR	m_CP;
	D3DVECTOR	m_CR;

	D3DXMATRIX	m_matTranslation;
	D3DXMATRIX	m_matRotation;
	D3DXMATRIX	m_matView;

public:
	HRESULT Initialize();
	HRESULT RestoreDeviceObjects();

	VOID	Move( FLOAT d );
	VOID	Strafe( FLOAT d );
	VOID	Position( FLOAT x, FLOAT y, FLOAT z );
	VOID	Rotate( FLOAT x, FLOAT y, FLOAT z );
	VOID	PositionRel( FLOAT x, FLOAT y, FLOAT z );
	VOID	RotateRel( FLOAT x, FLOAT y, FLOAT z );

	D3DXMATRIX	CameraLookAt( D3DXVECTOR3 vPos, D3DXVECTOR3 vAt, D3DXVECTOR3 vUp );
	D3DXMATRIX	GetViewMatrix();
	D3DXMATRIX	GetProjectionMatrix( float fScreenWidth, float fScreenHeight );
};

#endif /* _CAMERA_H */
