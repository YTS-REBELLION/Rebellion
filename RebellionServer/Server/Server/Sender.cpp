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

void CSender::SendLeaveObjectPacket(SOCKET s, int id, int objType)
{
	////printf("%d번 Leave 보냄 / 타입: %d\n", id, objType);

	sc_packet_leave packet;
	packet.id = id;
	packet.objectType = objType;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LEAVE_OBJECT;

	SendPacket(s, &packet);
}

void CSender::SendNPCAttackPacket(SOCKET s, int id, float x, float z)
{
	sc_packet_npc_attack packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_NPC_ATTACK;
	packet.x = x;
	//packet.yNor = y;
	packet.z = z;

	SendPacket(s, &packet);
}

void CSender::SendMovePacket(SOCKET s, int mover, float x, float y, float z, float dx, float dy, float dz, char status, std::chrono::time_point<std::chrono::system_clock> time)
{
	sc_packet_move packet;
	packet.id = mover;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.D_x = dx;
	packet.D_y = dy;
	packet.D_z = dz;
	packet.time = time;
	packet.size = sizeof(packet);
	packet.status = status;
	packet.type = SC_MOVE;

	SendPacket(s, &packet);
}

void CSender::SendPutObjectPacket(SOCKET s, int id, float x, float y, float z, int objType)
{
	printf("Enter 보냈다\n");
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
//
//void CSender::Send_Enter_Packet(SOCKET s, Vec3 target_pos, const short& id, const short& other_id)
//{
//	sc_packet_put_object p;
//	p.id = other_id;
//	p.size = sizeof(p);
//	p.type = SC_PACKET_PUT_OBJECT;
//	p.localVec = target_pos;
//	//p.localVec = Netmgr.GetMediatorMgr()->Find(other_id)->GetLocalPosVector();
//	//p.RotateY = Netmgr.GetMediatorMgr()->Find(other_id)->GetRoatateVector().y;
//	//strcpy_s(p.name, Netmgr.GetMediatorMgr()->Find(other_id)->GetName());    // data race???
//	p.o_type = 0;
//	//if (p.id >= START_MONSTER && p.id < END_MONSTER)
//	//{
//	//	p.hp = dynamic_cast<CMonster*>(Netmgr.GetMediatorMgr()->Find(other_id))->GetHP();
//
//	//	/*   cout << "********************" << endl;
//	//	   cout << other_id << "가 " << user_id << "에게  Enter Packet 전송" << endl;
//	//	   cout << "Monster POS ->   <" << Netmgr.GetMediatorMgr()->Find(other_id)->GetLocalPosVector().x << ", "
//	//		   << Netmgr.GetMediatorMgr()->Find(other_id)->GetLocalPosVector().z << ">" << endl;
//	//	   cout << "********************" << endl << endl;*/
//	//}
//	p.hp = 100;
//	{
//		cout << "********************" << endl;
//		cout << "********************" << endl;
//		cout << other_id << "가 " << id << "에게 Enter Packet 전송" << endl;
//		cout << "********************" << endl;
//		cout << "********************" << endl;
//	}
//	SendPacket(s, &p);
//}

void CSender::SendStopPacket(SOCKET s, int id)
{
}

void CSender::SendPlayerDiePacket(SOCKET s, int id)
{
	sc_packet_player_die packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_PLAYER_DIE;

	SendPacket(s, &packet);
}

void CSender::SendNPCDiePacket(SOCKET s, int id, int type)
{
}


void CSender::SendPlayerAttackPacket(SOCKET s, int id)
{
	sc_packet_player_attack packet;
	packet.id = id;
	packet.type = SC_PACKET_PLAYER_ATTACK;
	packet.size = sizeof(packet);

	SendPacket(s, &packet);
}

void CSender::SendTransformPacket(SOCKET s, int id, bool b)
{
}



void CSender::SendStatChangePacket(SOCKET s, short level, short currentHp, short maxHp, short currentExp, short maxExp)
{
	sc_packet_stat_change packet;
	packet.type = SC_PACKET_STAT_CHANGE;
	packet.size = sizeof(packet);
	packet.level = level;
	packet.c_hp = currentHp;
	packet.m_hp = maxHp;
	packet.c_exp = currentExp;
	packet.m_exp = maxExp;

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