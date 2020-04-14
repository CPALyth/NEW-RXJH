#include "GameFunction.h"

uniform_int_distribution<int> rnd(-10, 10);//�������[-10,10] ���ȷֲ�������
default_random_engine micro;	//���������

/* [2020/02/18 13:22]-[Remark: None] */
/* [Ѱ·��ָ������ŷ���]-[�ɹ������棬���򷵻ؼ�] */
BOOL FindWay(int x, int y)
{
	float flCurPosX;
	float flCurPosY;
	DWORD dwDistance = 0xFFFF;		//��ǰ���� �� ָ������ľ���
	DWORD dwPastDistance = 0xFFFF;	//��һ�ξ���,����ʱ��
	DWORD dwWaitTime = 0;	//Ѱ·���̺ķ�ʱ��(��)

	msg_RunToXY(x, y);
	while (g_cAutoPlay.LoopFlag)
	{
		Sleep(1000);
		dwWaitTime++;
		g_tPlayerObj.GetData();
		flCurPosX = g_tPlayerObj.flCurX;
		flCurPosY = g_tPlayerObj.flCurY;
		dwDistance = TwoPoint_Distance(flCurPosX, flCurPosY, (float)x, (float)y);
		DbgOutput("(%d,%d)Ѱ·��...,��ʣ%d��", x, y, dwDistance);
		if (dwDistance == dwPastDistance)	//����εľ������ϴξ������,��ʾ����
		{
			msg_RunToXY(x + rnd(micro), y + rnd(micro));	//������һ������
		}
		if (dwDistance < 20)		//�����򷵻�TRUE
		{
			DbgOutput("Ѱ·�ɹ�!,��ʱ:%d ��", dwWaitTime);
			return TRUE;
		}
		if (dwWaitTime > 6 * 60)	//6���ӻ�δ�����򷵻�FALSE
		{
			DbgOutput("Ѱ·ʧ��!,��ʱ:%d ��", dwWaitTime);
			return FALSE;
		}
		if (dwDistance >= 20)		//6������ �� δ�������¼����
		{
			dwPastDistance = dwDistance;	//��¼��һ�εľ���,��һ��ѭ����ȡ������ٱȽ϶���
		}
	}
	return FALSE;
}

BOOL HookRefreshScreen(BOOL IsDisableRefresh)
{
	BYTE NewBytes[] = { 0x90,0x90,0x90 };
	BYTE OldBytes[] = { 0x50,0xFF,0xD2 };
	BOOL bRet = 0;
	if (IsDisableRefresh)
		bRet = WriteProcessMemory((HANDLE)-1, (DWORD*)Base_HookRefreshScreen, NewBytes, 3, NULL);
	else
		bRet = WriteProcessMemory((HANDLE)-1, (DWORD*)Base_HookRefreshScreen, OldBytes, 3, NULL);
	return bRet;
}

int* GetRoleXY(int* Pos)
{
	msg_GetRoleXY(Pos);
	return Pos;
}

void UseGoodsByName(const char * szGoodName)
{
	msg_UseGoodsByName(szGoodName);
}

void UseSkillByName(const char * szSkillName)
{
	msg_UseSkillByName(szSkillName);
}

void SelNearMonster(const char * szMonName)
{
	msg_SelNearMonster(szMonName);
}

