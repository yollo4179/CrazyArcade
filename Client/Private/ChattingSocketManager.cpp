#include"pch.h"
#include "ChattingSocketManager.h"
#include"GameInstance.h"
#include"SessionManager.h"
#include"ChatProtocols.h"

#include"PlayerFrame.h"
#include"PlayerInFrame.h"
#include"PlayerNameInFrame.h"

ChattingSocketManager ChattingSocketManager::m_ChattingManager;

ChattingSocketManager::ChattingSocketManager()
	:m_pGameInstance{CGameInstance::GetInstance()}
	 , m_MyAccont{ SessionManager::Get().AccountID}
{
}
ChattingSocketManager::~ChattingSocketManager()
{
	shutdown(m_hSocket, SD_BOTH);
	closesocket(m_hSocket); /* 윈속 해제*/
	::WSACleanup();
}
BEGIN(Client)
 unsigned int WINAPI ThreadListening(void* pArg) {


	ChattingSocketManager* pGlobalInstance = reinterpret_cast<ChattingSocketManager*>(pArg);
	pGlobalInstance->RecvLoop();
	return 0; 

};
 unsigned int WINAPI ThreadSending(void* pArg)
{
	ChattingSocketManager* GlobalInstance = reinterpret_cast<ChattingSocketManager*>(pArg);
	GlobalInstance->ReqestSessionID();
	GlobalInstance->SendLoop();

	return 0;
}
END
HRESULT ChattingSocketManager::Initialize(const  _wstring&& HOST_IP, const _wstring&& PORT_NUMBER,  _bool isOnChattingLevel )
{

	m_isOnChattingLevel = isOnChattingLevel;
	//InitializeCriticalSection(&m_CriticalSection);
	if (0 == AcquireSocket(HOST_IP, PORT_NUMBER)) { //Connect까지 진행
		MSG_BOX("인원 꽉참");
		return E_FAIL;
	}

	/*연결 완료 (나 얻었다 소켓 클라) */
	m_hRecv =
		(HANDLE)_beginthreadex(
			nullptr,
			0,
			ThreadListening,
			this,
			0,
			nullptr
		);
	if (m_hRecv== INVALID_HANDLE_VALUE)
	{
		return E_FAIL; 
	}

	m_hSend = 
		(HANDLE)_beginthreadex(
		nullptr,
		0,
		ThreadSending,
		this,
		0,
		nullptr
	);

	CloseHandle(m_hSend);
	CloseHandle(m_hRecv); // UsageCount 2-> 1 종료시 0 -> 커널 오브젝트 0 

	return S_OK; 
}
_uint ChattingSocketManager::RecvLoop()
{

	char	StreamBuffer[8192] = {};	//8KB
	 BYTE* pSeeker = (BYTE*)StreamBuffer;
	while (0 < ::recv(m_hSocket, StreamBuffer, sizeof(StreamBuffer), 0)) { //소켓 끊어지지만 않으면 됌 

		//Header 읽기
		
		PROTOCOL_HEADER  Cmd;
		memcpy(&Cmd, reinterpret_cast<const char*>(StreamBuffer), sizeof(PROTOCOL_HEADER));
		pSeeker += sizeof(PROTOCOL_HEADER);

		switch (Cmd.Protocol)
		{
		case (_uint)PROTOCOL_CODE::ACK_LOGIN:
		{
			
			{
				ACCOUNT_ID SessionID = {};
				memcpy(&SessionID, pSeeker, sizeof(ACCOUNT_ID));
				SessionManager::Get().SessionID = m_SessionID =SessionID.SSESION_ID;
				
				{
					/*SubHeader*/
					pSeeker += sizeof(ACCOUNT_ID);
					PROTOCOL_HEADER subHeader = {}; 
					memcpy(&subHeader, pSeeker, sizeof(PROTOCOL_HEADER));
					int Cnt = subHeader.NextFileSize / sizeof(PLAYER_INFO);
					pSeeker += sizeof(PROTOCOL_HEADER);

					PLAYER_INFO pInfo = {};
					for (int i = 0; i < Cnt; ++i)
					{
						memcpy(&pInfo, pSeeker, sizeof(PLAYER_INFO));
						if (-1 == pInfo.SSESION_ID)continue;

						shared_ptr<CPlayerFrame> pFrame = dynamic_pointer_cast<CPlayerFrame>(
							m_pGameInstance->GetUIObjectByTag(L"PLAYERS_BACKGROUND")->FindChildByIndex(pInfo.SSESION_ID));
						pFrame->SetSessionID(pInfo.SSESION_ID);

						shared_ptr<CPlayerInFrame> pPlayerInFrame = dynamic_pointer_cast<CPlayerInFrame>(pFrame->FindChildByIndex(0));
						pPlayerInFrame->SetCharacter(pInfo.CharacterType);

						shared_ptr<CPlayerNameInFrame> pPlayerNameInFrame = dynamic_pointer_cast<CPlayerNameInFrame>(pFrame->FindChildByIndex(1));
						pPlayerNameInFrame->SetAccountName(pInfo.Account);
						pPlayerNameInFrame->Active(); 

						pSeeker += sizeof(PLAYER_INFO);
					}

				}

				/* 님이 참가 하셨습니다 패킷*/
				BYTE PCK[sizeof(PROTOCOL_HEADER) + sizeof(CHAT_LOG)] = {};
				BYTE* pSeeker = PCK;
				/*Header*/
				int size = sizeof(CHAT_LOG);
				PROTOCOL_HEADER ProtocolHeader = { (_uint)PROTOCOL_CODE::SND_MESSAGE , sizeof(CHAT_LOG) };
				memcpy(PCK, reinterpret_cast<const char*>(&ProtocolHeader), sizeof(PROTOCOL_HEADER));
				pSeeker += sizeof(PROTOCOL_HEADER);
				/* PayLoad */

				wstring Log =  SessionManager::Get().AccountID + _wstring(L"님이 들어오셨습니다.");
				m_bSendState = false;

				CHAT_LOG m_ChatLog = {};
				memcpy(m_ChatLog.Account, SessionManager::Get().AccountID.c_str(), sizeof(m_ChatLog.Account));
				m_ChatLog.CharacterType = SessionManager::Get().MyCharacter;
				memcpy(m_ChatLog.Message, Log.c_str(), sizeof(m_ChatLog.Message));
				m_ChatLog.SSESION_ID = SessionManager::Get().SessionID;
				m_ChatLog.vColor = { 1.f,1.f,0.f,1.f };

				/*패킷 완성*/
				memcpy(pSeeker, &m_ChatLog, sizeof(CHAT_LOG));
				int Sign = ::send(m_hSocket, reinterpret_cast<const char*>(PCK), sizeof(PCK), 0);
				if (SOCKET_ERROR == Sign)
				{
					int A = WSAGetLastError();
					MSG_BOX("소켓 쓰기 에러");
				}
			}
			break;
		}
		
		case (_uint)PROTOCOL_CODE::BROAD_CAST_ALL:
		{
			{
				CHAT_LOG ChatLog = {};
				memcpy(&ChatLog, pSeeker, sizeof(CHAT_LOG));

				if (ChattingLogs.size() >= CONST_MESSAGE_LIMIT) {
					++DisplayIndex[0]; ++DisplayIndex[1];
				}
				if(lstrlen(ChatLog.Message))
					ChattingLogs.push_back( make_pair(ChatLog.Message, XMLoadFloat4(&ChatLog.vColor)));

				if (0 > ChatLog.SSESION_ID)
				{
					MSG_BOX("아니야");
					return 1;
				}

				shared_ptr<CPlayerFrame> pFrame = dynamic_pointer_cast<CPlayerFrame>(
					m_pGameInstance->GetUIObjectByTag(L"PLAYERS_BACKGROUND")->FindChildByIndex(ChatLog.SSESION_ID));
				pFrame->SetSessionID(ChatLog.SSESION_ID);

				shared_ptr<CPlayerInFrame> pPlayerInFrame= dynamic_pointer_cast<CPlayerInFrame>(pFrame->FindChildByIndex(0));
				pPlayerInFrame->SetCharacter(ChatLog.CharacterType);

				shared_ptr<CPlayerNameInFrame> pPlayerNameInFrame = dynamic_pointer_cast<CPlayerNameInFrame>(pFrame->FindChildByIndex(1));
				pPlayerNameInFrame->SetAccountName(ChatLog.Account);
				pPlayerNameInFrame->Active();
			}

			break;
		}
		case (_uint)PROTOCOL_CODE::SND_PLAYERS_INFO:
		{
			PLAYER_INFO pInfo = {};
			int Cnt = Cmd.NextFileSize / sizeof(PLAYER_INFO);

			for (int i = 0; i < Cnt; ++i)
			{
				memcpy(&pInfo, pSeeker, sizeof(PLAYER_INFO));
				pSeeker += sizeof(PLAYER_INFO);
				if (false == pInfo.IsDead)continue;
				shared_ptr<CPlayerFrame> pFrame = dynamic_pointer_cast<CPlayerFrame>(
						m_pGameInstance->GetUIObjectByTag(L"PLAYERS_BACKGROUND")->FindChildByIndex(pInfo.SSESION_ID));
				pFrame->SetSessionID(-1);

				shared_ptr<CPlayerInFrame> pPlayerInFrame = dynamic_pointer_cast<CPlayerInFrame>(pFrame->FindChildByIndex(0));
				pPlayerInFrame->SetCharacter(-1);

				shared_ptr<CPlayerNameInFrame> pPlayerNameInFrame = dynamic_pointer_cast<CPlayerNameInFrame>(pFrame->FindChildByIndex(1));
				pPlayerNameInFrame->InActive();
			}
			/*종료 Message 보내기*/
			{
				wchar_t ExitMessage[MESSAGESIZE] = {};
				memcpy(ExitMessage, pSeeker, sizeof(wchar_t) * MESSAGESIZE);
				/*Coolor*/
				_float4 ExitColor = {};
				memcpy(&ExitColor, pSeeker + sizeof(wchar_t) * MESSAGESIZE, sizeof(_float4));
				if (ChattingLogs.size() >= CONST_MESSAGE_LIMIT) {
					++DisplayIndex[0]; ++DisplayIndex[1];
				}
				if (lstrlen(ExitMessage))
					ChattingLogs.push_back(make_pair(_wstring(ExitMessage), XMLoadFloat4(&ExitColor)));
			}


			break; 
		}
		}
		pSeeker = (BYTE*)StreamBuffer;
	
	}
	return 0;
}
_uint ChattingSocketManager::SendLoop()
{
	char	StreamBuffer[8192] = {};	//8KB
	while (true == m_isOnChattingLevel)
	{

		if (true == m_bSendState &&  -1  != SessionManager::Get().SessionID  )
		{
			
			BYTE PCK[sizeof(CHAT_LOG)+ sizeof(PROTOCOL_HEADER)] = {};
			BYTE* pSeeker = PCK;
			/*Header*/
			PROTOCOL_HEADER ProtocolHeader = {(_uint)PROTOCOL_CODE::SND_MESSAGE , sizeof(CHAT_LOG)};
			memcpy(PCK, reinterpret_cast<const char*>(&ProtocolHeader), sizeof(PROTOCOL_HEADER));
			int Size = sizeof(PROTOCOL_HEADER);
			pSeeker += sizeof(PROTOCOL_HEADER);
			/* PayLoad */

			wstring Log = SessionManager::Get().AccountID + _wstring(L": ") + m_MyString;
			m_bSendState = false;

			if (!m_MyString.size()) Log = {}; 

			CHAT_LOG m_ChatLog = {}; 
			memcpy(m_ChatLog.Account, SessionManager::Get().AccountID.c_str(), sizeof(m_ChatLog.Account));
			m_ChatLog.CharacterType = SessionManager::Get().MyCharacter;
			memcpy(m_ChatLog.Message, Log.c_str(), sizeof(m_ChatLog.Message));
			m_ChatLog.SSESION_ID = SessionManager::Get().SessionID; 
			m_ChatLog.vColor = { 1.f,1.f,1.f,1.f };

			/*패킷 완성*/
			memcpy(pSeeker, &m_ChatLog, sizeof(CHAT_LOG));
			int Sign =  ::send(m_hSocket, reinterpret_cast<const char*>(PCK), sizeof(PCK),0);
			if (SOCKET_ERROR == Sign)
			{
				int A = WSAGetLastError();
				MSG_BOX("소켓 쓰기 에러"); 
			}
			m_MyString = {}; 

		}
	}

	/*클라가 종료 버튼 누르면 이거 호출해준다 // 좀비처럼 연결되어있느 것을 방지  */
	shutdown(m_hSocket, SD_BOTH);
	closesocket(m_hSocket); /* 윈속 해제*/
	::WSACleanup();
	/*여기서 끝내는게 맞는거 같다.*/
	return 0;
}

_uint ChattingSocketManager::ReqestSessionID()
{
	/*Request 보낸다.*/

	{
		PROTOCOL_HEADER REQ_LOGIN_Protocol =
		{ (_uint)PROTOCOL_CODE::REQ_LOGIN,1111 };
		/*스트림으로 보낸다.*/
		::send(m_hSocket,reinterpret_cast<const char*>(&REQ_LOGIN_Protocol),sizeof(PROTOCOL_HEADER),0);
	}

	return 0;
}

_uint  ChattingSocketManager::AcquireSocket(_wstring HOST_IP, _wstring PORT_NUMBER)
{
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		MSG_BOX("ERROR: 윈속 초기화 실패");
		return 0;
	}
	m_hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		MSG_BOX("ERROR: 리스닝 소켓 생성 실패");
		return 0;
	}
	char HOST[MAX_PATH] = {};
	char PORT[MAX_PATH] = {};

	strcpy_s(HOST, WBCS2MBCS(HOST_IP).c_str());//도메인,
	strcpy_s(PORT, WBCS2MBCS(PORT_NUMBER).c_str());
	//포트포워딩 말고 터널링으로 제공받은 공인 ip(DNS와 매핑 포트(xxx(매핑 포트 사이트가 제공)->25000(서버할당))

	/*채팅을 TCP로 처리->설정 정보 tcp + IPv4 */
	addrinfo hints{};
	hints.ai_family = AF_INET;      // IPv4
	hints.ai_socktype = SOCK_STREAM;  // TCP
	hints.ai_protocol = IPPROTO_TCP;
	addrinfo* res = nullptr;

	//포트 정보 제공
	/* 서버 포트 제공*/
	SOCKADDR_IN svraddr = {};
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(25000); /*서버는 25000만 열거임, 해당 포트 달라고 서버 코드가 요구할 것*/
	svraddr.sin_addr.S_un.S_addr = getaddrinfo(HOST, PORT, &hints, &res);/*서버의 아이피 주소 DNS TO IP */
	
	/*연결 시도*/
	if (::connect(
		m_hSocket,
		(res->ai_addr),
		static_cast<int>(res->ai_addrlen))== SOCKET_ERROR)
	{
		puts("ERROR: 서버에 연결할 수 없습니다.");
		return 0;
	}
	return m_hSocket;
}

