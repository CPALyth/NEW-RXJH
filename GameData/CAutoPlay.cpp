#include "CAutoPlay.h"
#include "global_Var.h"
#include "StructGame.h"
#include "HookGameMainThread.h"
#include "GameFunction.h"
#include "TypeObj.h"

uniform_int_distribution<int> rndDelay(0, 200);//�������[0,200] ���ȷֲ�������
default_random_engine e;	//���������e,���������

CAutoPlay g_cAutoPlay;

CAutoPlay::CAutoPlay() {
	//����һ���߳���ר�Ÿ����̷߳�����Ϣ
	DWORD proc = NULL;
	__asm {
		mov eax, ThreadProc_AutoGuaJi
		mov proc, eax
	}
	ht_AutoGuaJi = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)proc, this, CREATE_SUSPENDED, NULL);
}

CAutoPlay::~CAutoPlay() {
	TerminateThread(ht_AutoGuaJi, 1);
}

/********************************************************************************************/
/* Description	: ���һ���Χ
/* Return		: ���������棬û�������ؼ�
/* Time			: 2020/02/17 21:17
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::CheckGuaJiRange()
{
	g_tPlayerObj.GetData();
	float PlayerCurPosX = g_tPlayerObj.flCurX;
	float PlayerCurPosY = g_tPlayerObj.flCurY;
	DWORD dwDistance = TwoPoint_Distance(PlayerCurPosX, PlayerCurPosY, (float)GuaJiPosX, (float)GuaJiPosY);
	DbgOutput("��һ������ %d��", dwDistance);
	if (dwDistance > RangeOfHitMonster)
	{
		msg_CancelSel();	//ȡ��ѡ�еĹ�
		DbgOutput("�����һ���Χ,�Զ����عһ���:%d,%d\n", GuaJiPosX, GuaJiPosY);
		FindWay(GuaJiPosX, GuaJiPosY);
		return TRUE;
	}
	return FALSE;
}

/* [2020/04/13 13:33]-[Remark: None] */
/* [�������Ƿ�����Ʒ�ɼ�]-[Return: ���򷵻�1,û�з���0] */
BOOL CAutoPlay::IsHaveThingToPick()
{
	g_tGroundList.GetData();
	for (int i = 0; i < Size_NearList; i++)	//����������Ʒ�б�
	{
		if (g_tGroundList.tlist[i].szName == NULL)	break;
		if (g_tGroundList.tlist[i].dwType == Type_GroundGoods)	return TRUE;
	}
	return FALSE;
}

/********************************************************************************************/
/* Description	: �ж��Ƿ���Ҫ�س�
/* Return		: ��Ҫ�سǷ����棬����Ҫ�سǷ��ؼ�
/* Time			: 2020/02/18 11:51
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::IsNeedBackToCity()
{
	g_tBackList.GetData();
	DWORD dwAllHpNum = g_tBackList.GetAllHpNum();
	DWORD dwAllMpNum = g_tBackList.GetAllMpNum();
	//HpҩƷ
	if (IsAutoBuyHpGoods)
	{
		if (dwAllHpNum < dwMinNum_HpGoods)
		{
			DbgOutput("HpҩƷ��������,��Ҫ�سǲ���\n");
			return TRUE;
		}
	}
	//MpҩƷ
	if (IsAutoBuyMpGoods)
	{
		if (dwAllMpNum < dwMinNum_MpGoods)
		{
			DbgOutput("MpҩƷ��������,��Ҫ�سǲ���\n");
			return TRUE;
		}
	}
	//������
	if (g_tBackList.IsBackpackFull())
	{
		DbgOutput("��������,��Ҫ�سǳ�����Ʒ,��Ųֿ�\n");
		return TRUE;
	}
	//����

	return FALSE;
}

/********************************************************************************************/
/* Description	: �سǲ���
/* Return		: �ɹ������棬ʧ�ܷ��ؼ�
/* Time			: 2020/02/18 15:48
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::BackToCitySupply()
{
	//ȡ��ѡ�ж���
	msg_CancelSel();
	
	//���̵� ���� �� ���� ��Ʒ
	GoToShopForSupply();

	//���ֿ� ��� �� ȡ�� ��Ʒ
	GoToDepotForSupply();

	//���ص��һ�����
	FindWay(GuaJiPosX, GuaJiPosY);
	return TRUE;
}

/********************************************************************************************/
/* Description	: ������Ʒ���жϸ���Ʒ�Ƿ���HpҩƷ
/* Return		: ���򷵻��棬���򷵻ؼ�
/* Time			: 2020/02/24 11:01
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::IsHpGoods(char* szName)
{
	if (strcmp(szName, "��ҩ(С)") == 0)	return TRUE;
	if (strcmp(szName, "��ҩ(��)") == 0)	return TRUE;
	if (strcmp(szName, "��ҩ(��)") == 0)	return TRUE;
	if (strcmp(szName, "��ҩ(��)") == 0)	return TRUE;
	if (strcmp(szName, "���ƽ�ҩ") == 0)	return TRUE;
	if (strcmp(szName, "ҩ�ɽ�ҩ") == 0)	return TRUE;
	return FALSE;
}

/********************************************************************************************/
/* Description	: ������Ʒ���жϸ���Ʒ�Ƿ���MpҩƷ
/* Return		: �� �򷵻��棬���򷵻ؼ�
/* Time			: 2020/02/24 11:06
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::IsMpGoods(char* szName)
{
	if (strcmp(szName, "�˲�") == 0)	return TRUE;
	if (strcmp(szName, "Ұ�˲�") == 0)	return TRUE;
	if (strcmp(szName, "ѩԭ��") == 0)	return TRUE;
	if (strcmp(szName, "��Ԫ��") == 0)	return TRUE;
	if (strcmp(szName, "������Ԫ��") == 0)	return TRUE;
	if (strcmp(szName, "ҩ����Ԫ��") == 0)	return TRUE;
	return FALSE;
}

BOOL CAutoPlay::IsStone(char * szName)
{
	if (strcmp(szName, "ǿ��ʯ") == 0)	return TRUE;
	if (strcmp(szName, "����ʯ") == 0)	return TRUE;
	if (strcmp(szName, "���ʯ") == 0)	return TRUE;
	if (strcmp(szName, "��Ѫʯ") == 0)	return TRUE;
	return FALSE;
}

/********************************************************************************************/
/* Description	: ���̵� ���� �� ���� ��Ʒ
/* Return		: �ɹ������棬ʧ�ܷ��ؼ�
/* Time			: 2020/02/22 16:07
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::GoToShopForSupply()
{
	//Ѱ·��ƽʮָ
	BOOL bRet = FindWay(737, 1955);
	if (bRet == FALSE)	return FALSE;
	Sleep(2000 + rndDelay(e));

	//��NPC�Ի�
	msg_ChatWithNPC("ƽʮָ");
	Sleep(2000 + rndDelay(e));

	//���̵�
	msg_OpenShop("ƽʮָ");
	Sleep(2000 + rndDelay(e));

	//���������趨��Ʒ���̵�
	SellSetGoodsToShop();
	Sleep(1000 + rndDelay(e));

	//���̵깺�������趨��Ʒ
	BuySetGoodsFromShop();
	Sleep(1000 + rndDelay(e));

	//������NPC�Ի�
	msg_EndChatWithNPC("ƽʮָ");
	Sleep(1000 + rndDelay(e));
	return TRUE;
}

/********************************************************************************************/
/* Description	: ���������趨��Ʒ���̵�
/* Return		: 
/* Time			: 2020/02/23 20:46
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::SellSetGoodsToShop()
{
	for (vector<TGoodsManage>::iterator it = vct_GoodsMngList.begin();
		it < vct_GoodsMngList.end(); it++)
	{
		if ((it->dwGoodsManageFlag) & Goods_Manage_Shop)
		{
			//��ѯ�����Ƿ��д���Ʒ,�еĻ�ȫ�����۵��̵�
			int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);
			if (iIndex >= 0)
			{
				DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
				msg_SellGoods(it->szGoodsName, dwNum);	//�����漰��CALL��Ҫ��װ����Ϣ
				Sleep(200);
			}
		}
	}
}

/********************************************************************************************/
/* Description	: ���̵깺�������趨��Ʒ
/* Return		: 
/* Time			: 2020/02/23 20:47
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::BuySetGoodsFromShop()
{
	for (vector<TSupplyData>::iterator it = vct_SupplyList.begin();
		it < vct_SupplyList.end(); it++)
	{
		if (strcmp(it->szAddr, "����ҩ��") == 0)
		{
			if (IsHpGoods(it->szGoodsName))	//��HpҩƷ����๺���趨�����ֵ
			{
				int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);	//��ѯ�����Ƿ��д���Ʒ
				if (iIndex >= 0)	
				{	//�����ȡ����
					DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
					if ((dwNum + it->dwNum) > dwMaxNum_HpGoods)	//���趨�����������ִ�����֮�� ���� �趨�����ֵ
						if (dwMaxNum_HpGoods > dwNum)
						{
							DbgOutput("�̵깺����Ʒ:%s,%d", it->szGoodsName, dwMaxNum_HpGoods - dwNum);
							msg_BuyGoods(it->szGoodsName, dwMaxNum_HpGoods - dwNum);	//ֻ�������ֵ
						}
				}
				else
				{	//û�������趨����
					DbgOutput("�̵깺����Ʒ:%s,%d", it->szGoodsName, it->dwNum);
					msg_BuyGoods(it->szGoodsName, it->dwNum);
				}
			}
			else if(IsMpGoods(it->szGoodsName))	//��MpҩƷ����๺���趨�����ֵ
			{
				
				int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);	//��ѯ�����Ƿ��д���Ʒ
				if (iIndex >= 0)	
				{	//�����ȡ����
					DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
					if ((dwNum + it->dwNum) > dwMaxNum_MpGoods)	//���趨�����������ִ�����֮�� ���� �趨�����ֵ
						if (dwMaxNum_MpGoods > dwNum)
						{
							DbgOutput("�̵깺����Ʒ:%s,%d", it->szGoodsName, dwMaxNum_MpGoods - dwNum);
							msg_BuyGoods(it->szGoodsName, dwMaxNum_MpGoods - dwNum);	//ֻ�������ֵ
						}
				}
				else
				{	//û�������趨����
					DbgOutput("�̵깺����Ʒ:%s,%d", it->szGoodsName, it->dwNum);
					msg_BuyGoods(it->szGoodsName, it->dwNum);
				}
			}
			else   //����HpMpҩƷ�����趨����
			{
				DbgOutput("�̵깺����Ʒ:%s,%d", it->szGoodsName, it->dwNum);
				msg_BuyGoods(it->szGoodsName, it->dwNum);	
			}
		}
	}
}

/********************************************************************************************/
/* Description	: ���ֿ� ��� �� ȡ�� ��Ʒ
/* Return		: �ɹ������棬ʧ�ܷ��ؼ�
/* Time			: 2020/02/22 16:08
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::GoToDepotForSupply()
{
	//Ѱ·��Τ��
	BOOL bRet = FindWay(151, 1786);
	if (bRet == FALSE)	return FALSE;
	Sleep(2000 + rndDelay(e));

	//��NPC�Ի�
	msg_ChatWithNPC("Τ��");
	Sleep(2000 + rndDelay(e));

	//�򿪲ֿ�
	msg_OpenShop("Τ��");
	Sleep(2000 + rndDelay(e));

	//����趨��Ʒ���ֿ�
	StoreSetGoodsToDepot();
	Sleep(1000 + rndDelay(e));

	//�Ӳֿ�ȡ�������趨��Ʒ
	PickSetGoodsFromDepot();
	Sleep(1000 + rndDelay(e));

	//������NPC�Ի�
	msg_EndChatWithNPC("Τ��");
	Sleep(1000 + rndDelay(e));
	return TRUE;
}

/********************************************************************************************/
/* Description	: ��������趨��Ʒ���ֿ�
/* Return		: �ɹ������棬ʧ�ܷ��ؼ�
/* Time			: 2020/02/22 19:46
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::StoreSetGoodsToDepot()
{
	for (int i=0; i<36; i++)	//��������
	{
		//�������󲻴���,����������ѭ��
		if (g_tBackList.tlist[i].dwGoodsObj == NULL)	continue;
		char* szName = g_tBackList.tlist[i].szName;	//��ȡ������i����Ʒ������
		for (vector<TGoodsManage>::iterator it = vct_GoodsMngList.begin();
			it < vct_GoodsMngList.end(); it++)	//�������� ��Ʒ�����б�
		{
			if (strcmp(szName,it->szGoodsName)==0)
			{
				if ( (it->dwGoodsManageFlag) & Goods_Manage_Depot )
				{
					//��ѯ�����Ƿ��д���Ʒ,�еĻ�ȫ���浽�ֿ�
					int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);
					if (iIndex >= 0)
					{
						DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
						msg_MoveGoodsToDepot(it->szGoodsName, dwNum);	//�����漰��CALL��Ҫ��װ����Ϣ
					}
				}
				break;	//�ҵ���Ӧ��Ʒ��,�Ͳ��ٱ��� ��Ʒ�����б�
			}
		}
		Sleep(50 + rndDelay(e));
	}
}

/********************************************************************************************/
/* Description	: �Ӳֿ�ȡ�������趨��Ʒ
/* Return		: �ɹ������棬ʧ�ܷ��ؼ�
/* Time			: 2020/02/22 19:49
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::PickSetGoodsFromDepot()
{
	for (vector<TSupplyData>::iterator it = vct_SupplyList.begin();
		it < vct_SupplyList.end(); it++)
	{
		if (strcmp(it->szAddr, "ȡ����Ʒ") == 0)
		{
			//��ѯ�ֿ��Ƿ��д���Ʒ
			int iIndex = g_tDeptList.GetGoodsIndexByName(it->szGoodsName);
			if (iIndex >= 0)
			{
				DWORD dwNum = g_tDeptList.GetGoodsNumByName(it->szGoodsName);
				//�ٱȽϲֿ�����Ʒ���� �� �趨Ҫȡ��������,ȡ���еĽ�Сֵ
				if (dwNum > it->dwNum)	dwNum = it->dwNum;
				msg_MoveGoodsToBcpk(it->szGoodsName, dwNum);
			}
		}
	}
}




/********************************************************************************************/
/* Description	: �Զ��һ��̺߳���
/* Return		: �ɹ������棬ʧ�ܷ��ؼ�
/* Time			: 2020/01/23 11:18
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::ThreadProc_AutoGuaJi(LPVOID lpData)
{
	__asm {
		mov ecx,lpData
		mov this,ecx
	}
	while (LoopFlag)
	{
		//�ж��Ƿ���Ҫ�سǲ���
		if (IsNeedBackToCity())
			BackToCitySupply();

		//����
		if (IsPickupGoods)
			if (IsHaveThingToPick())
			{
				msg_PickupGoods();
				Sleep(1000);
				msg_CancelSel();
			}
		Sleep(200 + rndDelay(e));

		//���㷶Χ���
		if (IsRangeLimit)
			CheckGuaJiRange();

		//���
		if (IsAutoBeatMonster)
		{
			if (IsUseSkill)
				msg_AutoBeatMonsterBySkill(szSkillName);
			else
				msg_AutoBeatMonsterBySkill("����");
		}
		DbgOutput("-----------------------------------------------------------");
		Sleep(200 + rndDelay(e));
	}
}

void CAutoPlay::LowHpMpProtect()
{
	g_tRoleProperty.GetData();
	g_tBackList.GetData();
	if (IsAutoUseHpGoods)
	{
		if (g_tRoleProperty.dwHpPercent <= dwHpPercent)
		{
			if (g_tBackList.GetGoodsIndexByName(szHpGoodsName) == -1)	// �����ѡ���ҩƷû��
			{
				strcpy_s(szHpGoodsName, "С��ת��(����)");
				if (g_tBackList.GetGoodsIndexByName(szHpGoodsName) == -1)	// �����ѡ���ҩƷû��
				{
					strcpy_s(szHpGoodsName, "��ת��(����)");
				}
			}
			msg_UseGoodsByName(szHpGoodsName);
		}
	}
	if (IsAutoUseMpGoods)
	{
		if (g_tRoleProperty.dwMpPercent <= dwMpPercent)
		{
			if (g_tBackList.GetGoodsIndexByName(szHpGoodsName) == -1)	// �����ѡ���ҩƷû��
			{
				strcpy_s(szHpGoodsName, "ǧ��ѩ��(����)");
			}
			msg_UseGoodsByName(szMpGoodsName);
		}
	}
}

void CAutoPlay::StartAutoPlay()
{
	LoopFlag = TRUE;
	ResumeThread(ht_AutoGuaJi);		//�ָ��߳�ִ��
}

void CAutoPlay::StopAutoPlay()
{
	LoopFlag = FALSE;
	SuspendThread(ht_AutoGuaJi);		//�����߳�
}
