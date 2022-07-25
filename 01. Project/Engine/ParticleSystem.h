#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"


class CStructuredBuffer;

class CParticleSystem :
	public CComponent
{
private:
	CStructuredBuffer* m_pParticleBuffer; // ��ƼŬ ������ ����
	CStructuredBuffer* m_pSharedBuffer;   // ��ƼŬ ���� �޸�
	UINT				m_iMaxParticle;    // �ִ� ��ƼŬ ����

	Ptr<CMaterial>		m_pUpdateMtrl;		// ��ǻƮ ���̴�
	Ptr<CMaterial>		m_pMtrl;			// ��ƼŬ ��������
	Ptr<CMesh>			m_pMesh;			// ��� �޽�(Point Mesh)

	float				m_fFrequency;		// ��ƼŬ ���� �ֱ�
	float				m_fAccTime;		    // �����ð� üũ�뵵

	float				m_fMinLifeTime;   // ��ƼŬ �ּ� ���� �ð�
	float				m_fMaxLifeTime;   // ��ƼŬ �ִ� ���� �ð�

	float				m_fMinSpeed;	  // ��ƼŬ ���� �ӵ�
	float				m_fMaxSpeed;	  // ��ƼŬ ���� �ӵ�

	float				m_fStartScale;   // ��ƼŬ ���� ũ��
	float				m_fEndScale;     // ��ƼŬ ���� ũ��

	Vec4				m_vStartColor;	// ��ƼŬ �ʱ� ����
	Vec4				m_vEndColor;    // ��ƼŬ ���� ����

private:
	bool				m_bType;

public:
	virtual void finalupdate();
	virtual void render();

public:
	float GetAcctime() { return m_fAccTime; }
	float GetMaxLifeTime() { return m_fMaxLifeTime; }

public:
	void Init(float frequency, float acctime, float minLife, float maxLife, float minSpeed, float maxSpeed, float StartScale, float EndScale, Vec4 StartColor, Vec4 EndColor);
	void SetMaxParticle(UINT _i) { m_iMaxParticle = _i; }
	void SetStartColor(const Vec4& _vColor) { m_vStartColor = _vColor; }
	void SetEndColor(const Vec4& _vColor) { m_vEndColor = _vColor; }
	void SetFrequency(float _fre) { m_fFrequency = _fre; }
	void SetMinLife(float _min) { m_fMinLifeTime = _min; }
	void SetMaxLife(float _max) { m_fMaxLifeTime = _max; }

	void SetMesh(Ptr<CMesh> _m) { m_pMesh = _m; }
	void SetMtrl(Ptr<CMaterial> _m) { m_pMtrl = _m; }

public:
	void SetType(bool _b) { m_bType = _b; }

	int InfiniteParticle();
	int finiteParticle();

public:
	CLONE(CParticleSystem);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CParticleSystem();
	virtual ~CParticleSystem();
};
