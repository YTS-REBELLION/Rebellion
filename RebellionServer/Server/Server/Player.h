#pragma once

#include "CObject.h"

class CPlayer : public CObject
{
	SOCKET		_socket;


public:
	CPlayer();
	~CPlayer();

public:
	SOCKET GetSocket() const { return _socket; }


};