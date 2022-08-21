#pragma once



#include "Shared.h"


class CObject
{
	SOCKET			_socket;

	int				_id;
	char			_name[MAX_ID_LEN];

	//ÁÂÇ¥

	Vec3			_pos;
	Vec3			_look;
	Vec3			_up;
	Vec3			_right;

	float			_speed;

	short			_currentHp, _maxHp;
	short			_damage;
	short			_currentExp, _maxExp;
	short			_level;

	std::mutex		_clientLock;

	int				_prevSize;

	int				_myType;

	bool			_isMove;

	int				_walkStatus;

	bool			_moveDirection[4];


	std::unordered_set<int> _viewLists;
	unordered_set<int>		_dungeonViewLists;
	unordered_set<int>		_bossViewLists;
	bool			_isAttack;
	
	bool			_isHeal;

	bool			_isDefence;

	bool			_isDummy;

	bool			_isNear;
	//NPC
	MONSTER_MOVE	_isMonsterMove;

	bool			_isTarget;

	MOVE_TYPE		_moveType;
	int				_targetID;

	int				_nextPosIndex;

	Vec3			_nextPos[3];
	
	bool			_dungeonEnter = false;
	bool			_bossmapEnter = false;

	unordered_set<int> _dungeonViewList;
	bool			_m2mCol;




public:
	std::atomic<STATUS> _status = ST_FREE;
	EXP_OVER _recvOver;
	char _packetBuf[MAX_PACKET_SIZE];
	bool			_questStart = false;
	bool _objectsDie = false;


	bool			_closed = false;
	bool			_move = true;
public:
	CObject();
	~CObject();

public:
	SOCKET GetSocket() const { return _socket; }
	void SetSocket(SOCKET s) { _socket = s; }

	int GetID() const { return _id; }
	void SetID(int id) { _id = id; }

	void SetMyName(const char* name) { strcpy_s(_name, name);}
	void GetMyName(char*& des) { des = _name; }

	void ClientLock() { _clientLock.lock(); }
	void ClientUnLock() { _clientLock.unlock(); }
	// ÁÂÇ¥ Get Set
	Vec3 GetPos() const { return _pos; }
	void SetPos(Vec3 pos) { _pos = pos; }

	Vec3 GetLook() const { return _look; }
	void SetLook(Vec3 l) { _look = l; }


	//----------------

	float GetSpeed() const { return _speed; }
	void SetSpeed(float speed) { _speed = speed; }

	short GetCurrentHp() const { return _currentHp; }
	void SetCurrentHp(short hp) { _currentHp = hp; }

	short GetMaxHp() const { return _maxHp; };
	void SetMaxHp(short hp) { _maxHp = hp; };

	short GetDamage() const { return _damage; }
	void SetDamage(short damage) { _damage = damage; }

	short GetCurrentExp() const { return _currentExp; }
	void SetCurrentExp(short exp) { _currentExp = exp; }

	short GetMaxExp() const { return _maxExp; }
	void SetMaxExp(short exp) { _maxExp = exp; }

	short GetLevel() const { return _level; }
	void SetLevel(short lv) { _level = lv; }

	int GetPrevSize() const { return _prevSize; }
	void SetPrevSize(int size) { _prevSize = size; }

	int GetMyType() const { return _myType; }
	void SetMyType(int type) { _myType = type; }

	bool GetIsMove() const { return _isMove; }
	void SetIsMove(bool b) { _isMove = b; }

	int GetWalkStatus() const { return _walkStatus; }
	void SetWalkStatus(int s) { _walkStatus = s; }

	void SetMoveDirection(int direction, bool b) { _moveDirection[direction] = b; }
	bool GetMoveDirection(int direction) { return _moveDirection[direction]; }


	void InsertViewList(int id) { 
		_viewLists.insert(id); 
	}
	void EraseViewList(int id) { 
		_viewLists.erase(id); 
	}
	void ClearViewList() { _viewLists.clear(); }
	size_t GetViewListCount(int id) const { return _viewLists.count(id); }
	std::unordered_set<int> GetViewList() const { return _viewLists; }
	void SetViewList(unordered_set<int> vl) { _viewLists = vl; }



	unordered_set<int> DungeonGetViewList() const { return _dungeonViewLists; }
	size_t DungeonGetViewListCount(int id) const { return _dungeonViewLists.count(id); }
	void DungeonClearViewList() { _dungeonViewLists.clear(); }
	void DungeonInsertViewList(int id) {
		_dungeonViewLists.insert(id);
	}
	void DungeonEraseViewList(int id) {
		_dungeonViewLists.erase(id);
	}

	unordered_set<int> BossMapGetViewList() const { return _bossViewLists; }
	size_t BossMapGetViewListCount(int id) const { return _bossViewLists.count(id); }
	void BossMapClearViewList() { _bossViewLists.clear(); }
	void BossMapInsertViewList(int id) {
		_bossViewLists.insert(id);
	}
	void BossMapEraseViewList(int id) {
		_bossViewLists.erase(id);
	}

	bool GetIsAttack() const { return _isAttack; }
	void SetIsAttack(bool b) { _isAttack = b; }

	bool GetIsHeal() const { return _isHeal; }
	void SetIsHeal(bool b) { _isHeal = b; }


	bool GetIsDefence() const { return _isDefence; }
	void SetIsDefence(bool b) { _isDefence = b; }

	bool GetIsDummy() const { return _isDummy; }
	void SetIsDummy(bool b) { _isDummy = b; }

	MOVE_TYPE GetMoveType() const { return _moveType; }
	void SetMoveType(MOVE_TYPE m) { _moveType = m; }

	int GetTargetID() const { return _targetID; }
	void SetTargetID(int id) { _targetID = id; }

	bool GetNearNpc() const { return _isNear; }
	void SetNearNpc(bool isNear) { _isNear = isNear; }

	bool GetTarget() const { return _isTarget; }
	void SetTarget(bool isTarget) { _isTarget = isTarget; }

	int GetNextPosIndex() const { return _nextPosIndex; }
	void SetNextPosIndex(int i) { _nextPosIndex = i; }
	Vec3 GetNextPos(int i) const { return _nextPos[i]; }
	void SetNextPos(int i, float x, float y, float z) { 
		_nextPos[i].x = x;
		_nextPos[i].y = y; 
		_nextPos[i].z = z;

	}
	void SetDunGeonEnter(bool isEnter) { _dungeonEnter = isEnter; }
	bool GetDunGeonEnter() const { return _dungeonEnter; }

	void SetBossMapEnter(bool isEnter) { _bossmapEnter = isEnter; }
	bool GetBossMapEnter() const { return _bossmapEnter; }

	void SetMonsterCol(bool isCol) { _m2mCol = isCol; }
	bool GetMonsterCol() const { return _m2mCol; }
};

