#include "stdafx.h"
#include "DeadReckoning.h"
#include "PlayerScript.h"
#include "GameObject.h"

DeadReckoning::DeadReckoning(const int& id)
{
    _localPos = GameObject.find(id)->second->Transform()->GetLocalPos();
    _rotateY = GameObject.find(id)->second->Transform()->GetLocalRot().y;
    _speed = GameObject.find(id)->second->GetScript<CPlayerScript>()->GetSpeed();
    _dirVec = GameObject.find(id)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT);

}
void DeadReckoning::DeadReck(CGameObject* obj)
{
    _localPos += _dirVec * _speed * DT;
}



bool DeadReckoning::isFollowing()
{
    Vec2 player(GameObject.find(g_myid)->second->Transform()->GetLocalPos().x, GameObject.find(g_myid)->second->Transform()->GetLocalPos().z);
    Vec2 follower(_localPos.x, _localPos.z);

    float distance = (player.x - follower.x) * (player.x - follower.x) + (player.y - follower.y) * (player.y - follower.y);

    return false;
}
