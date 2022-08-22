#pragma once





//const char* SERVERIP = "127.0.0.1";

const short SERVER_PORT = 9000;

constexpr int MAX_BUFFER = 1024;
constexpr int MAX_PACKET_SIZE = 255;
constexpr short NPC_ID_START = 101;
constexpr short MONSTER_LV1_ID = 104;
constexpr short MONSTER_LV2_ID = 108;
constexpr short MONSTER_LV3_ID = 113;
constexpr short MONSTER_LV4_ID = 118;
constexpr short MONSTER_LV5_ID = 123;
constexpr short MONSTER_LV6_ID = 129;
constexpr short NPC_ID_END = 141;

constexpr int MAX_USER = 100;
//#define NPC_ID_START 10000


const char  CS_PACKET_LOGIN = 1;

const char  CS_PACKET_LOGOUT = 2;
const char  CS_PACKET_ATTACK = 3;
const char  CS_PACKET_TELEPORT = 4;
const char  CS_PACKET_QUEST_DONE = 9;
const char	CS_PACKET_MOVE = 10;
const char	CS_PACKET_STOP = 11;
const char	CS_PACKET_ROTATE = 12;
const char	CS_PACKET_RUN = 13;
const char	CS_PACKET_MD = 14;
const char	CS_PACKET_P2MCOL = 15;
const char	CS_PACKET_MONSTERDIR = 16;
const char	CS_PACKET_DUNGEON = 17;
const char	CS_PACKET_DIETEST = 18;
const char	CS_PACKET_SKILL = 19;
const char	CS_PACKET_BOSSMAP = 20;
const char	CS_PACKET_M2PCOL = 21;


const char  SC_PACKET_LOGIN_OK = 1;
const char  SC_PACKET_LOGIN_FAIL = 2;
const char	SC_PACKET_PLAYER_DIE = 3;

const char  SC_PACKET_PUT_OBJECT = 4;
const char  SC_PACKET_LEAVE_OBJECT = 5;
const char	SC_PACKET_BOSSMAP = 6;
const char	SC_PACKET_MOVE = 8;
const char	SC_PACKET_STOP = 9;
const char	SC_PACKET_PLAYER_ATTACK = 10;
const char	SC_PACKET_RUN = 11;
const char	SC_PACKET_MONSTERDIE = 12;
const char	SC_PACKET_MONSTERDIR = 13;
const char	SC_PACKET_ROTATE = 14;
const char	SC_PACKET_NPC_ATTACK = 15;
const char	SC_PACKET_TARGET = 16;
const char	SC_PACKET_QUESTDONE = 17;
const char	SC_PACKET_QUESTSTART = 18;
const char	SC_PACKET_DUNGEON_ENTER = 19;
const char	SC_PACKET_WAITROOM = 20;
const char	SC_PACKET_SKILL = 21;

const char	SC_PACKET_HP_CHANGE = 10;
const char	SC_PACKET_DEFENCE = 15;
const char	SC_PACKET_RETURN = 13;

constexpr int MAX_PW_LEN = 12;
constexpr int MAX_ID_LEN = 50;
constexpr int MAX_STR_LEN = 50;

constexpr unsigned char PLAYER = 0;
constexpr unsigned char WALK = 0;
constexpr unsigned char RUN = 1;

constexpr unsigned char NUM_OBSTACLES = 50;

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
	bool isAttack;
};


struct sc_packet_put_object {
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
	int playerId;
	int monsterdieCnt;
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

struct sc_packet_npc_die 
{
	char size;
	char type;
	int id;
	char objectType;
};
struct sc_packet_move {
	char size;
	char type;
	int id;
	//float x, y, z;
	Vec3 localPos;
	
	float D_x, D_y, D_z;
	bool status;
	std::chrono::time_point<std::chrono::system_clock> time;
	
	bool isMoving;
};

struct sc_packet_stop {
	char size;
	char type;
	int id;
	bool isMoving;
};

struct sc_packet_npc_attack {
	char size;
	char type;
	int id;
	float x;
	//float y;
	float z;
	bool isAttack;
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
struct sc_packet_rotate {
	char size;
	char type;
	int id;
	Vec3 rotate;
};
struct sc_packet_run {
	char size;
	char type;
	int id;
	bool isRun;
	Vec3 pos;
};

struct sc_packet_monsterdie {
	char size;
	char type;
	int id;
	int playerId;
	
};
struct sc_packet_monsterdir {
	char size;
	char type;
	int id;
	int targetId;
	Vec3 vRot;
};
struct sc_packet_targetplayer {
	char size;
	char type;
	int id;
	bool isTarget;
	int monster_id;
};
struct sc_packet_questdone {
	char size;
	char type;
	bool isDone;
	int id;
	QUEST nextQuest;
};
struct sc_packet_queststart {
	char size;
	char type;
	bool isStart;
	int id;
};
struct sc_packet_dungeon {
	char size;
	char type;
	bool isEnter;
	int id;
};
struct sc_packet_waitroom {
	char size;
	char type;
	size_t enterNum;
};
struct sc_packet_skill {
	char size;
	char type;
	int id;
	bool isSkill;
	PLAYER_ANI_TYPE anitype;
};
struct sc_packet_bossmap {
	char size;
	char type;
	int id;
	bool isEnter;
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
	float	rotate;
	
	Vec3	localPos;
	Vec3	dirVec;
	float	speed;
	float	deltaTime;

	std::chrono::system_clock::time_point start;
	short	moveTime;
	bool	movings;
	
};


struct cs_packet_quest_done {
	char size;
	char type;
};

struct cs_packet_stop {
	char size;
	char type;
	short id;
	bool isMoving;
};
struct cs_packet_rotate {
	char size;
	char type;
	int id;
	Vec3 rotate;
};
struct cs_packet_attack {
	char size;
	char type;
	int id;
	bool isAttack;
};

struct cs_packet_run {
	char size;
	char type;
	int id;
	bool isRun;
	Vec3 pos;
};
struct cs_packet_monsterdie {
	char size;
	char type;
	int id;
	bool isDead;
};
struct cs_packet_player2monstercol {
	char size;
	char type;
	int id;
	int playerId;
	bool iscol;
	int attackType;
};
struct cs_packet_monsterdir {
	char size;
	char type;
	int id;		// 몬스터 아이디
	int playerId; // 플레이어 아이디 
	Vec3 vRot;

};
struct cs_packet_dungeon {
	char size;
	char type;
	bool isEnter;
};
struct cs_packet_bossmap {
	char size;
	char type;
	bool isEnter;
};
struct cs_packet_dietest {
	char size;
	char type;
	int id;
};
struct cs_packet_m2mcol {
	char size;
	char type;
	int monsterId;
	int other_monsterId;
	bool isCol;
	MONSTER_MOVE moveType;

};
struct cs_packet_teleport {
	char size;
	char type;
	int playerId;
	Vec3 localPos;

};
struct cs_packet_skill {
	char size;
	char type;
	int id;
	bool isSkill;
	PLAYER_ANI_TYPE anitype;
};
struct cs_packet_m2p {
	char size;
	char type;
	int playerId;
	bool isCol;
};
#pragma pack(pop)