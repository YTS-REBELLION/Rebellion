#include "pch.h"
#include "CObject.h"

CObject::CObject()
	:_isMove(false), _isAttack(false), _isHeal(false), _isDefence(false), _isDummy(false), _isNear(false) {
	for (int i = 0; i < 4; ++i) 
		_moveDirection[i] = false;

}
CObject::~CObject()
{

}