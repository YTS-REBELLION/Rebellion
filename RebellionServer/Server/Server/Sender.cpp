#include "pch.h"
#include "Sender.h"
#include "pch.h"
#include "Sender.h"
#include "Error.h"
#include "Shared.h"
#include "CServerFrame.h"


CSender::CSender()
{

}

CSender::~CSender()
{

}

// public function
void CSender::SendLoginOkPacket(SOCKET s, int id, float xPos, float yPos, float zPos, short damage, short c_hp, short m_hp, short level, short c_exp, short m_exp)
{

	sc_packet_login_ok packet;
	packet.id = id;
	packet.type = SC_PACKET_LOGIN_OK;

	packet.x = xPos;
	packet.y = yPos;
	packet.z = zPos;
	packet.damage = damage;
	packet.c_hp = c_hp;
	packet.m_hp = m_hp;
	packet.level = level;
	packet.c_exp = c_exp;
	packet.m_exp = m_exp;
	packet.size = sizeof(packet);
	SendPacket(s, &packet);
}

void CSender::SendLoginFailPacket(SOCKET s)
{
	sc_packet_login_fail packet;

	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_FAIL;

	SendPacket(s, &packet);
}

void CSender::SendLeaveObjectPacket(SOCKET s, int playerId,int monsterid, int monsterdieCnt)
{
	////printf("%d¹ø Leave º¸³¿ / Å¸ÀÔ: %d\n", id, objType);

	sc_packet_leave packet;
	packet.id = monsterid;
	packet.playerId = playerId;
	packet.monsterdieCnt = monsterdieCnt;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LEAVE_OBJECT;

	SendPacket(s, &packet);
}


void CSender::SendLeaveObjectPacket(SOCKET s, int playerId)
{
	sc_packet_leave packet;
	packet.playerId = playerId;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LEAVE_OBJECT;

	SendPacket(s, &packet);
}


void CSender::SendNPCAttackPacket(SOCKET s, int id, float x, float z,bool isAttack)
{
	sc_packet_npc_attack packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_NPC_ATTACK;
	packet.x = x;
	//packet.yNor = y;
	packet.z = z;
	packet.isAttack = isAttack;
	SendPacket(s, &packet);
}

void CSender::SendMovePacket(SOCKET s, int mover,Vec3 localPos, float dx, float dy, float dz, bool status, std::chrono::time_point<std::chrono::system_clock> time)
{
	sc_packet_move packet;
	packet.id = mover;
	packet.localPos = localPos;

	packet.D_x = dx;
	packet.D_y = dy;
	packet.D_z = dz;
	packet.time = time;
	packet.size = sizeof(packet);
	packet.status = status;
	packet.type = SC_PACKET_MOVE;
	SendPacket(s, &packet);
}

void CSender::SendMovePacket(SOCKET s,int mover, Vec3 localPos)
{
	sc_packet_move packet;
	packet.id = mover;
	packet.localPos = localPos;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MOVE;
	SendPacket(s, &packet);


}

void CSender::SendPutObjectPacket(SOCKET s, int id, float x, float y, float z, int objType)
{
	printf("Enter º¸³Â´Ù\n");
	sc_packet_put_object packet;
	packet.id = id;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.size = sizeof(packet);
	packet.objectType = objType;
	packet.type = SC_PACKET_PUT_OBJECT;

	SendPacket(s, &packet);
}
void CSender::Send_Stop_Packet(SOCKET s, int mover_id)
{
	sc_packet_stop p;
	p.size = sizeof(p);
	p.type = SC_PACKET_STOP;
	p.id = mover_id;
	
	SendPacket(s, &p);

}

void CSender::SendPlayerDiePacket(SOCKET s, int id)
{
	cout << id << "ÇÃ·¹ÀÌ¾î »ç¸Á" << endl;
	sc_packet_player_die packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_PLAYER_DIE;

	SendPacket(s, &packet);
}

void CSender::SendNPCDiePacket(SOCKET s, int id, int type)
{
}


void CSender::SendPlayerAttackPacket(SOCKET s, int attacker, bool isAttack)
{
	sc_packet_player_attack packet;
	packet.id = attacker;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_PLAYER_ATTACK;
	packet.isAttack = isAttack;

	SendPacket(s, &packet);
}


void CSender::SendHpPacket(SOCKET s, short currentHp)
{
	sc_packet_hp packet;
	packet.type = SC_PACKET_HP_CHANGE;
	packet.size = sizeof(packet);
	packet.c_hp = currentHp;

	SendPacket(s, &packet);
}

void CSender::SendDefencePacket(SOCKET s, int id, bool kind)
{
	sc_packet_defence packet;
	packet.type = SC_PACKET_DEFENCE;
	packet.size = sizeof(packet);
	packet.id = id;
	packet.kind = kind;

	SendPacket(s, &packet);
}

void CSender::SendDummyPacket(SOCKET s, int id, unsigned time)
{
	sc_packet_return packet;
	packet.type = SC_PACKET_RETURN;
	packet.size = sizeof(packet);
	packet.id = id;
	packet.moveTime = time;

	SendPacket(s, &packet);
}

void CSender::SendRotatePacket(SOCKET s, int mover, Vec3 rotate)
{
	sc_packet_rotate packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_ROTATE;
	packet.id = mover;
	packet.rotate = rotate;

	SendPacket(s, &packet);

}

void CSender::SendRunPacket(SOCKET s, int id,Vec3 pos, bool isRun)
{
	sc_packet_run packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_RUN;
	packet.isRun = isRun;
	packet.pos = pos;
	SendPacket(s, &packet);
}

void CSender::SendMonsterDiePacket(SOCKET s, const int& playerId,const int& monsterid)
{
	sc_packet_monsterdie packet;
	packet.id = monsterid;
	packet.type = SC_PACKET_MONSTERDIE;
	packet.playerId = playerId;
	packet.size = sizeof(packet);
	SendPacket(s, &packet);


}

void CSender::SendMonsterDirPacket(SOCKET s, const int& targetId, const int& id, Vec3 vRot)
{
	sc_packet_monsterdir packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.targetId = targetId;
	packet.type = SC_PACKET_MONSTERDIR;
	packet.vRot = vRot;

	SendPacket(s, &packet);


}

void CSender::SendTargetPlayerPacket(SOCKET s, const int& id, bool isTarget , const int& monster_id)
{
	sc_packet_targetplayer packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_TARGET;
	packet.isTarget = isTarget;
	packet.monster_id = monster_id;
	SendPacket(s, &packet);

}

void CSender::SendQuestDonePacket(SOCKET s, int playerId, QUEST nextQuest, bool isDone)
{
	sc_packet_questdone packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_QUESTDONE;
	packet.isDone = isDone;
	packet.id = playerId;
	packet.nextQuest = nextQuest;
	SendPacket(s, &packet);

}

void CSender::SendDungeonEnterPacket(SOCKET s, const int& playerId, bool isEnter)
{
	sc_packet_dungeon packet;
	packet.size = sizeof(packet);
	packet.id = playerId;
	packet.type = SC_PACKET_DUNGEON_ENTER;
	packet.isEnter = isEnter;

	SendPacket(s, &packet);
}

void CSender::Send_WaitRoom_Packet(SOCKET s, size_t enterNum)
{
	sc_packet_waitroom packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_WAITROOM;
	packet.enterNum = enterNum;
	SendPacket(s, &packet);

}


void CSender::SendQuestStartPacket(SOCKET s, int id, bool isStart)
{
	sc_packet_queststart packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_QUESTSTART;
	packet.isStart = isStart;
	packet.id = id;
	SendPacket(s, &packet);
}

void CSender::SendSkillPacket(SOCKET s, const int& id, PLAYER_ANI_TYPE anitype, bool isSkill)
{
	sc_packet_skill packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_SKILL;
	packet.id = id;
	packet.anitype = anitype;
	packet.isSkill = isSkill;
	SendPacket(s, &packet);

}

void CSender::SendBossMapPacket(SOCKET s, const int& playerId, bool isEnter)
{
	sc_packet_bossmap packet;
	packet.size = sizeof(packet);
	packet.id = playerId;
	packet.type = SC_PACKET_BOSSMAP;
	packet.isEnter = isEnter;

	SendPacket(s, &packet);


}



//private function
void CSender::SendPacket(SOCKET s, void* buff)
{
	char* packet = reinterpret_cast<char*>(buff);

	int packet_size = packet[0];
	EXP_OVER* send_over = new EXP_OVER;
	ZeroMemory(&send_over->over, sizeof(EXP_OVER));

	send_over->event_type = OP_SEND;
	memcpy(send_over->net_buf, packet, packet_size);
	send_over->wsabuf.buf = send_over->net_buf;
	send_over->wsabuf.len = packet_size;
	int ret = WSASend(s, &send_over->wsabuf, 1, 0, 0, &send_over->over, 0);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			m_error->error_display("WSASend Error :", err_no);
	}
}