#pragma once
#include "windows.h"

#define MSG_UseGoodsByName 1	// ʹ����Ʒ
#define MSG_DropSkillToF1F10 2	// ���ü��ܵ������
#define MSG_UseSkillByName 3	// ʹ�ü���
#define MSG_AutoBeatMonsterBySkill 4	// ʹ�ü����Զ����
#define MSG_RunToXY 5			// Ѱ·
#define MSG_ChatWithNPC 6		// ��NPC�Ի�
#define MSG_EndChatWithNPC 7	// �����Ի�
#define MSG_OpenShop 8			// ���̵�
#define MSG_BuyGoods 9			// ������Ʒ
#define MSG_SellGoods 10		// ������Ʒ
#define MSG_CancelSel 11		// ȡ��ѡ���˶���
#define MSG_MoveGoodsToDepot 12	// �����Ʒ���ֿ�
#define MSG_MoveGoodsToBcpk	 13	// �Ӳֿ�ȡ����Ʒ
#define MSG_PickupGoods 14		// ����
#define MSG_GetRoleXY 15		// ��ȡ��������
#define MSG_SelNearMonster 16	// �Զ�ѡ�����

#define MSG_TEST1 888
#define MSG_TEST2 888+1
#define MSG_TEST3 888+2

//���º��������� �����߳�ִ��
BOOL HookMainThread();
BOOL UnHookMainThread();
void msg_UseGoodsByName(const char* szName);
void msg_DropSkillToF1F10(const char* szSkillName);
void msg_UseSkillByName(const char* szSkillName);
void msg_AutoBeatMonsterBySkill(char* szSkillName);
void msg_RunToXY(int iX,int iY);
void msg_ChatWithNPC(char* szNpcName);
void msg_EndChatWithNPC(char* szNpcName);
void msg_OpenShop(char* szNpcName);
void msg_BuyGoods(char* szName, WORD nwIndex);
void msg_SellGoods(char* szName, WORD nwIndex);
void msg_CancelSel();
void msg_MoveGoodsToDepot(char* szName, WORD nwIndex);
void msg_MoveGoodsToBcpk(char* szName, WORD nwIndex);
void msg_PickupGoods();
void msg_GetRoleXY(int* Pos);
void msg_SelNearMonster(const char* szMonName);

void msg_Test1(LPVOID lpData);
void msg_Test2(LPVOID lpData);
void msg_Test3(LPVOID lpData);

//��Ϣ�����õĽṹ��
typedef struct Goods
{
	char szName[30];
	WORD nwIndex;
}Goods;
