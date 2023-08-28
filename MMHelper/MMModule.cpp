#include "StdAfx.h"
#include "MMModule.h"

//////////////////////////////////////////////////////////////////////////
//��Ʒ�汾
BYTE GetProductVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[3];
}

//��Ҫ�汾
BYTE GetMainVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[2];
}

//��Ҫ�汾
BYTE GetSubVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[1];
}

//��Ҫ�汾
BYTE GetBuildVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[0];
}

//�汾�Ƚ�
bool InterfaceVersionCompare(DWORD dwQueryVer, DWORD dwInterfaceVer)
{
	if (GetSubVer(dwQueryVer) > GetSubVer(dwInterfaceVer)) return false;
	if (GetMainVer(dwQueryVer) != GetMainVer(dwInterfaceVer)) return false;
	if (GetBuildVer(dwQueryVer) != GetBuildVer(dwInterfaceVer)) return false;
	if (GetProductVer(dwQueryVer) != GetProductVer(dwInterfaceVer)) return false;
	return true;
};

//////////////////////////////////////////////////////////////////////////