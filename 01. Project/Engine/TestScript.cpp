#include "stdafx.h"
#include "TestScript.h"

CTestScript::CTestScript()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
	, m_iLifeCount(2)
{
}

CTestScript::~CTestScript()
{
}

void CTestScript::update()
{

}

