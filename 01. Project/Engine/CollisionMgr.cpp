#include "stdafx.h"
#include "CollisionMgr.h"

#include "Network.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider2D.h"

#include"ParticleScript.h"
#include"ParticleSystem.h"

CCollisionMgr::CCollisionMgr()
	: m_LayerCheck{}

{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::init()
{
}

void CCollisionMgr::update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (int j = 0; j < MAX_LAYER; ++j)
		{			
			if (m_LayerCheck[i] & (1 << j))
			{
				// i <= j
				CollisionLayer(pCurScene->GetLayer(i), pCurScene->GetLayer(j));				
			}
		}
	}
}

void CCollisionMgr::CheckCollisionLayer(const wstring & _strLayerName1, const wstring & _strLayerName2)
{
	CLayer* pLayer1 = CSceneMgr::GetInst()->GetCurScene()->FindLayer(_strLayerName1);
	CLayer* pLayer2 = CSceneMgr::GetInst()->GetCurScene()->FindLayer(_strLayerName2);

	assert(nullptr != pLayer1 && nullptr != pLayer2);

	int iIdx1 = pLayer1->GetLayerIdx();
	int iIdx2 = pLayer2->GetLayerIdx();

	int iMinIdx = min(iIdx1, iIdx2);
	int iMaxIdx = max(iIdx1, iIdx2);

	m_LayerCheck[iMinIdx] |= (1 << iMaxIdx);
}

void CCollisionMgr::CheckCollisionLayer(int _iLayerIdx1, int _iLyaerIdx2)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();

	assert(pScene->GetLayer(_iLayerIdx1) && pScene->GetLayer(_iLyaerIdx2));

	int iMinIdx = min(_iLayerIdx1, _iLyaerIdx2);
	int iMaxIdx = max(_iLayerIdx1, _iLyaerIdx2);

	m_LayerCheck[iMinIdx] |= (1 << iMaxIdx);
}

void CCollisionMgr::CollisionLayer(const CLayer * _pLayer1, const CLayer * _pLayer2)
{
	const vector<CGameObject*>& vecObj1 = _pLayer1->GetObjects();
	const vector<CGameObject*>& vecObj2 = _pLayer2->GetObjects();

	map<DWORD_PTR, bool>::iterator iter;
	
	for (size_t i = 0; i < vecObj1.size(); ++i)
	{
		CCollider2D* pCollider1 = vecObj1[i]->Collider2D();

		if (nullptr == pCollider1)
			continue;

		size_t j = 0;		
		if (_pLayer1 == _pLayer2) // 동일한 레이어 간의 충돌을 검사하는 경우
			j = i + 1;

		for (; j < vecObj2.size(); ++j)
		{			
			CCollider2D* pCollider2 = vecObj2[j]->Collider2D();

			if (nullptr == pCollider2)
				continue;

			tColID id;
			id.iColID1 = pCollider1->GetColID();
			id.iColID2 = pCollider2->GetColID();
			iter = m_mapCol.find(id.ID);
			bool IsDead = false;
			if (pCollider1->GetObj()->IsDead()) {
				IsDead = true;
			}

			if (pCollider2->GetObj()->IsDead()) {
				IsDead = true;
			}
				
			// 충돌했다.
			if (IsCollision(pCollider1, pCollider2))
			{
				// 충돌 중이다
				if (m_mapCol.end() != iter && iter->second == true)
				{
					if (IsDead)
					{
						if (!m_bColCheck2)
						{
							m_bColCheck = true;
							pCollider1->OnCollisionExit(pCollider2);
							pCollider2->OnCollisionExit(pCollider1);
							iter->second = false;
						}
					}
					else
					{
						if (!m_bColCheck2)
						{
							m_bColCheck = true;
							pCollider1->OnCollision(pCollider2);
							pCollider2->OnCollision(pCollider1);
						}
					}					
				}
				// 처음 충돌했다
				else
				{
					if (IsDead)
						continue;
					if (!m_bColCheck2)
					{
						m_bColCheck = true;
						pCollider1->OnCollisionEnter(pCollider2);
						pCollider2->OnCollisionEnter(pCollider1);

						if (m_mapCol.end() == iter)
						{
							m_mapCol.insert(make_pair(id.ID, true));
						}
						else
						{
							iter->second = true;
						}
					}
				}
			}
			else // 충돌하지 않는다.
			{
				if (m_mapCol.end() != iter && true == iter->second)
				{
					pCollider1->OnCollisionExit(pCollider2);
					pCollider2->OnCollisionExit(pCollider1);

					iter->second = false;
				}
			}
		}
	}

}

bool CCollisionMgr::IsCollision(CCollider2D * _pCollider1, CCollider2D * _pCollider2)
{
	if (!_pCollider1->IsActive() || !_pCollider1->GetObj()->IsActive() || !_pCollider2->IsActive() || !_pCollider2->GetObj()->IsActive())
		return false;

	if (COLLIDER2D_TYPE::BOX == _pCollider1->GetColliderType() && COLLIDER2D_TYPE::BOX == _pCollider2->GetColliderType())
	{
		return CollisionCube(_pCollider1, _pCollider2);
	}
	else if (COLLIDER2D_TYPE::SPHERE == _pCollider1->GetColliderType() && COLLIDER2D_TYPE::SPHERE == _pCollider2->GetColliderType())
	{
		return CollisionSphere(_pCollider1, _pCollider2);
	}
	else if (COLLIDER2D_TYPE::BOX == _pCollider1->GetColliderType() && COLLIDER2D_TYPE::SPHERE == _pCollider2->GetColliderType())
	{
		return CollisionRectCircle(_pCollider1, _pCollider2);
	}
	else if (COLLIDER2D_TYPE::SPHERE == _pCollider1->GetColliderType() && COLLIDER2D_TYPE::BOX == _pCollider2->GetColliderType())
	{
		return CollisionRectCircle(_pCollider1, _pCollider2);
	}
	return false;
}

bool CCollisionMgr::CollisionRect(CCollider2D * _pCollider1, CCollider2D * _pCollider2)
{
	static Vec3 arrLocal[4] = {					// 0 -- 1
		  Vec3(-0.5f, 0.5f, 0.f)				// |	|
		, Vec3(0.5f, 0.5f, 0.f)					// 3 -- 2
		, Vec3(0.5f, -0.5f, 0.f)
		, Vec3(-0.5f, -0.5f, 0.f)};	
	

	const Matrix& matCol1 = _pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = _pCollider2->GetColliderWorldMat();

	Vec3 arrCol1[4] = {};
	Vec3 arrCol2[4] = {};
	Vec3 arrCenter[2] = {};
	
	for (UINT i = 0; i < 4; ++i)
	{
		arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
		arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);

		// 2D 충돌이기 때문에 같은 Z 좌표상에서 충돌을 계산한다.
		arrCol1[i].z = 0.f;
		arrCol2[i].z = 0.f;
	}

	arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);
	arrCenter[0].z = 0.f;
	arrCenter[1].z = 0.f;

	Vec3 vCenter = arrCenter[1] - arrCenter[0];
	
	Vec3 arrOriginVec[4] = { arrCol1[3] - arrCol1[0]
		, arrCol1[1] - arrCol1[0]
		, arrCol2[3] - arrCol2[0]
		, arrCol2[1] - arrCol2[0]
	};

	Vec3 arrProjVec[4] = {};
	for (UINT i = 0; i < 4; ++i)
	{
		arrOriginVec[i].Normalize(arrProjVec[i]);
	}


	// 투영을 통해서 분리축 테스트
	// vCenter		 두 사각형의 중심을 잇는 벡터
	// arrOriginVec  각 사각형의 표면 벡터
	// arrProjVec    사각형의 표면과 평행한 투영축 벡터(단위벡터)

	for (UINT i = 0; i < 4; ++i)
	{
		float fCenter = abs(vCenter.Dot(arrProjVec[i])); // 중심 거리 벡터를 해당 투영축으로 투영시킨 길이
		
		float fAcc = 0.f;
		for (UINT j = 0; j < 4; ++j)
			fAcc += abs(arrOriginVec[j].Dot(arrProjVec[i]));

		fAcc /= 2.f;

		if (fCenter > fAcc)
			return false;
	}

	return true;
}

bool CCollisionMgr::CollisionCircle(CCollider2D * _pCollider1, CCollider2D * _pCollider2)
{
	
	return false;
}

//bool CCollisionMgr::CollisionCube(CCollider2D* _pCollider1, CCollider2D* _pCollider2)
//{
//	{
//		static Vec3 arrLocal[4] = {					// 0 -- 1
//		  Vec3(-0.5f, 0.5f, 0.f)				// |	|
//		, Vec3(0.5f, 0.5f, 0.f)					// 3 -- 2
//		, Vec3(0.5f, -0.5f, 0.f)
//		, Vec3(-0.5f, -0.5f, 0.f) };
//
//		const Matrix& matCol1 = _pCollider1->GetColliderWorldMat();
//		const Matrix& matCol2 = _pCollider2->GetColliderWorldMat();
//
//		Vec3 arrCol1[4] = {};
//		Vec3 arrCol2[4] = {};
//		Vec3 arrCenter[2] = {};
//
//
//		for (UINT i = 0; i < 4; ++i)
//		{
//			arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
//			arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);
//
//			// 2D 충돌이기 때문에 같은 Z 좌표상에서 충돌을 계산한다.
//			arrCol1[i].z = 0.f;
//			arrCol2[i].z = 0.f;
//		}
//
//		arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
//		arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);
//		arrCenter[0].z = 0.f;
//		arrCenter[1].z = 0.f;
//
//		Vec3 vCenter = arrCenter[1] - arrCenter[0];
//
//		Vec3 arrOriginVec[4] = { arrCol1[3] - arrCol1[0]
//			, arrCol1[1] - arrCol1[0]
//			, arrCol2[3] - arrCol2[0]
//			, arrCol2[1] - arrCol2[0]
//		};
//
//		Vec3 arrProjVec[4] = {};
//		for (UINT i = 0; i < 4; ++i)
//		{
//			arrOriginVec[i].Normalize(arrProjVec[i]);
//		}
//
//
//		// 투영을 통해서 분리축 테스트
//		// vCenter		 두 사각형의 중심을 잇는 벡터
//		// arrOriginVec  각 사각형의 표면 벡터
//		// arrProjVec    사각형의 표면과 평행한 투영축 벡터(단위벡터)
//
//		for (UINT i = 0; i < 4; ++i)
//		{
//			float fCenter = abs(vCenter.Dot(arrProjVec[i])); // 중심 거리 벡터를 해당 투영축으로 투영시킨 길이
//
//			float fAcc = 0.f;
//			for (UINT j = 0; j < 4; ++j)
//				fAcc += abs(arrOriginVec[j].Dot(arrProjVec[i]));
//
//			fAcc /= 2.f;
//
//			if (fCenter > fAcc) {
//				return false;
//			}
//		}
//	}
//	return true;
//
//	
//}

bool CCollisionMgr::CollisionRectCircle(CCollider2D* _pCollider1, CCollider2D* _pCollider2)
{
	// _pCollider1 = circle
	// _pCollider2 = rect
	/*if (_pCollider1->GetObj()->GetName() == L"PlayerCol")
		cout << "너 여기로 들어오니?" << endl;*/
	float CenterX = _pCollider1->GetObj()->Transform()->GetLocalPos().x;
	float CenterZ = _pCollider1->GetObj()->Transform()->GetLocalPos().z;
	float Radius = _pCollider1->GetObj()->Transform()->GetLocalScale().x *_pCollider1->Collider2D()->GetOffsetScale().x;

	// left		: -x
	// right	: x
	// top		: z
	// bottom	: -z

	float left		= _pCollider2->GetObj()->Transform()->GetLocalPos().x - (_pCollider2->Transform()->GetLocalScale().x * _pCollider2->Collider2D()->GetOffsetScale().x) / 2;
	float right		= _pCollider2->GetObj()->Transform()->GetLocalPos().x + (_pCollider2->Transform()->GetLocalScale().x * _pCollider2->Collider2D()->GetOffsetScale().x) / 2;
	float top		= _pCollider2->GetObj()->Transform()->GetLocalPos().z - (_pCollider2->Transform()->GetLocalScale().z * _pCollider2->Collider2D()->GetOffsetScale().z) / 2;
	float bottom	= _pCollider2->GetObj()->Transform()->GetLocalPos().z + (_pCollider2->Transform()->GetLocalScale().z * _pCollider2->Collider2D()->GetOffsetScale().z) / 2;

	if ((left <= CenterX && CenterX < right))
	{
		if ((left - Radius < CenterX && CenterX < right + Radius) &&
			(top - Radius < CenterZ && CenterZ < bottom + Radius)) {
			_pCollider2->SetPlane(COL_PLANE::X_PLANE);
			return true;
		}
	}
	else if ((top <= CenterZ && CenterZ < bottom))
	{
		if ((left - Radius < CenterX && CenterX < right + Radius) &&
			(top - Radius < CenterZ && CenterZ < bottom + Radius)) {
			_pCollider2->SetPlane(COL_PLANE::Z_PLANE);
			return true;
		}
	}
	else {
		// 사각형 좌상단이 원 안에 있는지
		if(Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(left, 0.f, top))) return true;

		// 사각형 좌하단이 원 안에 있는지
		if (Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(left, 0.f, bottom))) return true;

		// 사각형 우상단이 원 안에 있는지
		if (Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(right, 0.f, top))) return true;

		// 사각형 우하단이 원 안에 있는지
		if (Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(right, 0.f, bottom))) return true;
	}
	//float pos[4] = 
	//{   
	//	(_pCollider2->Transform()->GetLocalPos().x + (_pCollider2->Transform()->GetLocalScale().x * _pCollider2->Collider2D()->GetOffsetScale().x) / 2),		// max_x
	//	(_pCollider2->Transform()->GetLocalPos().x - (_pCollider2->Transform()->GetLocalScale().x * _pCollider2->Collider2D()->GetOffsetScale().x) / 2),		// min_x
	//	(_pCollider2->Transform()->GetLocalPos().z + (_pCollider2->Transform()->GetLocalScale().z * _pCollider2->Collider2D()->GetOffsetScale().z) / 2),		// max_z	
	//	(_pCollider2->Transform()->GetLocalPos().z - (_pCollider2->Transform()->GetLocalScale().z * _pCollider2->Collider2D()->GetOffsetScale().z) / 2),		// min_z
	//};
	//int X_Zone = (_pCollider1->Transform()->GetLocalPos().x < pos[1]) ? 0 :
	//	(_pCollider1->Transform()->GetLocalPos().x > pos[0]) ? 2 : 1;

	//int Z_Zone = (_pCollider1->Transform()->GetLocalPos().z < pos[3]) ? 0 :
	//	(_pCollider1->Transform()->GetLocalPos().z > pos[2]) ? 2 : 1;

	//int N_Zone = X_Zone + 3 * Z_Zone;

	//switch (N_Zone)
	//{
	//case 1:
	//case 7:
	//{
	//	float disZ = fabsf(_pCollider1->Transform()->GetLocalPos().z - _pCollider2->Transform()->GetLocalPos().z);
	//	_pCollider2->SetPlane(COL_PLANE::X_PLANE);
	//	if (disZ <= (_pCollider1->Transform()->GetLocalScale().z * _pCollider1->Collider2D()->GetOffsetScale().z) +
	//		(_pCollider2->Transform()->GetLocalScale().z * _pCollider2->Collider2D()->GetOffsetScale().z) / 2)
	//		return true;
	//	break;
	//}
	//case 3:
	//case 5:
	//{
	//	float disX = fabsf(_pCollider1->Transform()->GetLocalPos().x - _pCollider2->Transform()->GetLocalPos().x);
	//	_pCollider2->SetPlane(COL_PLANE::Z_PLANE);
	//	if (disX <= (_pCollider1->Transform()->GetLocalScale().x * _pCollider1->Collider2D()->GetOffsetScale().x) +
	//		(_pCollider2->Transform()->GetLocalScale().x * _pCollider2->Collider2D()->GetOffsetScale().x) / 2)
	//		return true;
	//	break;
	//}
	//case 4:
	//	return true;
	//	break;
	//default:
	//	break;
	//}
	return false;
}

bool CCollisionMgr::CollisionSphere(CCollider2D* _pCollider1, CCollider2D* _pCollider2)
{
	Vec3 Center1 = Vec3(_pCollider1->Transform()->GetLocalPos().x, 0.f, _pCollider1->Transform()->GetLocalPos().z);
	Vec3 Center2 = Vec3(_pCollider2->Transform()->GetLocalPos().x, 0.f, _pCollider2->Transform()->GetLocalPos().z);

	float Radius1 = (_pCollider1->Transform()->GetLocalScale().x * _pCollider1->Collider2D()->GetOffsetScale().x);
	float Radius2 = (_pCollider2->Transform()->GetLocalScale().x * _pCollider2->Collider2D()->GetOffsetScale().x);
	float temp = Length(Center2 - Center1);

	if ((Radius1 + Radius2) >= temp)
	{

		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Particle");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CParticleSystem);
		pObject->Particlesystem()->SetFrequency(2.f);
		pObject->Particlesystem()->SetType(false);
		pObject->Particlesystem()->SetMaxParticle(10);
		pObject->AddComponent(new CParticleScript);
		pObject->GetScript<CParticleScript>()->SetLifeTime(pObject->Particlesystem()->GetMaxLifeTime());
		pObject->FrustumCheck(false);
		Vec3 particlePos = Center2;
		pObject->Transform()->SetLocalPos(particlePos);
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(pObject);


		return true;
	}
	else
		return false;
}

bool CCollisionMgr::CollisionCube(CCollider2D* _pCollider1, CCollider2D* _pCollider2)
{
		{
		static Vec3 arrLocal[4] = {               // 0 -- 1
			 Vec3(-0.5f, 0.0f, 0.5f)			  // |    |
		   , Vec3(0.5f, 0.0f, 0.5f)               // 3 -- 2
		   , Vec3(0.5f, 0.0f, -0.5f)
		   , Vec3(-0.5f, 0.0f, -0.5f) };


		const Matrix& matCol1 = _pCollider1->GetColliderWorldMat();
		const Matrix& matCol2 = _pCollider2->GetColliderWorldMat();

		Vec3 arrCol1[4] = {};
		Vec3 arrCol2[4] = {};
		Vec3 arrCenter[2] = {};

		for (UINT i = 0; i < 4; ++i)
		{
			arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
			arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);

			// 2D 충돌이기 때문에 같은 Z 좌표상에서 충돌을 계산한다.
			arrCol1[i].y = 0.f;
			arrCol2[i].y = 0.f;
		}

		arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
		arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);
		arrCenter[0].y = 0.f;
		arrCenter[1].y = 0.f;

		Vec3 vCenter = arrCenter[1] - arrCenter[0];

		Vec3 arrOriginVec[4] = { arrCol1[3] - arrCol1[0]
		   , arrCol1[1] - arrCol1[0]
		   , arrCol2[3] - arrCol2[0]
		   , arrCol2[1] - arrCol2[0]
		};

		Vec3 arrProjVec[4] = {};
		for (UINT i = 0; i < 4; ++i)
		{
			arrOriginVec[i].Normalize(arrProjVec[i]);
		}


		// 투영을 통해서 분리축 테스트
		// vCenter       두 사각형의 중심을 잇는 벡터
		// arrOriginVec  각 사각형의 표면 벡터
		// arrProjVec    사각형의 표면과 평행한 투영축 벡터(단위벡터)

		for (UINT i = 0; i < 4; ++i)
		{
			//if (_pCollider1->GetObj()->GetName() == L"Player1" && _pCollider2->GetObj()->GetName() == L"Map Object") {
			//	cout << i << " 표면   : " << arrOriginVec[i].x << ", " << arrOriginVec[i].y << ", " << arrOriginVec[i].z << endl;
			//	cout << i << " 단위   : " << arrProjVec[i].x << ", " << arrProjVec[i].y << ", " << arrProjVec[i].z << endl;
			//}
			//cout << endl;
			float fCenter = abs(vCenter.Dot(arrProjVec[i])); // 중심 거리 벡터를 해당 투영축으로 투영시킨 길이

			float fAcc = 0.f;
			for (UINT j = 0; j < 4; ++j)
				fAcc += abs(arrOriginVec[j].Dot(arrProjVec[i]));

			fAcc /= 2.f;

			if (fCenter > fAcc)
				return false;
		}
	}
	return true;
}

//bool CCollisionMgr::CollisionCube(CCollider2D* _pCollider1, CCollider2D* _pCollider2)
//{
//	{
//		static Vec3 arrLocal[4] = {					// 0 -- 1
//		  Vec3(-0.5f, 0.5f, 0.f)					// |	|
//		, Vec3(0.5f, 0.5f, 0.f)						// 3 -- 2
//		, Vec3(0.5f, -0.5f, 0.f)
//		, Vec3(-0.5f, -0.5f, 0.f) };
//
//
//		const Matrix& matCol1 = _pCollider1->GetColliderWorldMat();
//		const Matrix& matCol2 = _pCollider2->GetColliderWorldMat();
//
//		Vec3 arrCol1[4] = {};
//		Vec3 arrCol2[4] = {};
//		Vec3 arrCenter[2] = {};
//
//
//		for (UINT i = 0; i < 4; ++i)
//		{
//			arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
//			arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);
//
//			// 2D 충돌이기 때문에 같은 Z 좌표상에서 충돌을 계산한다.
//			arrCol1[i].z = 0.f;
//			arrCol2[i].z = 0.f;
//		}
//
//		arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
//		arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);
//		arrCenter[0].z = 0.f;
//		arrCenter[1].z = 0.f;
//
//		Vec3 vCenter = arrCenter[1] - arrCenter[0];
//
//		Vec3 arrOriginVec[4] = { arrCol1[3] - arrCol1[0]
//			, arrCol1[1] - arrCol1[0]
//			, arrCol2[3] - arrCol2[0]
//			, arrCol2[1] - arrCol2[0]
//		};
//
//		Vec3 arrProjVec[4] = {};
//		for (UINT i = 0; i < 4; ++i)
//		{
//			arrOriginVec[i].Normalize(arrProjVec[i]);
//		}
//
//
//		// 투영을 통해서 분리축 테스트
//		// vCenter		 두 사각형의 중심을 잇는 벡터
//		// arrOriginVec  각 사각형의 표면 벡터
//		// arrProjVec    사각형의 표면과 평행한 투영축 벡터(단위벡터)
//
//		for (UINT i = 0; i < 4; ++i)
//		{
//			float fCenter = abs(vCenter.Dot(arrProjVec[i])); // 중심 거리 벡터를 해당 투영축으로 투영시킨 길이
//
//			float fAcc = 0.f;
//			for (UINT j = 0; j < 4; ++j)
//				fAcc += abs(arrOriginVec[j].Dot(arrProjVec[i]));
//
//			fAcc /= 2.f;
//
//			if (fCenter > fAcc)
//				return false;
//		}
//	}
//	{
//		static Vec3 arrLocal[4] = {						// 0 -- 1
//			  Vec3(-0.5f, 0.0f, 0.5f)					// |	|
//			, Vec3(0.5f, 0.0f, 0.5f)					// 3 -- 2
//			, Vec3(0.5f, 0.0f, -0.5f)
//			, Vec3(-0.5f, 0.0f, -0.5f) };
//
//		const Matrix& matCol1 = _pCollider1->GetColliderWorldMat();
//		const Matrix& matCol2 = _pCollider2->GetColliderWorldMat();
//
//
//		//XMMATRIX matCol3 = _pCollider1->GetColliderWorldMat();
//		//XMMATRIX matCol4 = _pCollider2->GetColliderWorldMat();
//		//XMMATRIX matCol5;
//		//XMMATRIX matCol6;
//
//		//if (_pCollider1->GetObj()->GetName() == L"Player1") {
//		//	XMMATRIX matColPR = {
//		//		{1,0,0,0},
//		//		{0,cos(XM_PI/2),-sin(XM_PI/2),0},
//		//		{0,sin(XM_PI/2), cos(XM_PI/2),0},
//		//		{0,0,0,1}
//
//		//		//{cos(XM_PI / 2),-sin(XM_PI / 2),0,0},
//		//		//{sin(XM_PI / 2),cos(XM_PI / 2),0,0},
//		//		//{0,0,1,0},
//		//		//{0,0,0,1}
//		//	};
//		//	matCol5 = matColPR * matCol3;
//		//}
//		//else if (_pCollider1->GetObj()->GetName() == L"Player_Sword")
//		//{
//		//	XMMATRIX matColPR = {
//		//		{cos(XM_PI / 2),-sin(XM_PI / 2),0,0},
//		//		{sin(XM_PI / 2),cos(XM_PI / 2),0,0},
//		//		{0,0,1,0},
//		//		{0,0,0,1}
//		//		//{cos(-XM_PI / 2),0,-sin(-XM_PI / 2),0},
//		//		//{0,1,0,0},
//		//		//{sin(-XM_PI / 2),0, cos(-XM_PI / 2),0},
//		//		//{0,0,0,1}
//		//	};
//		//	matCol5 = matColPR * matCol3;
//		//}
//		//else matCol5 = matCol3;
//
//		//if (_pCollider2->GetObj()->GetName() == L"Monster1") {
//		//	XMMATRIX matColPR = {
//		//		{1,0,0,0},
//		//		{0,cos(-XM_PI / 2),-sin(-XM_PI / 2),0},
//		//		{0,sin(-XM_PI / 2), cos(-XM_PI / 2),0},
//		//		{0,0,0,1}
//
//		//		//{cos(XM_PI / 2),-sin(XM_PI / 2),0,0},
//		//		//{sin(XM_PI / 2),cos(XM_PI / 2),0,0},
//		//		//{0,0,1,0},
//		//		//{0,0,0,1}
//		//	};
//		//	matCol6 = matColPR * matCol4;
//		//}
//		//else matCol6 = matCol4;
//
//		/*if (_pCollider2->GetObj()->GetName() == L"Monster1") {
//			XMMATRIX matColPR = {
//				{1,0,0,0},
//				{0,cos(XM_PI / 2),-sin(XM_PI / 2),0},
//				{0,sin(XM_PI / 2), cos(XM_PI / 2),0},
//				{0,0,0,1}
//			};
//			matCol6 = matColPR * matCol4;
//		}
//		else matCol6 = matCol4;*/
//
//		Vec3 arrCol1[4] = {};
//		Vec3 arrCol2[4] = {};
//		Vec3 arrCenter[2] = {};
//
//		for (UINT i = 0; i < 4; ++i)
//		{
//			arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
//			arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);
//
//			// 2D 충돌이기 때문에 같은 y 좌표상에서 충돌을 계산한다.
//			arrCol1[i].y = 0.f;
//			arrCol2[i].y = 0.f;
//		}
//
//		arrCenter[0] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
//		arrCenter[1] = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);
//		arrCenter[0].y = 0.f;
//		arrCenter[1].y = 0.f;
//
//		Vec3 vCenter = arrCenter[1] - arrCenter[0];
//
//		Vec3 arrOriginVec[4] = { arrCol1[3] - arrCol1[0]
//			, arrCol1[1] - arrCol1[0]
//			, arrCol2[3] - arrCol2[0]
//			, arrCol2[1] - arrCol2[0]
//		};
//
//		Vec3 arrProjVec[4] = {};
//		for (UINT i = 0; i < 4; ++i)
//		{
//			arrOriginVec[i].Normalize(arrProjVec[i]);
//		}
//
//
//		// 투영을 통해서 분리축 테스트
//		// vCenter		 두 사각형의 중심을 잇는 벡터
//		// arrOriginVec  각 사각형의 표면 벡터
//		// arrProjVec    사각형의 표면과 평행한 투영축 벡터(단위벡터)
//
//		for (UINT i = 0; i < 4; ++i)
//		{
//			float fCenter = abs(vCenter.Dot(arrProjVec[i])); // 중심 거리 벡터를 해당 투영축으로 투영시킨 길이
//
//			float fAcc = 0.f;
//			for (UINT j = 0; j < 4; ++j)
//				fAcc += abs(arrOriginVec[j].Dot(arrProjVec[i]));
//
//			fAcc /= 2.f;
//
//			if (fCenter > fAcc)
//				return false;
//		}
//	}
//	return true;
//}