#pragma once
#include <wchar.h>
#include<locale.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include<iostream>
#include <cstdlib>
using namespace std;
using namespace DirectX;
#pragma	comment(lib, "d3d11.lib")

#pragma execution_character_set("utf-8")
using _uint = unsigned int;
const int MESSAGESIZE = 512;
const int ACCOUUNTSIZE = 128;
enum class PROTOCOL_CODE
{
	/*클라이언트- > 서버 */
	REQ_LOGIN =0x10,
	SND_MESSAGE =0x20,
	/*서버 -> 클라이언트*/
	SND_PLAYERS_INFO =0X25,
	ACK_LOGIN =0x30,
	BROAD_CAST_ALL=0x40,
	PROTOCOL_END
};

typedef struct ChatLog
{
	int SSESION_ID = {0};
	int CharacterType = {0};
	wchar_t Account[ACCOUUNTSIZE] = {};
	wchar_t Message[MESSAGESIZE] = {};
	XMFLOAT4 vColor = { 1.f,1.f ,1.f ,1.f }; /*white*/
} CHAT_LOG;

/*Chatting Info 제외한  */
typedef struct PlayersInfo
{
	int SSESION_ID = { -1 };
	int CharacterType = { -1 };
	bool IsDead = { false };
	wchar_t Account[ACCOUUNTSIZE] = { };
}PLAYER_INFO;
typedef struct ProtocolHeader
{
	int Protocol = {0};
	size_t		NextFileSize = {0};
}PROTOCOL_HEADER;

typedef struct AccountID
{
	int SSESION_ID = {0};
}ACCOUNT_ID;