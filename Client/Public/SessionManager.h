#pragma once
#include"Base.h"
#include"Client_Defines.h"
BEGIN(Client)
struct SessionManager final :CBase
{
public:
	SessionManager(){};

	static SessionManager& Get() { return m_Instance; };
public:
	static SessionManager m_Instance; 
	_wstring AccountID = {}; 
	_int MyCharacter = {0};
	int SessionID = {-1};
	
};
END

