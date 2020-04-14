#include <Windows.h>
#include "HookGameMainThread.h"
#include "global_Var.h"

HHOOK g_hhkGame;	//��Ϸ�����ӳ̾��
const DWORD MyMsgCode = RegisterWindowMessageA("MyMsgCode");	//��ϵͳ�����Ƿ���Ψһ��һ����Ϣ
LRESULT CALLBACK GameWndProc(
	int nCode,      // nCode��HC_ACTION�������ӳ̱��봦����Ϣ�����nCodeС��0�������ӳ̱��뽫��Ϣ���ݸ�CallNextHookEx������
	WPARAM wParam,  // ָ����Ϣ�Ƿ��ɵ�ǰ�̷߳��͡������Ϣ���ɵ�ǰ�̷߳��͵ģ���Ϊ����;�������㡣
	LPARAM lParam   // ָ������й���Ϣ����ϸ��Ϣ��CWPSTRUCT�ṹ��ָ�롣
)
{
	CWPSTRUCT* lpArg = (CWPSTRUCT*)lParam;
	if (nCode == HC_ACTION)
	{
		if (lpArg->hwnd == GetGameWndHandle() && lpArg->message == MyMsgCode)
		{
			//DbgOutput("��Ϣ����,wParam = %d\n", lpArg->wParam);
			switch (lpArg->wParam)
			{
			case MSG_UseGoodsByName:
				g_tBackList.UseGoodsByName((char *)(lpArg->lParam));
				break;

			case MSG_DropSkillToF1F10:
				g_tSkiList.DropSkillToF1F10((char*)(lpArg->lParam));
				break;

			case MSG_UseSkillByName:
				g_tScList.UseSkillByName((char *)(lpArg->lParam));
				break;

			case MSG_AutoBeatMonsterBySkill:
				g_tPlayerObj.AutoBeatMonsterBySkill((char *)(lpArg->lParam));
				break;

			case MSG_RunToXY:
			{
				int* iPosXY = (int*)(lpArg->lParam);
				g_tPlayerObj.RunToXY((float)(iPosXY[0]), (float)(iPosXY[1]));
			}
				break;

			case MSG_ChatWithNPC:
				g_tNearList.ChatWithNPC((char *)(lpArg->lParam));
				break;

			case MSG_EndChatWithNPC:
				g_tNearList.EndChatWithNPC((char *)(lpArg->lParam));
				break;

			case MSG_OpenShop:
				g_tNearList.OpenShop((char *)(lpArg->lParam));
				break;

			case MSG_BuyGoods:
			{
				Goods* pGoodsObj = (Goods*)(lpArg->lParam);
				g_tShopList.BuyGoodsByName(pGoodsObj->szName, pGoodsObj->nwIndex);
			}
				break;

			case MSG_SellGoods:
			{
				Goods* pGoodsObj = (Goods*)(lpArg->lParam);
				g_tShopList.SellGoodsByName(pGoodsObj->szName, pGoodsObj->nwIndex);
			}
				break;

			case MSG_CancelSel:
				g_tPlayerObj.CancelSel();
				break;

			case MSG_MoveGoodsToDepot:
			{
				Goods* pGoodsObj = (Goods*)(lpArg->lParam);
				if (pGoodsObj->nwIndex == 1)
					g_tBackList.MoveGoodsToDepot(pGoodsObj->szName);
				else
					g_tBackList.MoveGoodsToDepot(pGoodsObj->szName, pGoodsObj->nwIndex);
			}
				break;

			case MSG_MoveGoodsToBcpk:
			{
				Goods* pGoodsObj = (Goods*)(lpArg->lParam);
				if (pGoodsObj->nwIndex == 1)
					g_tDeptList.MoveGoodsToBackpack(pGoodsObj->szName);
				else
					g_tDeptList.MoveGoodsToBackpack(pGoodsObj->szName, pGoodsObj->nwIndex);
			}
				break;

			case MSG_PickupGoods:
				g_tGroundList.PickupGoods();
				break;

			case MSG_GetRoleXY:
			{
				int* Pos = (int*)(lpArg->lParam);
				g_tPlayerObj.GetData();
				Pos[0] = (int)g_tPlayerObj.flCurX;
				Pos[1] = (int)g_tPlayerObj.flCurY;
			}
				break;

			case MSG_SelNearMonster:
				g_tPlayerObj.SelNearMonster((char *)(lpArg->lParam));
				break;

			case MSG_TEST1:
				DbgOutput("��Ϣ����,MSG_TEST1");
				g_tNearList.PrintMsg();
				break;

			case MSG_TEST2:
				DbgOutput("��Ϣ����,MSG_TEST2");
				break;

			case MSG_TEST3:
				DbgOutput("��Ϣ����,MSG_TEST3"); 
				break;                              
			}
			return 1;
		}
	}
	return CallNextHookEx(g_hhkGame, nCode, wParam, lParam);
}

BOOL HookMainThread() {
	HWND hGame = GetGameWndHandle();
	DWORD dwThreadId = GetWindowThreadProcessId(hGame, NULL);
	if (dwThreadId != NULL)
	{
		g_hhkGame = SetWindowsHookExA(WH_CALLWNDPROC, GameWndProc, NULL, dwThreadId);
	}
	return TRUE;
}

BOOL UnHookMainThread() {
	UnhookWindowsHookEx(g_hhkGame);
	return TRUE;
}

//����ר����Ϣ
void msg_Test1(LPVOID lpData) 
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_TEST1, (LPARAM)lpData);
}

void msg_Test2(LPVOID lpData)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_TEST2, (LPARAM)lpData);
}

void msg_Test3(LPVOID lpData)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_TEST3, (LPARAM)lpData);
}

void msg_UseGoodsByName(const char* szName) 
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_UseGoodsByName, (LPARAM)szName);
}

void msg_DropSkillToF1F10(const char* szSkillName)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_DropSkillToF1F10, (LPARAM)szSkillName);
}

void msg_UseSkillByName(const char* szSkillName)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_UseSkillByName, (LPARAM)szSkillName);
}

void msg_AutoBeatMonsterBySkill(char* szSkillName)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_AutoBeatMonsterBySkill, (LPARAM)szSkillName);
}

void msg_RunToXY(int iX, int iY)
{
	int iPosXY[2] = { 0,0 };
	iPosXY[0] = iX;
	iPosXY[1] = iY;
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_RunToXY, (LPARAM)iPosXY);
}

void msg_ChatWithNPC(char* szNpcName)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_ChatWithNPC, (LPARAM)szNpcName);
}

void msg_EndChatWithNPC(char* szNpcName)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_EndChatWithNPC, (LPARAM)szNpcName);
}

void msg_OpenShop(char* szNpcName)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_OpenShop, (LPARAM)szNpcName);
}

void msg_BuyGoods(char* szName, WORD nwIndex)
{
	Goods GoodsObj;
	strcpy(GoodsObj.szName, szName);
	GoodsObj.nwIndex = nwIndex;
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_BuyGoods, (LPARAM)&GoodsObj);
}

void msg_SellGoods(char* szName, WORD nwIndex)
{
	Goods GoodsObj;
	strcpy(GoodsObj.szName, szName);
	GoodsObj.nwIndex = nwIndex;
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_SellGoods, (LPARAM)&GoodsObj);
}

void msg_CancelSel()
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_CancelSel, (LPARAM)NULL);
}

void msg_MoveGoodsToDepot(char* szName, WORD nwIndex)
{
	Goods GoodsObj;
	strcpy(GoodsObj.szName, szName);
	GoodsObj.nwIndex = nwIndex;
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_MoveGoodsToDepot, (LPARAM)&GoodsObj);
}

void msg_MoveGoodsToBcpk(char* szName, WORD nwIndex)
{
	Goods GoodsObj;
	strcpy(GoodsObj.szName, szName);
	GoodsObj.nwIndex = nwIndex;
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_MoveGoodsToBcpk, (LPARAM)&GoodsObj);
}

void msg_PickupGoods()
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_PickupGoods, (LPARAM)NULL);
}

void msg_GetRoleXY(int* Pos)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_GetRoleXY, (LPARAM)Pos);
}

void msg_SelNearMonster(const char* szMonName)
{
	::SendMessageA(GetGameWndHandle(), MyMsgCode, (WPARAM)MSG_SelNearMonster, (LPARAM)szMonName);
}


