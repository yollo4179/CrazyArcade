#pragma once
#ifndef Engine_Struct_h__
#define Engine_Struct_h__
#include "Engine_Typedef.h"
namespace Engine {
	enum class PROTOTYPE
	{
		PROTO_OBJ,
		PROTO_COM,
		PROTO_END
	};
	typedef struct
	{
		HINSTANCE		hInstance;
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iNumLevels;
		unsigned int	iViewportWidth;
		unsigned int	iViewportHeight;

	}ENGINE_DESC;

	/*Shaders */


	typedef struct ENGINE_DLL _VTXPOS
	{
		XMFLOAT3		vPosition;

		static const unsigned int					iNumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[1];
	}VTXPOS;


	typedef struct ENGINE_DLL _VTXPOINT
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vSize;
	}VTXPOINT;
	
	typedef struct ENGINE_DLL _VTXPOINTRECT
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];

	}VTXPOINTRECT;

	typedef struct ENGINE_DLL _VTXPOSTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];
	}VTXPOSTEX;



}
BEGIN(DirectX)

inline void to_json(json& j, const XMFLOAT4& a)
{
	j = { a.x,a.y ,a.z ,a.w };
}
inline void from_json(const json& j, XMFLOAT4& a)
{
	a.x = j[0].get<float>();
	a.y = j[1].get<float>();
	a.z = j[2].get<float>();
	a.w = j[3].get<float>();
}

inline void to_json(json& j, const XMFLOAT4X4& m)
{
	j = {
		{ "RIGHT", { m._11, m._12, m._13, m._14} },
		{ "UP",    { m._21, m._22, m._23, m._24} },
		{ "LOOK",  { m._31, m._32, m._33, m._34 } },
		{ "POSITION",   { m._41, m._42, m._43, m._44 } }
	};
}

inline void from_json(const json& j, XMFLOAT4X4& m)
{
	// 먼저 단위행렬로 초기화
	DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixIdentity());

	auto v4 = [](const json& a) { 
		return DirectX::XMFLOAT4(
		a[0].get<float>(),
		a[1].get<float>(),
		a[2].get<float>(),
		a[2].get<float>());
		};

	DirectX::XMFLOAT4 R = v4(j.at("RIGHT"));
	DirectX::XMFLOAT4 U = v4(j.at("UP"));
	DirectX::XMFLOAT4 L = v4(j.at("LOOK"));
	DirectX::XMFLOAT4 P = v4(j.at("POSITION"));

	m._11 = R.x; m._12 = R.y; m._13 = R.z; m._44 = 0.f;
	m._21 = U.x; m._22 = U.y; m._23 = U.z; m._44 = 0.f;
	m._31 = L.x; m._32 = L.y; m._33 = L.z; m._44 = 0.f;
	m._41 = P.x; m._42 = P.y; m._43 = P.z; m._44=1.f;
};
END

#endif	