#include "stdafx.h"
#include "Camera.h"

//17.4.5 Building the View Matrix
void Camera::update(float dt)
{
 //Find the net direction the camera is traveling (since the
 //camera could be running and strafing).
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	if (gDInput->keyDown(DIK_W))
		dir += mLookW;
	if (gDInput->keyDown(DIK_S))
		dir -= mLookW;
	if (gDInput->keyDown(DIK_D))
		dir += mRightW;
	if (gDInput->keyDown(DIK_A))
		dir -= mRightW;
	// Move at mSpeed along net direction.
	D3DXVec3Normalize(&dir, &dir);
	mPosW += dir*mSpeed*dt;
	// Angle to rotate around right vector.
	float pitch = gDInput->mouseDY() / 150.0f;
	// Angle to rotate around world y-axis.
	float yAngle = gDInput->mouseDX() / 150.0f;
	// Rotate camera's look and up vectors around the
	// camera's right vector.
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &mRightW, pitch);
	D3DXVec3TransformCoord(&mLookW, &mLookW, &R);
	D3DXVec3TransformCoord(&mUpW, &mUpW, &R);
	// Rotate camera axes about the world's y-axis.
	D3DXMatrixRotationY(&R, yAngle);
	D3DXVec3TransformCoord(&mRightW, &mRightW, &R);
	D3DXVec3TransformCoord(&mUpW, &mUpW, &R);
	D3DXVec3TransformCoord(&mLookW, &mLookW, &R);
	// Rebuild the view matrix to reflect changes.
	buildView();
	mViewProj = mView * mProj;
}

//17.4.5 Building the View Matrix
void Camera::buildView()
{
	// Keep camera's axes orthogonal to each other and
	// of unit length.
	D3DXVec3Normalize(&mLookW, &mLookW);
	D3DXVec3Cross(&mUpW, &mLookW, &mRightW);
	D3DXVec3Normalize(&mUpW, &mUpW);
	D3DXVec3Cross(&mRightW, &mUpW, &mLookW);
	D3DXVec3Normalize(&mRightW, &mRightW);
	// Fill in the view matrix entries.
	float x = -D3DXVec3Dot(&mPosW, &mRightW);
	float y = -D3DXVec3Dot(&mPosW, &mUpW);
	float z = -D3DXVec3Dot(&mPosW, &mLookW);
	mView(0, 0) = mRightW.x;
	mView(1, 0) = mRightW.y;
	mView(2, 0) = mRightW.z;
	mView(3, 0) = x;
	mView(0, 1) = mUpW.x;
	mView(1, 1) = mUpW.y;
	mView(2, 1) = mUpW.z;
	mView(3, 1) = y;
	mView(0, 2) = mLookW.x;
	mView(1, 2) = mLookW.y;
	mView(2, 2) = mLookW.z;
	mView(3, 2) = z;
	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
}