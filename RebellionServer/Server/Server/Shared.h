#pragma once
#define MASTER 99'999'999
#define VEC3_TOWN_ENTRANCE_POS Vec3(-3000.f, 0.f, 5683.f)

constexpr float VIEW_RADIUS = 2500.f;
enum class OBJECT_TYPE { PLAYER, MONSTER, NPC };
enum EV_TYPE { OP_RECV, OP_SEND, OP_ACCEPT, OP_PLAYER_MOVE, EV_BROADCAST, EV_ATTACK, EV_RESPAWN, EV_TRANSFORM, EV_TRANSFORM_COOLDOWNTIME, EV_HEAL };
enum STATUS { ST_FREE,ST_INGAME, ST_ACTIVE, ST_SLEEP };
enum DIRECTION_TYPE { RIGHT, UP, FRONT };
enum MOVE_TYPE { TARGET, RANDOM };
enum class OBJECT_KIND
{
	

};

struct OBSTACLES {
	float xPos;
	float yPos;
	float zPos;
	float xScale;
	float yScale;
	float zScale;
};

struct EVENT {
	int obj_id;
	std::chrono::high_resolution_clock::time_point wakeup_time;
	EV_TYPE event_type;
	int target_obj;

	constexpr bool operator < (const EVENT& left) const
	{
		return wakeup_time > left.wakeup_time;
	}
};

struct EXP_OVER {
	WSAOVERLAPPED over;
	
	char net_buf[MAX_BUFFER];
	//COMP_OP		comp_op;
	EV_TYPE		event_type;
	union {
		WSABUF wsabuf;
		SOCKET c_sock;
	};
};

struct DB_INFORM {
	int level;
	int exp;
	int hp;
};

struct OBSTACLE {
	float xPos;
	float yPos;
	float zPos;
	float xScale;
	float yScale;
	float zScale;
};



