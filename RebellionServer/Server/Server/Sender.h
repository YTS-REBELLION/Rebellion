#pragma once

class CError;

class CSender
{
public:
	CSender();
	~CSender();

public:
	void SendLoginOkPacket(SOCKET s, int id, float xPos, float yPos, float zPos, short damage, short c_hp, short m_hp, short level, short c_exp, short m_exp);
	void SendLoginFailPacket(SOCKET s);
	void SendLeaveObjectPacket(SOCKET s, int id, int objType);
	void SendNPCAttackPacket(SOCKET s, int id, float x, float z);
	void SendMovePacket(SOCKET s, int mover, Vec3 localPos, float dx, float dy, float dz, bool status, std::chrono::time_point<std::chrono::system_clock> time);
	void SendMovePacket(SOCKET s, int mover, Vec3 localPos);

	void SendPutObjectPacket(SOCKET s, int id, float x, float y, float z, int objType);
	void Send_Stop_Packet(SOCKET s, int mover_id );
	void SendPlayerDiePacket(SOCKET s, int id);
	void SendNPCDiePacket(SOCKET s, int id, int type);
	void SendPlayerAttackPacket(SOCKET s, int id);
	void SendTransformPacket(SOCKET s, int id, bool b);
	void SendStatChangePacket(SOCKET s, short level, short currentHp, short maxHp, short currentExp, short maxExp);
	void SendHpPacket(SOCKET s, short currentHp);
	void SendDefencePacket(SOCKET s, int id, bool kind);
	void SendDummyPacket(SOCKET s, int id, unsigned time);
	void Send_Enter_Packet(SOCKET s, Vec3 target_pos, const short& id, const short& other_id);
	void SendRotatePacket(SOCKET s, int mover, Vec3 rotate);
	void Activate_Player_Move_Event(int target, int player_id);
private:
	void SendPacket(SOCKET s, void* buff);

private:
	CError* m_error;
};
