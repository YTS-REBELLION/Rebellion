// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.



// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#include <Windows.h>

#include "global.h"


#include<iostream>
using namespace std;
//콘솔창키기
#ifdef _DEBUG
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
#endif




//namespace Vector3
//{
//	inline XMFLOAT3 Normalize(XMFLOAT3& xmf3Vector)
//	{
//		XMFLOAT3 m_xmf3Normal;
//		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
//		return(m_xmf3Normal);
//	}
//	inline float DotProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
//		return(xmf3Result.x);
//	}
//
//	inline XMFLOAT3 CrossProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, bool bNormalize = true)
//	{
//		XMFLOAT3 xmf3Result;
//		if (bNormalize)
//			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
//		else
//			XMStoreFloat3(&xmf3Result, XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
//		return(xmf3Result);
//	}
//
//}


typedef struct tagTrailVertex
{
	Vec3 vPosition;
	DWORD dwColor;
	Vec2 vTexUV;
}TRIALVTX;
//const unsigned long		FVF_TRAIL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;