#include "stdafx.h"

#include "SoundMgr.h"

#include "ResMgr.h"


void CSoundMgr::Play(Sound_Type type, const bool& isasync, const bool& isloop)
{
	wstring strFullPath = CPathMgr::GetResPath();
	switch (type)
	{
	case Sound_Type::BGM:
		strFullPath += L"Sound\\background.wav";
		break;
	case Sound_Type::HIT:
		strFullPath += L"Sound\\hit.wav";
		break;
	
	default:
		break;
	}


	if (isasync && isloop)	PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else if (isloop)		PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else if (isasync)		PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC);
	else					PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC);

}
