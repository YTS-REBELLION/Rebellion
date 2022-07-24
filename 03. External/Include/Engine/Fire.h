#pragma once
#include "Script.h"

struct NoiseBufferType
{
	float frameTime;
	Vec4 scrollSpeeds;
	Vec4 scales;
	float padding;
};

struct DistortionBufferType
{
	Vec2 distortion1;
	Vec2 distortion2;
	Vec2 distortion3;
	float distortionScale;
	float distortionBias;
};

class CFire :
	public CScript
{
private:
	NoiseBufferType			m_NoiseBuffer;
	DistortionBufferType	m_DistortionBuffer;

public:
	void init();
	NoiseBufferType GetNoiseBuffer() { return m_NoiseBuffer; }
	DistortionBufferType GetDistortionBuffer() { return m_DistortionBuffer; }

public:
	virtual void update();
	

public:
	CLONE(CFire);

public:
	CFire();
	virtual ~CFire();
};

