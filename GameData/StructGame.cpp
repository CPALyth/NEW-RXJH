#include "pch.h"
#include "BaseGame.h"
#include "StructGame.h"
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "TypeObj.h"
#include "global_Var.h"
#include "HookGameMainThread.h"

TRoleProperty g_tRoleProperty;	//��ɫ����
TPlayerObj g_tPlayerObj;		//��Ҷ���

TBackpackList g_tBackList;	//�����б�
TDepotList g_tDeptList;		//�ֿ��б�
TShopList g_tShopList;		//�̵��б�
TNearList g_tNearList;		//��Χ�б�
TActionList g_tActList;		//�����б�
TSkillList g_tSkiList;		//�����б�
TShortcutList g_tScList;	//������б�
TGroundGoodsList g_tGroundList;	//������Ʒ�б�

DWORD dwShopBase = NULL;	// �̵��б��׵�ַ


void DbgOutput(const char *szFormat, ...) {
#ifdef _DEBUG
	char szbufFormat[0x1000];
	char szBufFormat_Game[0x1008] = "Game:";
	va_list argList;
	va_start(argList, szFormat);     //�����б��ʼ��
	vsprintf_s(szbufFormat, szFormat, argList);
	strcat_s(szBufFormat_Game, szbufFormat);
	OutputDebugStringA(szBufFormat_Game);
	va_end(argList);
#endif
}

/* [2020/01/18 13:33]-[Remark: None] */
/* [��ȡ��Ϸ���ھ��]-[�ɹ����ش��ھ�������򷵻�NULL] */
HWND GetGameWndHandle() {
	HWND hGame;
	__try
	{
		hGame = *(HWND *)Base_GameWndHandle;
	}
	__except (1)
	{
		DbgOutput("HWND GetGameWndHandle() �쳣\n");
		return NULL;
	}
	return hGame;
}

/* [2020/01/18 13:20]-[Remark: None] */
/* [�����������ľ���]-[������������_float x1, float y1, float x2, float y2] */
DWORD TwoPoint_Distance(float x1, float y1, float x2, float y2) {
	DWORD dwDistance = 0xFFFF;
	double a = double(x1 - x2);
	double b = double(y1 - y2);
	double c = sqrt(a * a + b * b);
	dwDistance = (DWORD)c;
	return dwDistance;
}

/* [2020/03/01 12:59]-[Remark: None] */
/* [�������ݵ�������]-[�ɹ������棬���򷵻ؼ�] */
BOOL WSASendData(LPCVOID lpData, DWORD dwBufSize)
{
	__try
	{
		__asm
		{
			push dwBufSize
			push lpData
			mov ecx, dword ptr ds : [Call_SendData_Ecx]
			mov eax, Call_SendData
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL WSASendData(LPCVOID lpData, DWORD dwBufSize) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 12:59]-[Remark: ����Ϊ�����б�ID] */
/* [��ӡ��ǰ�����б�]-[�޷���ֵ] */
void PrintAllTask(DWORD dwTaskListID)
{
	DWORD Reg_edi;
	DWORD dwEnd;
	DWORD Reg_esi;
	BYTE byLevel;
	char* szName;
	DWORD dw0x10;
	DWORD Reg_eax;
	DbgOutput("����void PrintAllTask(DWORD dwTaskListID)\n");
	__try
	{	//�ȵ��ѡ�
		__asm
		{
			mov ecx, Base_AllObjList
			mov ecx, dword ptr [ecx + 4 * 0x633]
			mov edx, dword ptr [ecx]
			mov edx, dword ptr [edx + 0x4]
			mov Reg_edi, edx
			push 0x0
			push dwTaskListID
			push 0x3F4
			call edx
		}
		DbgOutput("edx=%X\n", Reg_edi);
		DbgOutput("��ʼ���������,������ȡ����\n");
		Reg_esi = *(DWORD*)(*(DWORD*)(Base_Unknown) + 0x2BC);
		Reg_edi = *(DWORD*)(Reg_esi + 0x4F0);
		dwEnd = *(DWORD*)(Reg_esi + 0x4F4);
		DbgOutput("Reg_esi:%X\n", Reg_esi);
		DbgOutput("Reg_edi:%X\n", Reg_edi);
		DbgOutput("dwEnd:%X\n", dwEnd);
		while (Reg_edi != dwEnd)
		{
			//����ȼ�
			byLevel = *(BYTE*)(*(DWORD*)(Base_Unknown2) + (*(DWORD*)Reg_edi) * 0xD4 + 0x20);
			//��������
			Reg_eax = *(DWORD*)(Base_Unknown2) + (*(DWORD*)Reg_edi) * 0xD4 + 0x4;
			dw0x10 = *(DWORD*)(*(DWORD*)(Base_Unknown2) + (*(DWORD*)Reg_edi) * 0xD4 + 0x18);
			if (dw0x10 >= 0x10)	Reg_eax = *(DWORD*)Reg_eax;
			szName = (char*)Reg_eax;
			DbgOutput("[%d] %s\n", byLevel, szName);
			Reg_edi += 8;
		}
	}
	__except(1)
	{
		DbgOutput("void PrintAllTask(DWORD dwTaskListID) �쳣\n");
	}
}

TRoleProperty* TRoleProperty::GetData()
{
	__try
	{
		szRoleName = (char *)Base_RoleProperty;
		byLevel = *(BYTE *)(Base_RoleProperty + 0x34);
		byTransferNum = *(BYTE *)(Base_RoleProperty + 0x35);
		szRole���� = (char *)(Base_RoleProperty + 0x36);
		dwHp = *(DWORD *)(Base_RoleProperty + 0x80);
		dwMp = *(DWORD *)(Base_RoleProperty + 0x84);
		dwAngry = *(DWORD *)(Base_RoleProperty + 0x88);
		dwHpMax = *(DWORD *)(Base_RoleProperty + 0x8C);
		dwMpMax = *(DWORD *)(Base_RoleProperty + 0x90);
		dwAngryMax = *(DWORD *)(Base_RoleProperty + 0x94);
		nqExp = *(QWORD *)(Base_RoleProperty + 0x98);
		nqExpMax = *(QWORD *)(Base_RoleProperty + 0xA0);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xAC);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xB0);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xB4);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xB8);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xBC);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xC8);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xCC);
		dw���� = *(DWORD *)(Base_RoleProperty + 0xD0);
		dw�ر� = *(DWORD *)(Base_RoleProperty + 0xD4);
		nqMoney = *(QWORD *)(Base_RoleProperty + 0xE4);
		nw���� = *(WORD *)(Base_RoleProperty + 0xEC);
		nw����Max = *(WORD *)(Base_RoleProperty + 0xEE);
		byPoint���� = *(BYTE *)(Base_RoleProperty + 0xF0);
		for (int i = 0; i < 32; i++)
		{
			byQiGongPoints[i] = *(BYTE *)(Base_RoleProperty + 0xF4 + 4 * i);
		}
		dwHpPercent = 100*dwHp / dwHpMax;
		dwMpPercent = 100*dwMp / dwMpMax;
	}
	__except (1)
	{
		DbgOutput("TRoleProperty* TRoleProperty::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TRoleProperty::PrintMsg()
{
	GetData();
	DbgOutput("��ɫ����%s\n", szRoleName);
	DbgOutput("�ȼ���%d\n", byLevel);
	DbgOutput("תְ������%d\n", byTransferNum);
	DbgOutput("������%s\n", szRole����);
	DbgOutput("HP��%d/%d\n", dwHp,dwHpMax);
	DbgOutput("MP��%d/%d\n", dwMp,dwMpMax);
	DbgOutput("��ŭ��%d/%d\n", dwAngry,dwAngryMax);
	DbgOutput("Exp��%d/%d\n", nqExp, nqExpMax);
	DbgOutput("���أ�%d/%d\n", nw����, nw����Max);
	DbgOutput("����ֵ��%d\n", dw����);
	DbgOutput("�ģ�%d\n", dw����);
	DbgOutput("����%d\n", dw����);
	DbgOutput("�壺%d\n", dw����);
	DbgOutput("�꣺%d\n", dw����);
	DbgOutput("������%d\n", dw����);
	DbgOutput("������%d\n", dw����);
	DbgOutput("���У�%d\n", dw����);
	DbgOutput("�رܣ�%d\n", dw�ر�);
	DbgOutput("��Ǯ��%d\n", nqMoney);
	DbgOutput("δ���������㣺%d\n", byPoint����);
	for (int i=0;i<32;i++)
	{
		DbgOutput("����%d,�ѷ��������㣺%d\n", i, byQiGongPoints[i]);
	}
}

TBackpackList* TBackpackList::GetData()
{
	memset(tlist, 0, sizeof(tlist));
	__try
	{
		DWORD dwBase = *(DWORD *)Base_BackPackList;
		DWORD dwFirstGoodsBase = dwBase + 0x43C;
		DWORD dwObj = NULL;
		for (int i=0;i<36;i++)
		{
			dwObj = *(DWORD *)(dwFirstGoodsBase + 4 * i);	//ȡ����i������ַ
			tlist[i].dwGoodsObj = dwObj;
			if (tlist[i].dwGoodsObj == NULL)	continue;
			tlist[i].szName = (char *)(dwObj + 0x64);
			tlist[i].szMsg = (char *)(dwObj + 0x0F9);
			tlist[i].szAttribute = (char *)(dwObj + 0x238);
			tlist[i].nwNum = *(WORD *)(dwObj + 0xC4C);

			tlist[i].dwType1 = *(DWORD*)(dwObj + 0x54);
			tlist[i].nqID2 = *(QWORD*)(dwObj + 0x5C);
			tlist[i].byIndex = *(BYTE*)(dwObj + 0x1FC);
		}
	}
	__except (1)
	{
		DbgOutput("TBackpackList* TBackpackList::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TBackpackList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 36; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)
		{
			continue;
		}
		DbgOutput("��Ʒ����%s��:��Ʒ����=%d,��Ʒ��������=%s,��Ʒ����=%s\n",
			tlist[i].szName,
			tlist[i].nwNum,
			tlist[i].szMsg,
			tlist[i].szAttribute
			);
	}
}


/* [2020/03/01 13:00]-[Remark: None] */
/* [�����±�ʹ�ñ�����Ʒ]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::UseGoodsByIndex(DWORD dwIndex) {
	GetData();
	__try
	{ 
		__asm
		{
			mov eax, dwIndex
			push eax
			push 1
			push 0
			mov ecx, Base_BackPackList
			mov ecx, [ecx]
			mov eax, Call_UseObjForIndex
			call eax    //�����±�ʹ�ñ�����Ʒ �������׵�ַ
		}
	}
	__except (1)
	{
		DbgOutput("GameDebug:UseGoodsByIndex(DWORD dwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:00]-[Remark: None] */
/* [�������ֲ��ұ�������Ʒ����ȡ��Ʒ���ڱ����б���±�]-[�ɹ�������Ʒ���ڱ����б���±꣬ʧ�ܷ���-1] */
int TBackpackList::GetGoodsIndexByName(char *szName) {
	GetData();
	for (int i = 0;i<36; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName,tlist[i].szName) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* [2020/03/01 13:01]-[Remark: None] */
/* [ʹ�ñ�����Ʒ ��������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::UseGoodsByName(char *szName) {
	DWORD dwIndex = GetGoodsIndexByName(szName);	//ͨ�����ֲ���Ʒ��ȡ�±�
	if (dwIndex == -1)
	{
		return FALSE;
	}
	UseGoodsByIndex(dwIndex);	//ͨ���±�ʹ����Ʒ
	return TRUE;
}

/* [2020/03/01 13:01]-[Remark: ֻ����ѡ�ж����Ϊ��Ʒ���󣬵����������������Ʒ] */
/* [ѡ�б���ָ�������Ʒ]-[�ɹ�������Ʒ����ʧ�ܷ���NULL] */
DWORD TBackpackList::SelectGoods(DWORD dwIndex)
{
	DWORD dwObj = NULL;
	__try
	{
		__asm
		{
			mov ebx, dwIndex
			mov edi, dword ptr ds:[Base_BackPackList]
			mov edi, dword ptr ds:[edi + 0x43C + ebx*4]
			mov dwObj,edi
			mov eax, dword ptr ds:[Base_MouseSelObj]
			mov dword ptr ds:[eax + 0x230], edi
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::SelectGoods(DWORD dwIndex) �쳣\n");
		return NULL;
	}
	DbgOutput("ѡ�ж���dwObj = %X\n",dwObj);
	return dwObj;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [��ѯ����ָ����Ʒ����]-[�ɹ������棬���򷵻ؼ�] */
DWORD TBackpackList::GetGoodsNumByName(char* szName)
{
	for (int i = 0; i < 36; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return tlist[i].nwNum;
		}
	}
	return 0;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [��ѯ����HpҩƷ����]-[����HpҩƷ����] */
DWORD TBackpackList::GetAllHpNum()
{
	GetData();
	DWORD dwAllHpNum = 0;
	dwAllHpNum += GetGoodsNumByName("С��ת��(����)");
	dwAllHpNum += GetGoodsNumByName("��ת��(����)");
	if (dwAllHpNum > 0)
	{
		return 1000;	//ֻҪ�о�ת��,�Ͳ�ȱҩ,ֱ����ΪҩƷ������1000��
	}
	dwAllHpNum += GetGoodsNumByName("��ҩ(С)");
	dwAllHpNum += GetGoodsNumByName("��ҩ(��)");
	dwAllHpNum += GetGoodsNumByName("��ҩ(��)");
	dwAllHpNum += GetGoodsNumByName("��ҩ(��)");
	dwAllHpNum += GetGoodsNumByName("���ƽ�ҩ");
	dwAllHpNum += GetGoodsNumByName("ҩ�ɽ�ҩ");
	return dwAllHpNum;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [��ѯ����MpҩƷ����]-[����MpҩƷ����] */
DWORD TBackpackList::GetAllMpNum()
{
	GetData();
	DWORD dwAllMpNum = 0;
	dwAllMpNum += GetGoodsNumByName("ǧ��ѩ��(����)");
	if (dwAllMpNum > 0)
	{
		return 1000;	//ֻҪ��ǧ��ѩ��,�Ͳ�ȱҩ,ֱ����ΪҩƷ������1000��
	}
	dwAllMpNum += GetGoodsNumByName("�˲�");
	dwAllMpNum += GetGoodsNumByName("Ұɽ��");
	dwAllMpNum += GetGoodsNumByName("ѩԭ��");
	dwAllMpNum += GetGoodsNumByName("��Ԫ��");
	dwAllMpNum += GetGoodsNumByName("������Ԫ��");
	dwAllMpNum += GetGoodsNumByName("ҩ����Ԫ��");
	return dwAllMpNum;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [�жϱ����Ƿ���]-[�����������棬û�����ؼ�] */
BOOL TBackpackList::IsBackpackFull()
{
	for (int i=0; i < 36; i++)
	{
		if (tlist[i].nwNum == 0)
		{
			return FALSE;	//ֻҪ��һ��û����Ʒ,������û����
		}
	}
	return TRUE;	//������
}

/* [2020/03/01 13:03]-[Remark: �ɲ����Σ�Ĭ��Ϊ1] */
/* [�ƶ���ѡ����Ʒ���ֿ�]-[�ɹ������棬ʧ�ܷ��ؼ�] */
BOOL TBackpackList::MoveGoodsToDepot()
{
	__try
	{
		__asm
		{
			mov edi, dword ptr ds:[Base_DepotList]
			mov ecx, dword ptr ds:[edi+0x1634]
			mov edx, dword ptr ds:[edi+0x1C60]
			mov eax, 1
			push eax
			push ecx
			push edx
			
			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::MoveGoodsToDepot() �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:03]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ���ֿ�]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToDepot(char* szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToDepot();
	return bRet;
}

/* [2020/03/01 13:04]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ���ֿ�,��ָ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToDepot(char * szName, WORD nwIndex)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x94,0x00,0x8C,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x11,0x52,
		0x1C,0xEB,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xC5,0x57,0x0D,0x00,0x00,0x00,0x00,0x00,0x05,0x5E,0xB9,0xCA,0x17,0x0E,
		0xC4,0x06,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x54,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x20,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x21,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x21,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x21,0xEC,0x18,0x5B,0x21,0xEC,0x16,0x58,0x20,0xEC,0x15,0x56,
		0x20,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	TStorePickDepotData* pTStorePickDepotData = (TStorePickDepotData*)byData;
	
	__try 
	{
		pTStorePickDepotData->dwType1 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwSaveNum = nwIndex;
		pTStorePickDepotData->nqID2 = tlist[iIndex].nqID2;	//ע�⣺��λ�ֽ�Ҫ�ڸ�λ
		pTStorePickDepotData->dwType3 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwGoodsNum = tlist[iIndex].nwNum;
		pTStorePickDepotData->byIndex = tlist[iIndex].byIndex;
		for (int i = 0; i < 0xA; i++)
		{
			DbgOutput("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
				byData[i * 0x10 + 0x0], byData[i * 0x10 + 0x1], byData[i * 0x10 + 0x2], byData[i * 0x10 + 0x3],
				byData[i * 0x10 + 0x4], byData[i * 0x10 + 0x5], byData[i * 0x10 + 0x6], byData[i * 0x10 + 0x7],
				byData[i * 0x10 + 0x8], byData[i * 0x10 + 0x9], byData[i * 0x10 + 0xA], byData[i * 0x10 + 0xB],
				byData[i * 0x10 + 0xC], byData[i * 0x10 + 0xD], byData[i * 0x10 + 0xE], byData[i * 0x10 + 0xF]
			);
		}
		WSASendData(pTStorePickDepotData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::MoveGoodsToDepot(char * szName, DWORD dwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:04]-[Remark: None] */
/* [�ƶ�������ѡ����Ʒ��װ���б�ָ����]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip(DWORD dwIndex)
{
	__try
	{
		__asm
		{
			mov edi, dword ptr ds : [Base_EquipList]
			mov ecx, dword ptr ds : [edi + 0x1634]
			mov edx, dword ptr ds : [edi + 0x1C60]
			mov eax, dwIndex
			push eax
			push ecx
			push edx

			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::MoveGoodsToEquip(DWORD dwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:04]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ���·���]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_Clothes(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwClothes);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_HandL(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwHandL);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ���һ���]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_HandR(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwHandR);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_Weapon(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwWeapon);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ��Ь��]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_Shoes(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwShoes);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_Armor(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwArmor);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_Neck(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwNeck);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ�������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_EarL(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwEarL);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ���Ҷ���]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_EarR(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwEarR);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ�����ָ]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_RingL(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwRingL);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [�ƶ�����ָ�����ֵ���Ʒ��װ���ҽ�ָ]-[�ɹ������棬���򷵻ؼ�] */
BOOL TBackpackList::MoveGoodsToEquip_RingR(char* szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwRingR);
	return bRet;
}

TDepotList* TDepotList::GetData()
{
	memset(tlist, 0, sizeof(TDepotList));
	__try
	{
		DWORD dwBase = *(DWORD *)Base_DepotList;
		DWORD dwFirstGoodsBase = dwBase + 0x43C;
		DWORD dwObj = NULL;
		for (int i = 0; i < 60; i++)
		{
			dwObj = *(DWORD *)(dwFirstGoodsBase + 4 * i);	//ȡ����i������ַ
			tlist[i].dwGoodsObj = dwObj;
			if (tlist[i].dwGoodsObj == NULL)	continue;
			tlist[i].szName = (char *)(dwObj + 0x64);
			tlist[i].szMsg = (char *)(dwObj + 0x0F9);
			tlist[i].szAttribute = (char *)(dwObj + 0x238);
			tlist[i].nwNum = *(WORD *)(dwObj + 0xC4C);

			tlist[i].dwType1 = *(DWORD*)(dwObj + 0x54);
			tlist[i].nqID2 = *(QWORD*)(dwObj + 0x5C);
			tlist[i].byIndex = *(BYTE*)(dwObj + 0x1FC);
		}
	}
	__except (1)
	{
		DbgOutput("TDepotList* TDepotList::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TDepotList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)
		{
			continue;
		}
		DbgOutput("��Ʒ����%s��:��Ʒ����=%d,��Ʒ����=%X\n,��ƷID2=%X\n",
			tlist[i].szName,
			tlist[i].nwNum,
			tlist[i].dwType1,
			tlist[i].nqID2
		);
	}
}

/* [2020/03/01 13:07]-[Remark: None] */
/* [�������ֲ��Ҳֿ�����Ʒ����ȡ��Ʒ���ڲֿ��б���±�]-[�ɹ�������Ʒ���ڲֿ��б���±꣬ʧ�ܷ���-1] */
int TDepotList::GetGoodsIndexByName(char * szName)
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* [2020/03/01 13:07]-[Remark: None] */
/* [��ѯ�ֿ���Ʒ����]-[�ɹ������棬���򷵻ؼ�] */
DWORD TDepotList::GetGoodsNumByName(char * szName)
{
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return tlist[i].nwNum;
		}
	}
	return 0;
}

/* [2020/03/01 13:07]-[Remark: ֻ����ѡ�ж����Ϊ��Ʒ���󣬵����������������Ʒ] */
/* [ѡ�вֿ�ָ�������Ʒ]-[�ɹ�������Ʒ����ʧ�ܷ���NULL] */
DWORD TDepotList::SelectGoods(DWORD dwIndex)
{
	DWORD dwObj = NULL;
	__try
	{
		__asm
		{
			mov ebx, dwIndex
			mov edi, dword ptr ds : [Base_DepotList]
			mov edi, dword ptr ds : [edi + 0x43C + ebx * 4]
			mov dwObj, edi
			mov eax, dword ptr ds : [Base_MouseSelObj]
			mov dword ptr ds : [eax + 0x230], edi
		}
	}
	__except (1)
	{
		DbgOutput("DWORD TDepotList::SelectGoods(DWORD dwIndex) �쳣\n");
		return NULL;
	}
	DbgOutput("ѡ�ж���dwObj = %X\n", dwObj);
	return dwObj;
}

/* [2020/03/01 13:08]-[Remark: None] */
/* [�ƶ��ֿ���ѡ����Ʒ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TDepotList::MoveGoodsToBackPack()
{
	__try
	{
		__asm
		{
			mov edi, dword ptr ds : [Base_BackPackList]
			mov ecx, dword ptr ds : [edi + 0x1634]
			mov edx, dword ptr ds : [edi + 0x1C60]
			mov eax, 1
			push eax
			push ecx
			push edx

			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TDepotList::MoveGoodsToBackPack() �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:08]-[Remark: None] */
/* [�ƶ��ֿ�ָ�����ֵ���Ʒ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TDepotList::MoveGoodsToBackpack(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToBackPack();
	return bRet;
}

/* [2020/03/01 13:08]-[Remark: None] */
/* [�ƶ��ֿ�ָ�����ֵ���Ʒ������,��ָ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TDepotList::MoveGoodsToBackpack(char * szName, WORD nwIndex)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x94,0x00,0x8C,0x00,0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x53,0x84,0x0D,0x00,0x00,0x00,0x00,0x00,0xAD,0x11,0x0F,0x1D,0x1C,0x22,
		0xC4,0x06,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x85,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x08,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	TStorePickDepotData* pTStorePickDepotData = (TStorePickDepotData*)byData;
	__try
	{
		pTStorePickDepotData->dwType1 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwSaveNum = nwIndex;
		pTStorePickDepotData->nqID2 = tlist[iIndex].nqID2;	//ע�⣺��λ�ֽ�Ҫ�ڸ�λ
		pTStorePickDepotData->dwType3 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwGoodsNum = tlist[iIndex].nwNum;
		pTStorePickDepotData->byIndex = tlist[iIndex].byIndex;
		for (int i = 0; i < 0xA; i++)
		{
			DbgOutput("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
				byData[i * 0x10 + 0x0], byData[i * 0x10 + 0x1], byData[i * 0x10 + 0x2], byData[i * 0x10 + 0x3],
				byData[i * 0x10 + 0x4], byData[i * 0x10 + 0x5], byData[i * 0x10 + 0x6], byData[i * 0x10 + 0x7],
				byData[i * 0x10 + 0x8], byData[i * 0x10 + 0x9], byData[i * 0x10 + 0xA], byData[i * 0x10 + 0xB],
				byData[i * 0x10 + 0xC], byData[i * 0x10 + 0xD], byData[i * 0x10 + 0xE], byData[i * 0x10 + 0xF]
			);
		}
		WSASendData(pTStorePickDepotData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TDepotList::MoveGoodsToBackpack(char * szName, WORD nwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/04/13 18:33]-[Remark: �̵��б��׵�ַ: [[dwShopBase]+4]+0x43C+4*0    ] */
/* [��ȡ�̵��б��ַ]-[Return:None] */
DWORD TShopList::GetShopBase()
{
	if (dwShopBase != NULL)	return dwShopBase;	// ֮ǰ�Ѿ��ҵ��̵��б��׵�ַ
	// �̵��б��׵�ַ��[[Base_ShopList+4D2*4+4]+4]+0x43C+4*0,�̵��б��ַÿ�ν���Ϸ���ܲ�һ��,���� 0x4D2 ����
	DWORD dwBase = NULL;
	DWORD dwObj = NULL;

	for (DWORD i = 0x4D2 - 0x100; i < 0x4D2 + 0x100; ++i)
	{
		__try
		{
			DbgOutput("i = %X\n", i);
			dwBase = Base_ShopList + 4 * i;
			dwObj = *(DWORD*)dwBase;
			if (dwObj == NULL)	continue;
			dwObj = *(DWORD*)(dwObj + 4);
			if (dwObj == NULL)	continue;
			dwObj = *(DWORD*)(dwObj + 0x43C + 4 * 0);	// ȡ�̵��б�ĵ�һ��
			if (dwObj == NULL)	continue;

			DWORD dwType = *(DWORD*)(dwObj + offset_Obj_Type);
			if (dwType == Type_GoodsObj)
			{
				WORD nwNum = *(WORD*)(dwObj + 0xC4C);
				if (nwNum == 0xFFFF)
				{
					dwShopBase = dwBase;
					DbgOutput("��ȡ�̵��б��ַ�ɹ�,�̵��б��׵�ַ: %X\n", dwShopBase);
					return dwShopBase;
				}
			}
		}
		__except (1)
		{
			continue;
		}
	}
	DbgOutput("��ȡ�̵��б��ַʧ��\n");
	return NULL;
}

TShopList* TShopList::GetData()
{
	memset(tlist, 0, sizeof(TShopList));
	GetShopBase();
	__try
	{
		DWORD dwObj = NULL;
		for (int i = 0; i < 60; i++)
		{
			//ȡ����i������ַ
			dwObj = *(DWORD *)(dwShopBase);	
			dwObj = *(DWORD *)(dwObj + 4);
			dwObj = *(DWORD *)(dwObj + 0x43C + 4*i);

			tlist[i].dwGoodsObj = dwObj;
			if (tlist[i].dwGoodsObj == NULL)	return this;	//�̵���Ʒ��������ŵ�
			tlist[i].szName = (char *)(dwObj + 0x64);
			tlist[i].szMsg = (char *)(dwObj + 0x0F9);
			tlist[i].szAttribute = (char *)(dwObj + 0x238);
			tlist[i].nwNum = *(WORD *)(dwObj + 0xC4C);

			tlist[i].dwType1 = *(DWORD*)(dwObj + 0x54);
			tlist[i].nqID2 = *(QWORD*)(dwObj + 0x5C);
			tlist[i].byIndex = *(BYTE*)(dwObj + 0x1FC);
		}
	}
	__except (1)
	{
		DbgOutput("TShopList* TShopList::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TShopList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)
		{
			return;
		}
		DbgOutput("��Ʒ����%s��:��Ʒ����=%X\n,��ƷID2=%X\n",
			tlist[i].szName,
			tlist[i].dwType1,
			tlist[i].nqID2
		);
	}
}

/* [2020/03/01 13:09]-[Remark: None] */
/* [�������ֲ����̵�����Ʒ]-[�ɹ�������Ʒ�����̵��б���±꣬ʧ�ܷ���-1] */
int TShopList::GetGoodsIndexByName(char* szName)
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	return -1;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* [2020/03/01 13:09]-[Remark: None] */
/* [ͨ����Ʒ���֣�����ָ����������Ʒ]-[�ɹ������棬���򷵻ؼ�] */
BOOL TShopList::BuyGoodsByName(char* szName, WORD nwIndex)
{
	int iIndex = GetGoodsIndexByName(szName);
	DbgOutput("��Ʒ�±�Ϊ%d\n",iIndex);
	if (iIndex < 0)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x92,0x00,0x88,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x65,0xCA,
		0x9A,0x3B,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};
	TBuySellData* pTBuySellData = (TBuySellData*)byData;
	__try
	{
		pTBuySellData->dwCmd = 0x00880092;
		pTBuySellData->dwBuySellType = BuyGoods;
		pTBuySellData->dwType1 = tlist[iIndex].dwType1;
		pTBuySellData->nwGoodsNum1 = nwIndex;
		WSASendData(pTBuySellData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TShopList::BuyGoodsByName(char* szName, WORD nwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:09]-[Remark: None] */
/* [ͨ����Ʒ���֣�����ָ����������Ʒ]-[�ɹ������棬���򷵻ؼ�] */
BOOL TShopList::SellGoodsByName(char * szName, WORD nwIndex)
{
	int iIndex = g_tBackList.GetGoodsIndexByName(szName);
	DbgOutput("��Ʒ�ڱ����б��±�Ϊ%d\n", iIndex);
	if (iIndex < 0)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x92,0x00,0x88,0x00,0x02,0x00,0x00,0x00,0x14,0x56,0x1B,0xEB,0x65,0xCA,
		0x9A,0x3B,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x71,
		0x0D,0x00,0x00,0x00,0x00,0x00,0x45,0xB4,0x87,0x55,0xAB,0x0D,0xC4,0x06,0x65,0xCA,
		0x9A,0x3B,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
		0x00,0x00,0x01,0x00,0x16,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x17,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,
		0x17,0xEC,0x0D,0x52,0x18,0xEC,0x0D,0x52,0x18,0xEC,0x0E,0x53,0x19,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};
	TBuySellData* pTBuySellData = (TBuySellData*)byData;
	__try
	{
		pTBuySellData->dwCmd = 0x00880092;
		pTBuySellData->dwBuySellType = SellGoods;
		pTBuySellData->dwType1 = g_tBackList.tlist[iIndex].dwType1;
		pTBuySellData->nwGoodsNum1 = nwIndex;
		pTBuySellData->nqID2 = g_tBackList.tlist[iIndex].nqID2;
		pTBuySellData->dwType2 = g_tBackList.tlist[iIndex].dwType1;
		pTBuySellData->nwGoodsNum2 = nwIndex;
		pTBuySellData->byIndex = g_tBackList.tlist[iIndex].byIndex;
		WSASendData(pTBuySellData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TShopList::SellGoodsByName(char * szName, WORD nwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}


TNearList* TNearList::GetData() {
	memset(tlist, 0, sizeof(tlist));
	__try
	{
		DWORD dwObj = NULL;
		for (int i = 0; i< Size_NearList; i++)
		{
			dwObj = *(DWORD*)(Base_NearObjList + 4 * i);
			tlist[i].dwNearObj = dwObj;
			if (dwObj == NULL )	//��Χ���󲻴���
			{
				continue; 
			}

			tlist[i].dwType = *(DWORD*)(dwObj + 0x8);
			tlist[i].dwIndexInAllObj = *(DWORD*)(dwObj + 0xC);
			g_tPlayerObj.GetData();
			
			if (tlist[i].dwType == Type_Monster)
			{
				tlist[i].szName = (char*)(dwObj + 0x360);	//NPC���������
			}
			else
			{
				tlist[i].szName = (char*)(dwObj + 0x18);	//�������
				continue;
			}
			//NPC�����ר������
			tlist[i].IsSelected = *(DWORD *)(dwObj + 0x354);
			tlist[i].flDistance = *(float*)(dwObj + 0x35C);
			tlist[i].IsDead = *(BYTE *)(dwObj + 0x3C0);
			tlist[i].IsAbleKill = *(BYTE *)(dwObj + 0x3C8);
			tlist[i].dwHP = *(DWORD *)(dwObj + 0x5F4);
			tlist[i].dwLevel = *(DWORD *)(dwObj + 0x5F8);
			tlist[i].flX = *(float *)(dwObj + 0x1060);
			tlist[i].flY = *(float *)(dwObj + 0x1068);
		}
	}
	__except (1)
	{
		DbgOutput("TNearList* TNearList::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TNearList::PrintMsg() {
	GetData();
	for (int i=0;i< Size_NearList;i++)
	{
		if (tlist[i].dwNearObj == NULL)
		{
			continue;
		}
		if (tlist[i].dwType != Type_Monster)
			DbgOutput("%s:����=%X,ID=%X", tlist[i].szName, tlist[i].dwNearObj, tlist[i].dwIndexInAllObj);
		else
			DbgOutput("%s:����=%X,ID=%X,�ȼ�=%d,HP=%d,����=(%f,%f),IsDead=%d,IsAbleKill=%d,IsSelected=%d��flDistance=%f\n",
						tlist[i].szName,
						tlist[i].dwNearObj,
						tlist[i].dwIndexInAllObj,
						tlist[i].dwLevel,
						tlist[i].dwHP,
						tlist[i].flX,
						tlist[i].flY,
						tlist[i].IsDead,
						tlist[i].IsAbleKill,
						tlist[i].IsSelected,
						tlist[i].flDistance);
	}
}

/* [2020/03/01 13:11]-[Remark: ��1CA4] */
/* [����NPC���֣����NPC��ID]-[�ɹ������棬���򷵻ؼ�] */
DWORD TNearList::GetNpcID(char* szName)
{
	GetData();	//��ʼ����Χ�����б�
	DbgOutput("�����NPC���֣�%s\n", szName);
	for (int i = 0; i < Size_NearList; i++)
	{
		if (tlist[i].dwNearObj == NULL)
		{
			break;
		}
		DbgOutput("%d,����=%s\n", i, tlist[i].szName);
		if ( strcmp(tlist[i].szName, szName) == 0 )
		{
			DbgOutput("����ƥ��ɹ�\n");
			DWORD dwID = tlist[i].dwIndexInAllObj;
			DbgOutput("NPC��ID=%d\n", dwID);
			return dwID;
		}
	}
	return NULL;
}

/* [2020/03/01 13:11]-[Remark: None] */
/* [����NPC���֣���ָ��NPC�Ի�]-[�ɹ������棬���򷵻ؼ�] */
BOOL TNearList::ChatWithNPC(char* szName)
{
	DWORD dwIndex = GetNpcID(szName);
	DbgOutput("ChatWithNPC NPC��ID=%d\n", dwIndex);
	if (dwIndex == NULL)	return FALSE;
	g_tPlayerObj.SelObj(dwIndex);
	__try
	{
		__asm
		{
			mov eax, dwIndex
			mov ecx, dword ptr ds : [eax * 4 + Base_AllObjList]
			mov edx, dword ptr ds : [ecx]
			mov eax, dword ptr ds : [edx + 0x4]
			push 0
			push 0
			push 0x401
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TNearList::ChatWithNPC(char* szName) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:12]-[Remark: 5�򿪲ֿ⣬4ȷ������3���̵�] */
/* [����Ի���˵�ѡ��]-[�ɹ������棬���򷵻ؼ�] */
BOOL TNearList::ClickChatMenuOption(DWORD dwIndex)
{
	__try
	{
		__asm
		{
			mov ecx, dword ptr ds : [Base_Unknown]
			mov ecx, dword ptr ds : [ecx + 0x2EC]

			push dwIndex
			mov eax, Call_ClickChatMenuOption
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL ClickChatMenuOption(DWORD dwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

TActionList* TActionList::GetData() {
	DWORD dwFirstObjAddr = NULL;	//��һ������ĵ�ַ
	DWORD dwObj = NULL;
	__try
	{
		dwFirstObjAddr = *(DWORD*)(Base_ActionList) + 0x43C;
		for (int i=0;i<12;i++)
		{
			dwObj = *(DWORD *)(dwFirstObjAddr + 4 * i);
			tlist[i].dwActionID = *(DWORD*)(dwObj + 0x54);
			tlist[i].szName = (char *)(dwObj + 0x64);
		}
	}
	__except (1)
	{
		DbgOutput("TActionList* TActionList::GetData() �쳣\n");
	}
	return this;
}

/* [2020/03/01 13:12]-[Remark: None] */
/* [������NPC�Ի�]-[�ɹ������棬���򷵻ؼ�] */
BOOL TNearList::EndChatWithNPC(char* szNpcName)
{
	DWORD byData[0x24] = { 0 };	//0x24 * 4 = 0x90ȫ����ʼ��Ϊ0
	byData[0] = 0x00900000;
	byData[1] = 0x00020018;
	if (strcmp(szNpcName, "Τ��") == 0)
	{
		byData[3] = 0x00010000;
	}
	else if (strcmp(szNpcName, "����Ц") == 0)
	{
		byData[3] = 0x00020000;
	}
	else if (strcmp(szNpcName, "ƽʮָ") == 0)
	{
		byData[3] = 0x00030000;
	}
	BOOL bRet = WSASendData(byData, 0x1E);
	return bRet;
}

/* [2020/03/01 13:12]-[Remark: None] */
/* [�򿪲ֿ�(Τ��),���/����(ƽʮָ,����Ц)]-[�ɹ������棬���򷵻ؼ�] */
BOOL TNearList::OpenShop(char* szNpcName)
{
	DWORD byData[0x24] = { 0 };	//0x24 * 4 = 0x90ȫ����ʼ��Ϊ0
	byData[0] = 0x00900000;
	byData[5] = 0x00010000;
	if (strcmp(szNpcName,"Τ��") == 0)
	{
		byData[1] = 0x00050018;
		byData[3] = 0x00010000;
	}
	else if (strcmp(szNpcName, "����Ц") == 0)
	{
		byData[1] = 0x00030018;
		byData[3] = 0x00020000;
	}
	else if (strcmp(szNpcName, "ƽʮָ") == 0)
	{
		byData[1] = 0x00030018;
		byData[3] = 0x00030000;
	}
	BOOL bRet = WSASendData(byData, 0x1E);
	return bRet;
}

void TActionList::PrintMsg() {
	GetData();
	for (int i = 0; i < 12; i++)
	{
		DbgOutput("%s[%d] ID=%d\n", tlist[i].szName, i, tlist[i].dwActionID);
	}
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [���ݶ����б���±�ʹ�ö���]-[�ɹ������棬���򷵻ؼ�] */
BOOL TActionList::UseActionByIndex(DWORD dwIndex) {
	GetData();
	DWORD dwEcx = Call_ActionUse_Ecx;
	DWORD dwArg1 = tlist[dwIndex].dwActionID;
	__try
	{
		__asm {
			mov ecx, dwArg1	//ȡ ����ID �� ecx
			push ecx		//�� ����ID ��ջ
			mov ecx, dwEcx
			mov edx, Call_ActionUse
			call edx
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TActionList::UseAction(DWORD dwIndex) �쳣\n");
	}
	return TRUE;
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [���ݶ�����ʹ�ö������磺��������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TActionList::UseActionByName(char * szActionName) {
	GetData();
	for (int i =0;i<12;i++)
	{
		//DbgOutput(tlist[i].szName);
		if (strcmp(tlist[i].szName, szActionName) == 0)
		{
			UseActionByIndex(i);
			return TRUE;
		}
	}
	return FALSE;
}

TPlayerObj* TPlayerObj::GetData() {
	__try
	{
		DWORD dwObj = *(DWORD*)Base_PlayerObj;
		dwSelIndex = *(DWORD*)(dwObj + offset_PlayerObj_dwSelIndex);
		flDesX = *(float*)(dwObj + offset_PlayerObj_flDesX);
		flDesY = *(float*)(dwObj + offset_PlayerObj_flDesX + 8);
		flCurX = *(float*)(dwObj + offset_PlayerObj_flCurX);
		flCurY = *(float*)(dwObj + offset_PlayerObj_flCurX + 8);
	}
	__except (1)
	{
		DbgOutput("TPlayerObj* TPlayerObj::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TPlayerObj::PrintMsg() {
	GetData();
	DbgOutput("���ѡ�ж���ID=%X,��ҵ�ǰ����(X:%f,Y:%f),���Ŀ������(X:%f,Y:%f)", dwSelIndex,
				flCurX,flCurY,flDesX,flDesY);
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [ѡ����Χ����]-[�ɹ������棬���򷵻ؼ�] */
BOOL TPlayerObj::SelObj(DWORD dwIndexInAllObj)
{
	GetData();
	DWORD dwObjRole;
	__try 
	{
		__asm {
			//ȡѡ�й���ID
			mov edi, dword ptr ds : [Base_PlayerObj]
			mov eax, dword ptr ds : [edi + offset_PlayerObj_dwSelIndex]
			cmp eax, 0xFFFF       //�Ƿ�ѡ�й���
			jz NEXTLABEL  //eax==0xFF,��ζ��ûѡ��,ֱ������NEXTLABEL
			//���ѡ�ж���
			mov ecx, dword ptr ds : [Base_AllObjList + eax * 4]
			mov edx, dword ptr ds : [ecx]
			mov eax, dword ptr ds : [edx + 0x04]
			push 0
			push 0 //ȡ��ѡ��
			push 0x450
			call eax
		}
	NEXTLABEL:
		//���µ����ѡ�й����ID ���� [ [Base_PlayerObj]+offset_PlayerObj_dwSelIndex ]
		dwObjRole = *(DWORD*)Base_PlayerObj;
		*(DWORD*)(dwObjRole + offset_PlayerObj_dwSelIndex) = dwIndexInAllObj;       //���Σ�������ASM����
		__asm {
			//����ļĴ������������ʱ�����գ��޷�������Ч����Ҫ�ٸ�ֵ
			mov edi, dword ptr ds : [Base_PlayerObj]
			mov eax, dword ptr ds : [edi + offset_PlayerObj_dwSelIndex]    //eax=��ѡ�й���ID
			//ȡ���¶���
			mov ecx, dword ptr ds : [Base_AllObjList + eax * 4]
			mov edx, dword ptr ds : [ecx]
			mov eax, dword ptr ds : [edx + 0x04]
			push 0
			push 1 //��ʾѡ��
			push 0x450
			call eax
		}
	}
	__except(1) 
	{
		DbgOutput("BOOL TPlayerObj::SelObj(DWORD dwIndexInAllObj) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [ȡ��ѡ����Χ����]-[�ɹ������棬���򷵻ؼ�] */
BOOL TPlayerObj::CancelSel()
{
	DWORD dwObjRole = NULL;
	__try
	{
		dwObjRole = *(DWORD*)Base_PlayerObj;
		*(DWORD*)(dwObjRole + offset_PlayerObj_dwSelIndex) = 0xFFFF;
	}
	__except(1)
	{
		DbgOutput("BOOL TPlayerObj::CancelSel() �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:14]-[Remark: None] */
/* [ѡ�����������Ĺ���]-[�ɹ������棬���򷵻ؼ�] */
BOOL TPlayerObj::SelNearMonster()
{
	GetData();
	DbgOutput("����SelNearMonster\n");
	DWORD dwIndex = 0xFFFF;
	float flMinDistance = 9999.9F;
	g_tNearList.GetData();
	for (int i=0;i< Size_NearList;i++)	//����������Χ�б��ҵ������������Χ�����ID
	{
		if (g_tNearList.tlist[i].dwNearObj == NULL)
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwLevel == 0)	//����NPC
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwType != Type_Monster)	//�������,����,������Ʒ��
		{
			continue;
		}
		if (g_tNearList.tlist[i].flDistance < flMinDistance)
		{
			flMinDistance = g_tNearList.tlist[i].flDistance;	
			dwIndex = g_tNearList.tlist[i].dwIndexInAllObj;	//��dwIndex = �����������Χ����ID
			DbgOutput("����ID��%X��������룺%f\n",dwIndex, flMinDistance);
		}
	}
	if (dwIndex == 0xFFFF)
	{
		return FALSE;
	}
	BOOL bRet = 0;
	bRet = SelObj(dwIndex);
	DbgOutput("SelNearMonster����ֵ��%d\n", bRet);
	return bRet;
}

/* [2020/04/12 11:01]-[Remark: None] */
/* [���ݹ�����ѡ������Ĺ���]-[Return:None] */
BOOL TPlayerObj::SelNearMonster(const char* szMonName)
{
	if (szMonName == NULL)	return SelNearMonster();

	GetData();
	DbgOutput("����SelNearMonster(const char* szMonName)\n");
	DWORD dwIndex = 0xFFFF;
	float flMinDistance = 9999.9F;
	g_tNearList.GetData();
	for (int i = 0; i < Size_NearList; i++)	//����������Χ�б��ҵ������������Χ�����ID
	{
		if (g_tNearList.tlist[i].dwNearObj == NULL)
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwLevel == 0)	//����NPC
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwType != Type_Monster)	//�������,����,������Ʒ��
		{
			continue;
		}
		if (strcmp(g_tNearList.tlist[i].szName, szMonName) == 0)
		{
			DbgOutput("�ҵ�������:%s\n", szMonName);
			if (g_tNearList.tlist[i].flDistance < flMinDistance)
			{
				flMinDistance = g_tNearList.tlist[i].flDistance;
				dwIndex = g_tNearList.tlist[i].dwIndexInAllObj;	//��dwIndex = �����������Χ����ID
				DbgOutput("����ID��%X��������룺%f\n", dwIndex, flMinDistance);
			}
		}
		
	}
	if (dwIndex == 0xFFFF)
	{
		return FALSE;
	}
	BOOL bRet = 0;
	bRet = SelObj(dwIndex);
	DbgOutput("SelNearMonster����ֵ��%d\n", bRet);
	return bRet;
}

/* [2020/03/01 13:15]-[Remark: None] */
/* [�Զ��չ����]-[�ɹ������棬���򷵻ؼ�] */
BOOL TPlayerObj::AutoBeatMonster()
{
	BOOL bRet = 1;
	
	if (GetSelObjType() != Type_Monster)	//�жϵ�ǰ�Ƿ�ѡ�й���,ûѡ�о�ѡ����Ĺ�
	{
		bRet = SelNearMonster();
	}
	
	if (bRet)	//ѡ�˾�ֱ�ӹ���
	{
		bRet = g_tActList.UseActionByName("����");	//ʹ���չ�
		if (bRet)	return TRUE;
	}
	return FALSE;
}

/* [2020/03/01 13:15]-[Remark: None] */
/* [�Զ�ʹ�ü��ܴ��]-[�ɹ������棬���򷵻ؼ�] */
BOOL TPlayerObj::AutoBeatMonsterBySkill(char * szSkillName)
{
	BOOL bRet = SelNearMonster();
	if (bRet)	//ѡ�˾�ֱ�ӹ���
	{
		if (g_tPlayerObj.IsSelObjAbleKill())
			bRet = g_tScList.UseSkillByName("��ħ�ᴩ");	//ʹ�ñ�ɱ��
		else
			bRet = g_tScList.UseSkillByName(szSkillName);	//ʹ�ü���
		DbgOutput("ʹ�ü��ܽ����%d", bRet);
		if (bRet)	return TRUE;
		else
		{
			bRet = g_tActList.UseActionByName("����");	//ʹ���չ�
			if (bRet)	return TRUE;
		}
	}
	return FALSE;
}

/* [2020/03/01 13:16]-[Remark: None] */
/* [��ȡ���ѡ�ж��������]-[�ɹ����ض������ͣ�ʧ���򷵻�NULL] */
DWORD TPlayerObj::GetSelObjType()
{
	GetData();
	DWORD dwType = NULL;
	DWORD dwCurObj = NULL;
	if (dwSelIndex == 0xFFFF)
	{
		return NULL;
	}
	__try
	{
		//���� ��ǰѡ��ID ȡ�� ѡ�ж���
		dwCurObj = *(DWORD*)(Base_AllObjList + 4 * dwSelIndex);
		dwType = *(DWORD*)(dwCurObj + 0x8);
		DbgOutput("ѡ�ж���ID��%X��ѡ�ж���%X�����ͣ�%X\n", dwSelIndex, dwCurObj, dwType);
	}
	__except (1)
	{
		DbgOutput("DWORD TPlayerObj::GetSelObjType() �쳣\n");
		return NULL;
	}
	return dwType;
}

BOOL TPlayerObj::IsSelObjDead()
{
	GetData();
	BOOL isDead = FALSE;
	DWORD dwCurObj = NULL;
	if (dwSelIndex == 0xFFFF)
	{
		return FALSE;
	}
	__try
	{
		//���� ��ǰѡ��ID ȡ�� ѡ�ж���
		dwCurObj = *(DWORD*)(Base_AllObjList + 4 * dwSelIndex);
		isDead = *(BYTE*)(dwCurObj + 0x3C0);
		DbgOutput("ѡ�ж���ID��%X��ѡ�ж���%X��������%d\n", dwSelIndex, dwCurObj, isDead);
	}
	__except (1)
	{
		DbgOutput("BOOL TPlayerObj::IsSelObjDead() �쳣\n");
		return NULL;
	}
	return isDead;
}

/* [2020/04/14 09:54]-[Remark: None] */
/* [ѡ��Ĺ����Ƿ�ɱ���ɱ]-[Return:None] */
BOOL TPlayerObj::IsSelObjAbleKill()
{
	GetData();
	g_tNearList.GetData();
	if (dwSelIndex == 0xFFFF)	return FALSE;
	DWORD indexList = 0;
	for(DWORD i = 0; i < Size_NearList; ++i)
	{
		if (g_tNearList.tlist[i].dwIndexInAllObj == dwSelIndex)
		{
			indexList = i;
		}
	}
	BYTE byRet = g_tNearList.tlist[indexList].IsAbleKill;
	if (byRet)
		DbgOutput("����ɱ���ɱ,byRet=%d,dwSelIndex=%X,indexList=%d\n",byRet, dwSelIndex, indexList);
	else
		DbgOutput("���ﲻ�ɱ���ɱ\n");
	return byRet;
}

/* [2020/03/01 13:16]-[Remark: ����103��ʱ����] */
/* [�ܵ�ָ������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TPlayerObj::RunToXY(float flx, float fly)
{
	__try
	{
		__asm 
		{
			//��ʾ���Ѱ·���: [[[Base_Unknown]+2B0]+230] = 1
			mov eax, Base_Unknown
			mov eax, [eax]
			mov eax, [eax + 0x2B0]
			mov byte ptr ds : [eax + 0x230], 1
			//Ѱ·
			sub esp, 0x54
			mov eax, esp
			mov ecx, flx
			mov edx, fly
			mov dword ptr ss : [eax + 0x00], ecx
			mov dword ptr ss : [eax + 0x04], 0x00000000
			mov dword ptr ss : [eax + 0x08], edx
			mov dword ptr ss : [eax + 0x0C], 0x00000000
			mov dword ptr ss : [eax + 0x10], 0x00000000
			mov dword ptr ss : [eax + 0x14], 0x00000000
			mov dword ptr ss : [eax + 0x18], 0x0000FFFF
			mov dword ptr ss : [eax + 0x1C], 0x00000001
			mov dword ptr ss : [eax + 0x20], 0x00000001
			push 0x54
			push eax
			push 0x3EF
			mov ecx, dword ptr ds : [Base_PlayerObj]
			mov edx, dword ptr ds : [ecx]
			mov edx, dword ptr ds : [edx + 0x4]
			call edx
			add esp, 0x54
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TPlayerObj::RunToXY(float flx, float fly) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

TSkillList* TSkillList::GetData()
{
	memset(tlist, 0, sizeof(tlist));
	DWORD dwFirstSkillObjAddr = NULL; //��һ������ĵ�ַ
	DWORD dwObj = NULL;
	__try
	{
		dwFirstSkillObjAddr = *(DWORD*)(Base_SkillList)+0x43C;
		for (int i = 0; i < 32; i++)
		{
			tlist[i].dwIndexForSkill = i;
			dwObj = *(DWORD*)(dwFirstSkillObjAddr + 4 * i);
			tlist[i].dwSkillObj = dwObj;
			if (dwObj == NULL)   continue;
			tlist[i].dwType = *(DWORD*)(dwObj + 0x8);
			tlist[i].dwIndexForAll = *(DWORD*)(dwObj + 0xC);
			tlist[i].szName = (char*)(dwObj + 0x64);
			tlist[i].byLevelNeed = *(DWORD*)(dwObj + 0xB4);
			tlist[i].IsLearned = *(WORD*)(dwObj + 0x1FE);
			tlist[i].dwExpNeed = *(DWORD*)(dwObj + 0x270);
		}
	}
	__except (1)
	{
		DbgOutput("TSkillList * TSkillList::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TSkillList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 32; i++)
	{
		if (tlist[i].dwSkillObj == NULL)	continue;
		DbgOutput("%s %d %2X %4X �Ƿ���ѧ��%d������Ҫ�ȼ���%d������Ҫ������%d��\n",
			tlist[i].szName,
			tlist[i].dwIndexForSkill,
			tlist[i].dwType,
			tlist[i].dwIndexForAll,
			tlist[i].IsLearned,
			tlist[i].byLevelNeed,
			tlist[i].dwExpNeed);
	}
}

/* [2020/03/01 13:17]-[Remark: None] */
/* [���ݼ��������Ѽ����ϵ�������ϵ�ָ����λ��]-[�ɹ������棬���򷵻ؼ�] */
BOOL TSkillList::DropSkillToF1F10(char *szSkillName, DWORD dwIndexF1F10)
{
	GetData();
	DWORD dwSkiObj = NULL;
	__try
	{
		for (int i = 0; i <32; i++)	//�ȱ��������б��ҵ���������Ӧ���±�
		{
			if (tlist[i].dwSkillObj == NULL)	continue;
			DbgOutput("��%d����������%s", i, tlist[i].szName);
			if (strcmp(tlist[i].szName, szSkillName)  == 0)
			{
				dwSkiObj = tlist[i].dwSkillObj;
				__asm
				{
					//д����� �� �������Ķ���Ļ�ַ
					mov eax,Base_MouseSelObj
					mov eax,[eax]
					mov ecx, dwSkiObj
					mov [eax+0x230], ecx
				}
				break;
			}
		}
		if (dwSkiObj == NULL)	//˵��û�ҵ���Ӧ�ļ���
		{
			return FALSE;
		}
		__asm
		{
			//�Ѷ����������
			mov edi, Base_ShortcutBar
			mov edi, [edi]
			mov ecx, dword ptr ds : [edi + 0x1634]
			mov edx, dword ptr ds : [edi + 0x1C60]
			mov eax, dwIndexF1F10
			push eax
			push ecx
			push edx
			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TSkillList::DropSkillToF1F10(char *szSkillName, DWORD dwIndexF1F10) �쳣\n");
	}
	return TRUE;
}

/* [2020/03/01 13:17]-[Remark: None] */
/* [���ݼ��������Ѽ����ϵ�������ϵĵ�һ����λ��]-[�ɹ��򷵻ط����ڿ�����б���±꣬ʧ�ܷ���-1] */
int TSkillList::DropSkillToF1F10(char* szSkillName)
{
	//�ȼ�⼼�����Ƿ�Ϊ��
	if (g_cAutoPlay.szSkillName == NULL || g_cAutoPlay.IsUseSkill == FALSE)
	{
		DbgOutput("δ����ʹ�ü���\n");
		return -1;
	}
	//�ȼ�⼼���Ƿ��Ѿ��ڿ������
	int niIndex = g_tScList.GetIndexByName(szSkillName);
	if (niIndex >= 0)	return niIndex;
	//�ҿ�λ���ü���
	niIndex = g_tScList.GetScNullIndex();
	if (niIndex == -1)
	{
		DbgOutput("������Ѿ����ˣ�\n");
		return -1;
	}
	BOOL bRet = DropSkillToF1F10(szSkillName, niIndex);
	if (bRet == TRUE)
	{
		return niIndex;
	}
	return -1;
}

/* [2020/03/01 13:18]-[Remark: None] */
/* [�������ܣ�ͨ�������б��±�]-[�ɹ������棬���򷵻ؼ�] */
BOOL TSkillList::LearnSkillByIndex(DWORD dwIndex)
{
	if (IsLearnable(dwIndex) == FALSE)	return FALSE;
	GetData();
	__try
	{
		__asm
		{
			mov edi, dword ptr ds : [Base_SkillList]
			mov edx, dwIndex
			mov edx, dword ptr ds : [edi + 0x43C + 4 * edx]
			mov ecx, dword ptr ds : [Call_LearnSkill_Ecx]
			mov eax, dword ptr ds : [edx + 0x54]
			push ecx
			push eax
			lea ecx, dword ptr ss : [ebp - 0x280C]
			push ecx
			mov ecx, dword ptr ds : [Base_PlayerObj]
			mov eax, Call_LearnSkill
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TSkillList::LearnSkill(DWORD dwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:18]-[Remark: None] */
/* [�������ܣ�ͨ����������]-[�ɹ������棬���򷵻ؼ�] */
BOOL TSkillList::LearnSkillByName(char * szSkillName)
{
	GetData();
	for (int i=0;i<32;i++)	
	{
		if (tlist[i].dwSkillObj == NULL)	continue;
		DbgOutput("����[%d] = %s\n", i, tlist[i].szName);
		if (strcmp(szSkillName,tlist[i].szName) == 0)
		{
			BOOL bRet = LearnSkillByIndex(i);
			return bRet;
		}
	}
	return FALSE;
}

/* [2020/03/01 13:18]-[Remark: ��Ҫ����1.�Ƿ���ѧ 2.�Ƿ�ﵽ�ȼ� 3.�Ƿ�ﵽ����] */
/* [���ݼ����б��±��жϼ����Ƿ��ѧ]-[�����������棬���������򷵻ؼ�] */
BOOL TSkillList::IsLearnable(DWORD dwIndex)
{
	//��ʼ������
	g_tRoleProperty.GetData();
	g_tSkiList.GetData();
	//���ܶ����Ƿ����
	if (g_tSkiList.tlist[dwIndex].dwSkillObj == NULL) 
	{
		DbgOutput("����[%d] ���ܶ��󲻴���\n", dwIndex);
		return FALSE;
	}	
	//�����Ƿ���ѧ
	BOOL bIsLearned = g_tSkiList.tlist[dwIndex].IsLearned;		
	if (bIsLearned == TRUE)
	{
		DbgOutput("����[%d]��%s �Ѿ�ѧϰ\n", dwIndex, tlist[dwIndex].szName);
		return FALSE;
	}
	//�Ƿ�ﵽ�ȼ�
	BOOL bNeedLevel = g_tSkiList.tlist[dwIndex].byLevelNeed;	//��Ҫ�ȼ�
	BOOL bRoleLevel = g_tRoleProperty.byLevel;					//��ɫ�ȼ�
	if (bRoleLevel < bNeedLevel)
	{
		DbgOutput("����[%d]��%s ��δ�ﵽҪ��ȼ�\n", dwIndex, tlist[dwIndex].szName);
		return FALSE;
	}
	//�Ƿ�ﵽ����
	BOOL bNeedExp = g_tSkiList.tlist[dwIndex].dwExpNeed;		//��Ҫ����
	BOOL bRoleExp = g_tRoleProperty.dw����;						//��ɫ����
	if (bRoleExp < bNeedExp)
	{
		DbgOutput("����[%d]��%s ��δ�ﵽҪ������\n", dwIndex, tlist[dwIndex].szName);
		return FALSE;
	}
	DbgOutput("����[%d]��%s ��������\n", dwIndex, tlist[dwIndex].szName);
	return TRUE;
}

#define offset_ScObjType 0x08
#define offset_ScObjName 0x64
TShortcutList* TShortcutList::GetData()
{
	memset(tlist, 0, sizeof(tlist));
	DWORD dwFirstSCObjAddr = NULL;	//��һ������ĵ�ַ
	DWORD dwObj = NULL;
	__try
	{
		dwFirstSCObjAddr = *(DWORD*)(Base_ShortcutBar)+0x43C;
		for (int i=0;i<10;i++)
		{
			dwObj = *(DWORD*)(dwFirstSCObjAddr + 4 * i);	//ûװ������Ϊ0
			tlist[i].dwScObj = dwObj;
			if (dwObj == NULL)	continue;
			tlist[i].dwType = *(WORD*)(dwObj + offset_ScObjType);
			tlist[i].szName = (char*)(dwObj + offset_ScObjName);
		}
	}
	__except (1)
	{
		DbgOutput("TShortcutList * TShortcutList::GetData() �쳣\n");
		return NULL;
	}
	return this;
}


void TShortcutList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 10; i++)
	{
		DbgOutput("�����%d:��������(%s),�������ͣ�%X��\n",i,
			tlist[i].szName,
			tlist[i].dwType);
	}
}

/* [2020/03/01 13:19]-[Remark: None] */
/* [���ݼ����������ڿ�������±�]-[�ҵ��ü����򷵻��±꣬û�ҵ��򷵻�-1] */
int TShortcutList::GetIndexByName(char * szSkillName)
{
	GetData();
	for (int i = 0; i < 10; i++)
	{
		if (tlist[i].dwScObj == NULL)	continue;
		if (strcmp(tlist[i].szName, szSkillName) == 0)	return i;
	}
	return -1;
}

/* [2020/03/01 13:19]-[Remark: None] */
/* [���ݿ�������±�ʹ�ü���]-[�ɹ������棬���򷵻ؼ�] */
BOOL TShortcutList::UseSkillByIndex(DWORD dwIndex)
{
	//DbgOutput("����UseSkillByIndex(%d) \n", dwIndex);
	DWORD dwEcx = Call_F1F10Use_Ecx;
	__try
	{
		__asm
		{
			mov eax, dwIndex
			push eax
			mov ecx, dwEcx
			mov eax, Call_F1F10Use
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TShortcutList::UseSkillByIndex(DWORD dwIndex) �쳣\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:20]-[Remark: None] */
/* [���ݼ�������ʹ�ü��ܣ������û�����ϵ����������ʹ��]-[�ɹ������棬���򷵻ؼ�] */
BOOL TShortcutList::UseSkillByName(char *szSkillName)
{
	//��ȡ�����ڿ�������±�
	int niIndex = g_tScList.GetIndexByName(szSkillName);
	if (niIndex == -1)	return FALSE;
	BOOL bRet = UseSkillByIndex(niIndex);
	return bRet;
}

/* [2020/03/01 13:20]-[Remark: None] */
/* [��ÿ������һ����λ���±�]-[�ɹ������±꣬û�ҵ���λ����-1] */
BOOL TShortcutList::GetScNullIndex()
{
	GetData();
	for (int i=0;i<10;i++)
	{
		if (tlist[i].dwScObj == NULL)	return i;
	}
	return -1;
}

TGroundGoodsList * TGroundGoodsList::GetData()
{
	DWORD dwObj = NULL;
	memset(tlist, 0, sizeof(tlist));
	__try
	{
		int i = 0;
		for (int j=0;j< Size_NearList;j++)
		{
			dwObj = *(DWORD*)(Base_NearObjList + 4 * j);
			if(dwObj==NULL)	continue;
			if(*(DWORD*)(dwObj+offset_Obj_Type) != Type_GroundGoods)	continue;
			tlist[i].dwType = *(DWORD*)(dwObj + offset_Obj_Type);
			tlist[i].szName = (char*)(dwObj + offset_TGroundGoodsObj_szName);
			tlist[i].dwIndexInAllObj = *(DWORD*)(dwObj + offset_Obj_Index);
			i++;
		}
	}
	__except(1)
	{
		DbgOutput("TGroundGoodsList * TGroundGoodsList::GetData() �쳣\n");
		return NULL;
	}
	return this;
}

void TGroundGoodsList::PrintMsg()
{
	GetData();
	for (int i = 0; i < Size_NearList; i++)
	{
		if (tlist[i].szName == NULL)	break;
		DbgOutput("%s,%X\n", tlist[i].szName, tlist[i].dwIndexInAllObj);
	}
}

/* [2020/03/01 13:20]-[Remark: None] */
/* [ѡ�е�����Ʒ]-[�ɹ������棬���򷵻ؼ�] */
BOOL TGroundGoodsList::SelGroundGoods()
{
	BOOL IsPickup = 1;	//����ΪĬ��ʰȡ����Ʒ
	GetData();
	for (int i = 0; i < Size_NearList; i++)	//����������Ʒ�б�
	{
		if (tlist[i].szName == NULL)	break;	//��������������Ʒ�б�
		for (vector<TGoodsManage>::iterator it = g_cAutoPlay.vct_GoodsMngList.begin();
			it < g_cAutoPlay.vct_GoodsMngList.end(); it++)	//������Ʒ�����б�,���˲�ʰȡ����Ʒ
		{
			if (strcmp(tlist[i].szName, it->szGoodsName) == 0) {
				if ((it->dwGoodsManageFlag) & Goods_Manage_NoPick) {
					IsPickup = 0;	//��ʰȡ
				}
				else {
					IsPickup = 1;	//ʰȡ
				}
				break;	//����������Ʒ�����б�
			}
		}
		if (IsPickup)
		{
			DWORD dwPlayerObj = *(DWORD*)(Base_PlayerObj);
			*(DWORD*)(dwPlayerObj + offset_PlayerObj_dwSelIndex) = tlist[i].dwIndexInAllObj;	
			break;	//ѡ�к���������������Ʒ�б�
		}
	}
	return TRUE;
}

/* [2020/03/01 13:20]-[Remark: ʰȡ�����޷�ʵ��ѡ���Լ���] */
/* [�Զ�����]-[�ɹ������棬���򷵻ؼ�] */
BOOL TGroundGoodsList::PickupGoods()
{
	BOOL bRet = SelGroundGoods();
	if (bRet)
	{
		bRet = g_tActList.UseActionByName("����");	//����������ʵ��ѡ���Լ���
		g_tBackList.UseGoodsByName("��ľ����");
	}
	return bRet;
}
