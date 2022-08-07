#pragma once

#include "FBXLoader.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CGameObject;


class CMeshData :
	public CResource
{
private:
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;

	// Animation3D
public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);
	
	virtual void Load(const wstring& _strFilePath);
	virtual void Save(const wstring& _strFilePath);

	CGameObject* Instantiate();	
	CGameObject* DmyInstantiate();

	Ptr<CMesh> GetMesh() { return m_pMesh; }
	Ptr<CMaterial> GetMtrl(UINT _iSubSet = 0) { return m_vecMtrl[_iSubSet]; }
public:
	CMeshData();
	virtual ~CMeshData();
};