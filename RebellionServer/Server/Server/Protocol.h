#pragma once

#include <iostream>
#include <chrono>
#include <cstring>


//const char* SERVERIP = "127.0.0.1";

#define SERVER_PORT 9000
#define MAX_BUFFER 1024

#define NPC_ID_START 10000


#define CS_LOGIN				1
#define CS_LOGOUT				2	
#define CS_ATTACK				6
#define CS_TRANSFORM			7
#define CS_TELEPORT				8
#define CS_CHAT					9
#define CS_MOUSE_MOVE_PACKET	10
#define CS_POSITION				11
#define CS_DEFENCE				12
#define CS_QUEST_DONE			13
#define CS_INV					14

#define SC_LOGIN_OK			1
#define SC_LOGIN_FAIL		2
#define SC_MOVE_DIRECTION	3
#define SC_ENTER			6
#define SC_LEAVE			7
#define SC_STAT_CHANGE		8
#define SC_CHAT				9
#define SC_NPC_ATTACK		12
#define SC_PLAYER_ATTACK	13
#define SC_HP_CHANGE		14
#define SC_PLAYER_DIE		15
#define SC_DEFENCE			16
#define SC_RETURN			17

constexpr int MAX_PW_LEN = 12;
constexpr int MAX_ID_LEN = 50;
constexpr int MAX_STR_LEN = 50;

constexpr unsigned char PLAYER = 0;

#pragma pack(push,1)

struct sc_packet_login_ok {
	char size;
	char type;
	int id;
	float x, y, z;
	short damage;
	short c_hp, m_hp;
	short level;
	int	c_exp, m_exp;
};

struct sc_packet_login_fail {
	char size;
	char type;
};

struct sc_packet_return {
	char size;
	char type;
	int id;
	unsigned moveTime;
};

struct sc_packet_move_direction {
	char size;
	char type;
	int id;
	int direction;
	float x, y, z;
};

struct sc_packet_player_attack {
	char size;
	char type;
	int id;
};


struct sc_packet_enter {
	char size;
	char type;
	int id;
	char name[MAX_ID_LEN];
	unsigned char objectType;
	float x, y, z;
};

struct sc_packet_leave {
	char size;
	char type;
	int id;
	unsigned char objectType;
};

struct sc_packet_chat {
	char size;
	char type;
	int	 id;
	char name[MAX_STR_LEN];
};

struct sc_packet_stat_change {
	char size;
	char type;
	short level;
	short c_hp;
	short m_hp;
	short c_exp;
	short m_exp;
};

struct sc_packet_npc_die {
	char size;
	char type;
	int id;
	char objectType;
};
struct sc_packet_move {
	char size;
	char type;
	int id;
	float x, y, z;
	float D_x, D_y, D_z;
	char status;
	std::chrono::time_point<std::chrono::system_clock> time;
};

struct sc_packet_stop {
	char size;
	char type;
	int id;
};

struct sc_packet_npc_attack {
	char size;
	char type;
	int id;
	float x;
	//float y;
	float z;
};

struct sc_packet_player_die {
	char size;
	char type;
	int id;
};

struct sc_packet_hp {
	char size;
	char type;
	short c_hp;
};

struct sc_packet_defence {
	char size;
	char type;
	bool kind;
	int id;
};
//---------------------------------------


struct cs_packet_login {
	char	size;
	char	type;
	char	name[MAX_ID_LEN];
	char	pw[MAX_PW_LEN];
	char	loginType;
};

constexpr unsigned char D_UP = 0;
constexpr unsigned char D_DOWN = 1;
constexpr unsigned char D_LEFT = 2;
constexpr unsigned char D_RIGHT = 3;

struct cs_packet_mouse_move {
	char size;
	char type;
	float look[3];
	float right[3];
	float up[3];
};

struct cs_packet_position {
	char size;
	char type;
	float xPos;
	float yPos;
	float zPos;
	float xLook;
	float yLook;
	float zLook;
	unsigned moveTime;
};

struct cs_packet_move {
	char	size;
	char	type;
	char	direction;
};

struct cs_packet_run {
	char	size;
	char	type;
	bool	running;
};

struct cs_packet_quest_done {
	char size;
	char type;
};

struct cs_packet_move_stop {
	char	size;
	char	type;
	char	direction;
};

#pragma pack(pop)