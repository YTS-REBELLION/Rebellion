#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class CAnimator3D 
	: public CComponent
{
private:
	const vector<tMTBone>*		m_pVecBones;
	const vector<tMTAnimClip>*	m_pVecClip;
	vector<tMTAnimClip>			m_pVecAnimClip;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // 클립 인덱스

	int							m_iFrameIdx; // 클립의 현재 프레임
	Ptr<CMaterial>				m_pBoneMtrl; // BoneData Update Compute Shader
	CStructuredBuffer*			m_pBoneFinalMat;  // 특정 프레임의 최종 행렬
	bool						m_bFinalMatUpdate; // 최종행렬 연산 수행여부

	bool						m_bAniUse; // animator 사용할 건지 말 건지
public:
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void UpdateData();
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
	float GetCliTime(int _iClipIdx) { return  m_vecClipUpdateTime[_iClipIdx]; }
	//vector<float> Getab() { return m_vecClipUpdateTime }
	tMTAnimClip GetAnimClip(int _idx) { return m_pVecClip->at(_idx); }
	//double GetCurTime() { return m_dCurTime; }
	vector<Matrix>& GetFinalBoneMat() { return m_vecFinalBoneMat; }

	void SetCurClip(int CurClip) { m_iCurClip = CurClip; }
	int GetCurClip() { return m_iCurClip;}

	int GetFrameIdx() { return m_iFrameIdx; }
	void SetFrameIdx(const int& _idx) { m_iFrameIdx = _idx; }
	void SetCurTime(const double& _CurTime) { m_vecClipUpdateTime[m_iCurClip] = _CurTime; }
	
	void SetCurTime(const UINT& _iClipIdx, const double& _CurTime) { m_vecClipUpdateTime[_iClipIdx] = _CurTime; }
	double GetCurTime() { return m_vecClipUpdateTime[m_iCurClip]; }
	double GetCurTime(const UINT& _iClipIdx) { return m_vecClipUpdateTime[_iClipIdx]; }
	
	const vector<tMTAnimClip>* GetAnimClip() { return m_pVecClip; }


	bool GetAniUse() { return m_bAniUse; }
private:
	void check_mesh(Ptr<CMesh> _pMesh);

public:
	virtual void awake() {};
	virtual void start() {};
	virtual void update();
	virtual void lateupdate();
	virtual void finalupdate();

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CAnimator3D* Clone() { return new CAnimator3D(*this); }

public:
	CAnimator3D();
	virtual ~CAnimator3D();
};

