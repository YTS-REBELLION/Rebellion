#pragma once
class CPlayerScript;
class CGameObject;

class DeadReckoning
{
	Vec3	_localPos;
	Vec3	_dirVec;
	float	_rotateY;
	float	_speed;
public:
	DeadReckoning() {};
	DeadReckoning(const int& id);
	~DeadReckoning() {};
public:
	//Get
	Vec3 GetLocalPos() { return _localPos; }
	Vec3 GetDirVec() { return _dirVec; }
	float GetRotateY() { return _rotateY; }
	float GetSpeed() { return _speed; }

	//Set
	void SetLocalPos(const Vec3& localpos) { _localPos = localpos; }
	void SetDirVec(const Vec3& dirvec) { _dirVec = dirvec; }
	void SetRotateY(const float& rotateY) { _rotateY = rotateY; }
	void SetSpeed(const float& speed) { _speed = speed; }

	void DeadReck(CGameObject* obj);
	bool isFollowing();

};

