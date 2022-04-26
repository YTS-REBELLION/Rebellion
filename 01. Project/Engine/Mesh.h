#pragma once
#include "Resource.h"

#include "FBXLoader.h"
#include "Ptr.h"
#include "Texture.h"

struct tIndexInfo
{
	ComPtr<ID3D12Resource>		pIB;
	D3D12_INDEX_BUFFER_VIEW		tIdxView;
	DXGI_FORMAT					eIdxFormat;
	UINT						iIdxCount;
	void*						pIdxSysMem;
};

class CMesh :
	public CResource
{
private:
	ComPtr<ID3D12Resource>		m_pVB;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView; // ���ؽ� ���� ���� ����ü(stride, total size, ���� GPU �ּ�)
	
	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem;

	vector<tIndexInfo>			m_vecIdxInfo;

	// Animation3D ����
	vector<tMTAnimClip>			m_vecAnimClip;
	vector<tMTBone>				m_vecBones;
	Ptr<CTexture>				m_pBoneTex;

private:
	//	�� ��ü mesh ��ǥ �ּ� �ִ밪 ������ ���� �������.
	Vec4						m_vecMMax[2];

public:
	void Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem
		, DXGI_FORMAT _iIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem);

	static CMesh * CreateFromContainer(CFBXLoader & _loader);

	void render(UINT _iSubset = 0);

public:
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
	const vector<tMTBone>* GetBones() { return &m_vecBones; }
	void SetBoneTex(Ptr<CTexture> _pTex) { m_pBoneTex = _pTex; }
	const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
	Ptr<CTexture> GetBoneTex() { return m_pBoneTex; }
	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

public:	
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath/*��� ���*/);

public:
	Vec4* GetMinMaxVertex() { return m_vecMMax; }
	void SetMinMaxVertex(Vec4* _v) { m_vecMMax[0] = _v[0]; m_vecMMax[1] = _v[1]; }

public:
	CMesh();
	virtual ~CMesh();
};

