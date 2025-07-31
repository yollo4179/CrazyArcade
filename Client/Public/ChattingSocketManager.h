#pragma once
#include "Base.h"
#include"Client_Defines.h"
BEGIN(Client)
class ChattingSocketManager final :CBase
{
public:
	ChattingSocketManager(); 

	virtual ~ChattingSocketManager();

public: 
	HRESULT Initialize(const _wstring&&  HOST_IP,const _wstring&& PORT_NUMBER,_bool isOnChattingLevel=true); /*쓰레드 준비 로더에서 준비하나? 흠... */
protected:
	_uint RecvLoop();
	_uint SendLoop();
	_uint ReqestSessionID();
private:
	friend  unsigned int WINAPI ThreadListening(void* pArg);
	friend  unsigned int WINAPI ThreadSending(void* pArg);

public:
	static ChattingSocketManager& Get() { return m_ChattingManager; }
	void SetChattingLevel(_bool _bChattingLevel) { m_isOnChattingLevel = _bChattingLevel; };
public:
	void SendInputString(_wstring InputString) { 
		m_MyString = InputString;
		m_bSendState = true; 
//#ifdef _DEBUG 
		/*if (ChattingLogs.size() >= CONST_MESSAGE_LIMIT) {
			++DisplayIndex[0]; ++DisplayIndex[1];
		}
		ChattingLogs.push_back(InputString);*/
//#endif
	};
	void SendNowInfo() { m_bSendState = true; };

	deque<pair< _wstring, _vector> >& GetChattingLogs() { return ChattingLogs;  };
	pair<_int, _int> GetDisplayIndex() { return make_pair(DisplayIndex[0], DisplayIndex[1]); };
	void SetIndex(_uint idx1, _uint idx2) { DisplayIndex[0] = idx1; DisplayIndex[1] = idx2; }
	size_t GetLogSize() { return ChattingLogs.size(); }
private:
	/**/
	shared_ptr<CGameInstance> m_pGameInstance;

	static ChattingSocketManager m_ChattingManager;
	deque<pair< _wstring,_vector > >ChattingLogs;
	_uint DisplayIndex[2] = { 0,7 };
	const int CONST_MESSAGE_LIMIT = { 7 };
	/*크리티컬 섹션*/
	CRITICAL_SECTION m_CriticalSection = {};
	
	HANDLE m_hRecv= {};
	HANDLE m_hSend = {}; 
	SOCKET m_hSocket = {};
	_uint AcquireSocket(_wstring HOST_IP,_wstring PORT_NUMBER);
	
	/*Connection Info*/
	_wstring m_HOST_IP;
	_wstring m_PORT_NUMBER; 
	int m_SessionID = { -1 };

	
	

private: 
	_bool m_isOnChattingLevel = {false};

	_wstring m_MyString = {}; // Input Box에서 받아서 뿌려줄것임  Static 으로 받아와서 채워주기  
	_wstring m_MyAccont = {}; 
	_bool m_bSendState = {false};//인풋 박스에서 복사후에 SendState를 True로 만들어주면 메시지 보내고 false 로 다시 고친다  
};
END
