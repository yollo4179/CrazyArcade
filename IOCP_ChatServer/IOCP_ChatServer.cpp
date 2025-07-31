// IOCP_ChatServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>   // 필요하면
#pragma comment(lib, "ws2_32")


#include<unordered_map>
#include <iterator>

#include"ChatProtocols.h"

typedef struct _USERSESSION
{
	SOCKET	hSocket;
	char	buffer[8192] = {};	//8KB
	int   SESSION_ID = {-1};

} USERSESSION;

/////////////////////////////////////////////////////////////////////////
//클라이언트 처리를 위한 작업자 스레드 개수.
#define MAX_THREAD_CNT		4
const int g_SESSION_ID_LIMIT = 4;
PLAYER_INFO g_PlayerInfo[g_SESSION_ID_LIMIT];
void ClearPlayerInfo(PLAYER_INFO& pInfo) { memset(&pInfo, 0, sizeof(PLAYER_INFO)); pInfo.CharacterType = -1; pInfo.SSESION_ID = -1; };

CRITICAL_SECTION  g_cs;				//스레드 동기화 객체
std::unordered_map<int, SOCKET>	g_mapClient;	//연결된 클라이언트 소켓 리스트. 
SOCKET	g_hSocket;					//서버의 리슨 소켓
HANDLE	g_hIocp;					//IOCP 핸들


/////////////////////////////////////////////////////////////////////////
//연결된 클라이언트 모두에게 메시지를 전송한다.
void SendMessageAll(const char *pszMessage, int nSize)
{
	std::unordered_map<int, SOCKET>::iterator it;

	::EnterCriticalSection(&g_cs);
	for (it = g_mapClient.begin(); it != g_mapClient.end(); ++it)
		::send((*it).second, pszMessage, nSize, 0);
	::LeaveCriticalSection(&g_cs);
}
int  FindUsableSessionID() /* 소켓과 함께 counting으로 사용 가능한 키 찾기 */
{
	int iCount = -1;
	auto iter = g_mapClient.begin();
	do
	{
		iter = g_mapClient.find(++iCount);

	} while (iter != g_mapClient.end() );
	
	if (g_SESSION_ID_LIMIT == iCount) //4이상은 삽입하지 않고 거부
		return -1;

	return iCount;

}

/////////////////////////////////////////////////////////////////////////
//연결된 모든 클라이언트 및 리슨 소켓을 닫는다.
void CloseAll()
{
	std::unordered_map<int, SOCKET>::iterator it;

	::EnterCriticalSection(&g_cs);
	for (it = g_mapClient.begin(); it != g_mapClient.end(); ++it)
	{
		::shutdown((*it).second, SD_BOTH);
		::closesocket((*it).second);
	}
	::LeaveCriticalSection(&g_cs);
}

/////////////////////////////////////////////////////////////////////////
void CloseClient(_uint _SESSION_ID ) //Client_Connection_Socket //클라이언트와의 연결을 끊고 목록에서 삭제한다.
{
	::shutdown(g_mapClient[_SESSION_ID], SD_BOTH);
	::closesocket(g_mapClient[_SESSION_ID]);
	::EnterCriticalSection(&g_cs);
	ClearPlayerInfo(g_PlayerInfo[_SESSION_ID]);
	g_mapClient.erase(_SESSION_ID);
	::LeaveCriticalSection(&g_cs);
}

/////////////////////////////////////////////////////////////////////////
void ReleaseServer(void) //서버 닫는 코드 
{
	//클라이언트 연결을 모두 종료한다.
	CloseAll();
	::Sleep(500);

	//Listen 소켓을 닫는다.
	::shutdown(g_hSocket, SD_BOTH);
	::closesocket(g_hSocket);
	g_hSocket = NULL;

	//IOCP 핸들을 닫는다. 이렇게 하면 GQCS() 함수가 FALSE를 반환하며
	//:GetLastError() 함수가 ERROR_ABANDONED_WAIT_0을 반환한다.
	//IOCP 스레드들이 모두 종료된다.
	::CloseHandle(g_hIocp);
	g_hIocp = NULL;

	//IOCP 스레드들이 종료되기를 일정시간 동안 기다린다.
	::Sleep(500);
	::DeleteCriticalSection(&g_cs);
}

/////////////////////////////////////////////////////////////////////////
//Ctrl+C 이벤트를 감지하고 프로그램을 종료한다.
BOOL CtrlHandler(DWORD dwType)
{
	if (dwType == CTRL_C_EVENT)
	{
		ReleaseServer();

		_putws(L"*** 채팅서버를 종료합니다! ***");
		::WSACleanup();
		exit(0);
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
DWORD WINAPI WorkingThread(LPVOID pParam)//스레드를OS 가 꺠운다. 
{
	DWORD			dwTransferredSize = 1;
	DWORD			dwFlag = 0;
	USERSESSION		*pSession = NULL;
	LPWSAOVERLAPPED	pWol = NULL;
	BOOL			bResult =false;

	_putws(L"[IOCP 작업자 스레드 시작]");
	while (1)
	{
		//이거 요청하고 받은 데이터가 없으면 이 스레드는 Alertable Sleep 상태로 돌아간다 
		//(아무거나 소켓 메시지오면 os 가깨워줌) 처음 실행되면 받을게 없으므로 바로 alertablewait으로 빠지는것임)

		bResult = ::GetQueuedCompletionStatus( 
			g_hIocp,				//Dequeue할 IOCP 핸들.
			&dwTransferredSize,		//수신한 데이터 크기.
			(PULONG_PTR)&pSession,	//수신된 데이터가 저장된 메모리 (소켓이랑 데이터랑)
			&pWol,					//OVERLAPPED 구조체.
			INFINITE);				//이벤트를 무한정 대기.
		//Accepted되서 IOCP에 등록된 소켓들 중 아무거나 연결된 소켓으로부터 메시지가 오면 깨어난다.
		//  등록된 소켓 정보와 받은 세션 정보 (받을 데이터)가 들어있을것 ( 동적할당 해놓은)   
		if (bResult == TRUE)
		{
			//정상적인 경우.

			//1. 클라이언트가 소켓을 정상적으로 닫고 연결을 끊은 경우.
			if (dwTransferredSize == 0)
			{
				/*To do 연결 종료 메시지 뿌리고 종료 */
				/*1*/
				{
					/*종료 전에 업데이트 한번*/
					BYTE PCK[sizeof(PROTOCOL_HEADER) + sizeof(g_PlayerInfo)+(sizeof(wchar_t)* MESSAGESIZE) +sizeof(XMFLOAT4)] = {};
					/*SubHeader*/
					/*다른 플레이어의 정보도 함께 보내기*/
					g_PlayerInfo[pSession->SESSION_ID].IsDead = true;
					PROTOCOL_HEADER* pCmd = (PROTOCOL_HEADER*)(PCK);
					pCmd->Protocol = { (_uint)PROTOCOL_CODE::SND_PLAYERS_INFO };
					pCmd->NextFileSize = { sizeof(g_PlayerInfo) };
					PLAYER_INFO* pPlayerInfo = (PLAYER_INFO*)((BYTE*)pCmd + sizeof(PROTOCOL_HEADER));
					memcpy(pPlayerInfo, &g_PlayerInfo[0], sizeof(g_PlayerInfo));
					

					wchar_t* pMessage = (wchar_t*)((BYTE*)pPlayerInfo + sizeof(g_PlayerInfo));
					wstring ExitMessage = wstring(g_PlayerInfo[pSession->SESSION_ID].Account)+wstring(L"님이 나갔습니다.");
					memcpy(pMessage, ExitMessage.c_str(), sizeof(wchar_t) * MESSAGESIZE);
					
					XMFLOAT4* pColor = (XMFLOAT4*)((BYTE*)pMessage + sizeof(wchar_t) * MESSAGESIZE);
					XMFLOAT4 ColorRed = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
					memcpy(pColor, &ColorRed, sizeof(XMFLOAT4));

					
					SendMessageAll(reinterpret_cast<const char*>(PCK), 
						sizeof(PROTOCOL_HEADER) + sizeof(g_PlayerInfo) 
						+ (sizeof(wchar_t) * MESSAGESIZE) + sizeof(XMFLOAT4));
				}
			
				CloseClient(pSession->SESSION_ID);
				delete pWol;
				delete pSession;
				_putws(L"\tGQCS: 클라이언트가 정상적으로 연결을 종료함.");
			}

			//2. 클라이언트가 보낸 데이터를 수신한 경우.
			else
			{
				/*모든 클라이언트는 (페이로드)데이터를 보내기전에 헤더(프로토콜 정보)를 보낸다.*/
				
				dwTransferredSize;

				BYTE* pSeeker = (BYTE*)pSession->buffer; 
				/* 1. 프로토콜 코드 받는 작업 먼저 수행 */
				PROTOCOL_HEADER ProtocolDesc;  // *(PROTOCOL_HEADER*)(pSeeker));

				memcpy(&ProtocolDesc, pSeeker, sizeof(PROTOCOL_HEADER));
				

				switch (ProtocolDesc.Protocol)
				{
					/*요청만 보낸다  Seeker는 더이상 정보가 없다. */
				case (_uint)PROTOCOL_CODE::REQ_LOGIN:
				{
					/*Session ID 보내기 */
					BYTE PCK[ sizeof(PROTOCOL_HEADER) + sizeof(AccountID) + sizeof(PROTOCOL_HEADER) + sizeof(g_PlayerInfo) ] = {};
					memset(PCK, 0, sizeof(PROTOCOL_HEADER) + sizeof(AccountID) + sizeof(PROTOCOL_HEADER) + sizeof(g_PlayerInfo));
					
					PROTOCOL_HEADER* pCmd = (PROTOCOL_HEADER*)PCK;
					pCmd->Protocol = { (_uint)PROTOCOL_CODE::ACK_LOGIN };
					pCmd->NextFileSize = { sizeof(AccountID) };
					
			
					AccountID* pSessionID = (AccountID*)(PCK + sizeof(PROTOCOL_HEADER));
					pSessionID->SSESION_ID = pSession->SESSION_ID;
					/*내가 부여받은 세션 아이디 보내기 */
					{
						/*SubHeader*/
						/*다른 플레이어의 정보도 함께 보내기*/
						PROTOCOL_HEADER* pCmd = (PROTOCOL_HEADER*)((BYTE*)pSessionID + sizeof(AccountID));
						pCmd->Protocol = { (_uint)PROTOCOL_CODE::SND_PLAYERS_INFO };
						pCmd->NextFileSize = { sizeof(g_PlayerInfo) };

						PLAYER_INFO* pPlayerInfo = (PLAYER_INFO*)((BYTE*)pCmd + sizeof(PROTOCOL_HEADER));
						memcpy(pPlayerInfo, &g_PlayerInfo[0], sizeof(g_PlayerInfo));

						::send(pSession->hSocket, (const char*)PCK, sizeof(PROTOCOL_HEADER) + sizeof(AccountID)+ sizeof(PROTOCOL_HEADER) + sizeof(g_PlayerInfo), 0);

					}
					//::send(pSession->hSocket,(const char*)PCK, sizeof(PROTOCOL_HEADER) +sizeof(AccountID), 0);

					/*연결된 소켓에게만 보내기*/
					break;

				}

				case (_uint)PROTOCOL_CODE::SND_MESSAGE:
				{
					pSeeker += sizeof(PROTOCOL_HEADER);
					/*ToSend*/
					BYTE PCK[sizeof(PROTOCOL_HEADER) + sizeof(CHAT_LOG)] = {};
					
						PROTOCOL_HEADER* pCmd = (PROTOCOL_HEADER*)PCK;
						pCmd->Protocol = { (_uint)PROTOCOL_CODE::BROAD_CAST_ALL };
						pCmd->NextFileSize = { sizeof(CHAT_LOG) };
						
						CHAT_LOG* ChatLog = (CHAT_LOG*)(PCK + sizeof(PROTOCOL_HEADER));
						memcpy(ChatLog, pSeeker, sizeof(CHAT_LOG)); /*Seeker부터 CHAT Log만큼*/
					
						/*해당 플레이어 업데이트 동기화  실제 게임 플레이 중에 쓰일 정보?(서버 처리?)*/
						{
							memcpy(&g_PlayerInfo[ChatLog->SSESION_ID].Account
								, ChatLog->Account,
								sizeof(g_PlayerInfo[ChatLog->SSESION_ID].Account));
							g_PlayerInfo[ChatLog->SSESION_ID].CharacterType = ChatLog->CharacterType;
							g_PlayerInfo[ChatLog->SSESION_ID].SSESION_ID = ChatLog->SSESION_ID;
						}

						SendMessageAll(reinterpret_cast<const char*>(PCK), sizeof(PCK));
						
					//SendMessageAll(pSession->buffer, dwTransferredSize); //그대로 보내는게 아니라 헤더 붙여서 보낸다. 
					break;
				}
				}
	
				//Message 보냈으면 다시 초기화
				memset(pSession->buffer, 0, sizeof(pSession->buffer));

				//다시 IOCP에 등록.
				DWORD dwReceiveSize	= 0;
				DWORD dwFlag		= 0;
				WSABUF wsaBuf		= { 0 };
				wsaBuf.buf = pSession->buffer;
				wsaBuf.len = sizeof(pSession->buffer);
				//queue 에서 연락 못받는다. (등록을 Recv 걸어서 운영체제에서 io수신 처리하게 한다음 queue에서 꺼내야함)   
				::WSARecv(
					pSession->hSocket,	//클라이언트 소켓 핸들
					&wsaBuf,			//WSABUF 구조체 배열의 주소
					1,					//배열 요소의 개수
					&dwReceiveSize,
					&dwFlag,
					pWol,
					NULL);
				if (::WSAGetLastError() != WSA_IO_PENDING)
					_putws(L"\tGQCS: ERROR: WSARecv()");
			}
		}
		else
		{
			//비정상적인 경우.

			/////////////////////////////////////////////////////////////
			//3. 완료 큐에서 완료 패킷을 꺼내지 못하고 반환한 경우.
			if (pWol == NULL)
			{
				//IOCP 핸들이 닫힌 경우(서버를 종료하는 경우)도 해당된다.
				_putws(L"\tGQCS: IOCP 핸들이 닫혔습니다.");
				break;
			}

			/////////////////////////////////////////////////////////////
			//4. 클라이언트가 비정상적으로 종료됐거나
			//   서버가 먼저 연결을 종료한 경우.
			else
			{
				if (pSession != NULL)
				{
					CloseClient(pSession->hSocket);
					delete pWol;
					delete pSession;
				}

				_putws(L"\tGQCS: 서버 종료 혹은 비정상적 연결 종료");
			}
		}
	}

	_putws(L"[IOCP 작업자 스레드 종료]");
	return 0;
}

/////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadAcceptLoop(LPVOID pParam) 
{
	LPWSAOVERLAPPED	pWol = NULL;
	DWORD			dwReceiveSize, dwFlag;
	USERSESSION		*pNewUser;
	int				nAddrSize = sizeof(SOCKADDR);
	WSABUF			wsaBuf;
	SOCKADDR		ClientAddr;
	SOCKET			hClient;
	int				nRecvResult = 0;

	while ((hClient = ::accept(g_hSocket,
						&ClientAddr, &nAddrSize)) != INVALID_SOCKET)
	{
		
		::EnterCriticalSection(&g_cs);
		
		int _SESSION_ID = FindUsableSessionID();
		if (0 > _SESSION_ID) {
			
			_putws(L"멤버가 가득 찼습니다.");
		}
		g_mapClient.emplace(_SESSION_ID,  hClient);
		::LeaveCriticalSection(&g_cs);

		_putws(L"새 클라이언트가 연결됐습니다.");
		//새 클라이언트에 대한 세션 객체 생성
		pNewUser = new USERSESSION;
		::ZeroMemory(pNewUser, sizeof(USERSESSION));
		pNewUser->hSocket = hClient;
		pNewUser->SESSION_ID = _SESSION_ID;

		//비동기 수신 처리를 위한 OVERLAPPED 구조체 생성.
		pWol = new WSAOVERLAPPED;
		::ZeroMemory(pWol, sizeof(WSAOVERLAPPED));

		//(연결된) 클라이언트 소켓 핸들을 IOCP에 연결.
		::CreateIoCompletionPort( (HANDLE)hClient, g_hIocp,
			(ULONG_PTR)pNewUser,		//KEY!!!
			0);
		/*수신 대기*/
		dwReceiveSize = 0;
		dwFlag = 0;
		wsaBuf.buf = pNewUser->buffer;
		wsaBuf.len = sizeof(pNewUser->buffer);

		//클라이언트가 보낸 정보를 비동기 수신한다.
		nRecvResult = ::WSARecv(hClient, &wsaBuf, 1, &dwReceiveSize, //수신 등록 메시지를IOCP Queue Buffer에 넣는다 . 동적 할당한 메모리에 락걸어서 접근 못하게 함 
							&dwFlag, pWol, NULL);
		if (::WSAGetLastError() != WSA_IO_PENDING)
			_putws(L"ERROR: WSARecv() != WSA_IO_PENDING");
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[]) //iNITIALZE 할때 한번만 호출
{
	
	// 1. 콘솔 코드 페이지를 UTF-8로 (65001)
	SetConsoleOutputCP(CP_UTF8);
	// 2. 로케일 설정
	setlocale(LC_ALL, "");
	//윈속 초기화
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		_putws(L"ERROR: 윈속을 초기화 할 수 없습니다.");
		return 0;
	}

	//임계영역객체를 생성한다.
	::InitializeCriticalSection(&g_cs);

	//Ctrl+C 키를 눌렀을 때 이를 감지하고 처리할 함수를 등록한다.
	if (::SetConsoleCtrlHandler(
			(PHANDLER_ROUTINE)CtrlHandler, TRUE) == FALSE)
		_putws(L"ERROR: Ctrl+C 처리기를 등록할 수 없습니다.");

	//IOCP 생성
	g_hIocp = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,	//연결된 파일 없음.
		NULL,			//기존 핸들 없음.
		0,				//식별자(Key) 해당되지 않음.
		0);				//스레드 개수는 OS에 맡김.
	if (g_hIocp == NULL)
	{
		_putws(L"ERORR: IOCP를 생성할 수 없습니다.");
		return 0;
	}

	//IOCP 스레드들 생성
	HANDLE hThread;
	DWORD dwThreadID;
	for (int i = 0; i < MAX_THREAD_CNT; ++i)
	{
		dwThreadID = 0;
		//클라이언트로부터 문자열을 수신함.
		hThread = ::CreateThread(NULL,	//보안속성 상속
			0,				//스택 메모리는 기본크기(1MB)
			WorkingThread,	//스래드로 실행할 함수이름
			(LPVOID)NULL,	//
			0,				//생성 플래그는 기본값 사용
			&dwThreadID);	//생성된 스레드ID가 저장될 변수주소

		::CloseHandle(hThread);//핸들 닫고 refcnt 2->1 
	}

	//서버 리슨 소켓 생성
	g_hSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,
						NULL, 0, WSA_FLAG_OVERLAPPED);

	//bind()/listen()
	SOCKADDR_IN addrsvr;
	addrsvr.sin_family = AF_INET;
	addrsvr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
	addrsvr.sin_port = ::htons(25000);

	if (::bind(g_hSocket,
			(SOCKADDR*)&addrsvr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		_putws(L"ERROR: 포트가 이미 사용중입니다.");
		ReleaseServer();
		return 0;
	}

	if (::listen(g_hSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		_putws(L"ERROR:  리슨 상태로 전환할 수 없습니다.");
		ReleaseServer();
		return 0;
	}

	//반복해서 클라이언트의 연결을 accept() 한다.
	hThread = ::CreateThread(NULL, 0, ThreadAcceptLoop,
					(LPVOID)NULL, 0, &dwThreadID);
	::CloseHandle(hThread);

	//_tmain() 함수가 반환하지 않도록 대기한다.
	_putws(L"*** 채팅서버를 시작합니다! ***");
	while (1)
		getchar();

	return 0;
}


