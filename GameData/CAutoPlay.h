#pragma once
#include <Windows.h>
#include <vector>
#include <random>    //�����

using namespace std;

//��Ʒ������ʽ�����ݽṹ
typedef struct TSupplyData 
{
	char szAddr[30];		//ȥ����
	char szGoodsName[30];	//��ʲô
	DWORD dwNum;			//�����
}TSupplyData;

//ս��Ʒ����ʽ�����ݽṹ
#define Goods_Manage_No 0		//������ 000
#define Goods_Manage_Depot 1	//��ֿ� 001
#define Goods_Manage_Shop 2		//���̵� 010
#define Goods_Manage_NoPick 4	//��ʰȡ	100
typedef struct TGoodsManage 
{
	char szGoodsName[30];	//��Ʒ����
	DWORD dwGoodsManageFlag;//��Ʒ����ʽ
}TGoodsManage;

class CAutoPlay
{
public:
	CAutoPlay();
	~CAutoPlay();

	HANDLE ht_AutoGuaJi;	//�һ��߳̾��

	//������
	BOOL IsAutoBeatMonster;		// �Ƿ��Զ����
	BOOL IsRangeLimit;			// �Ƿ����ƴ�ַ�Χ
	DWORD RangeOfHitMonster;	// ��ַ�Χ�뾶
	int GuaJiPosX;				// �һ�������X
	int GuaJiPosY;				// �һ�������Y
	BOOL CheckGuaJiRange();		// ���һ���Χ
	BOOL IsUseSkill;			// �Ƿ�ʹ�ü���
	char szSkillName[20];		// ����õļ���
	BOOL IsPickupGoods;			// �Ƿ��Զ�����
	BOOL IsHaveThingToPick();	// �������Ƿ�����Ʒ�ɼ�

	//�������
	BOOL IsAutoUseHpGoods;	//�Ƿ��ڵ�HPʱ�Զ�ʹ��ҩƷ
	BOOL IsAutoUseMpGoods;	//�Ƿ��ڵ�MPʱ�Զ�ʹ��ҩƷ
	char szHpGoodsName[20];	//��HPʱ�Զ�ʹ�õ���Ʒ��
	char szMpGoodsName[20];	//��MPʱ�Զ�ʹ�õ���Ʒ��
	DWORD dwHpPercent;		//��HP�жϰٷֱ�
	DWORD dwMpPercent;		//��MP�жϰٷֱ�
	DWORD dwCheckHpMpSpeed;	//Ѫ������ٶ�

	//�Ṧ���
	BOOL IsAutoUseQingGong;		//�Ƿ��Զ�ʹ���Ṧ
	char szQingGongName[20];	//�Ṧ������

	//�س��������
	BOOL IsAutoBuyHpGoods;		//�����趨HpҩƷ�����Ƿ�سǲ���
	DWORD dwMinNum_HpGoods;		//HpҩƷ�������ڴ�ֵʱ�سǲ���
	DWORD dwMaxNum_HpGoods;		//HpҩƷ�����سǲ���������ֵ
	char szHpGoodsNameToSupply[20];	//�سǲ�����HpҩƷ��

	BOOL IsAutoBuyMpGoods;		//�����趨MpҩƷ�����Ƿ�سǲ���
	DWORD dwMinNum_MpGoods;		//MpҩƷ�������ڴ�ֵʱ�سǲ���
	DWORD dwMaxNum_MpGoods;		//MpҩƷ�����سǲ���������ֵ
	char szMpGoodsNameToSupply[20];	//�سǲ�����MpҩƷ��

	BOOL IsNeedBackToCity();	//�ж��Ƿ���Ҫ�س�
	BOOL BackToCitySupply();	//�سǲ���
	vector<TSupplyData> vct_SupplyList;		//��� ������Ʒ�б� ������
	vector<TGoodsManage> vct_GoodsMngList;	//��� ������Ʒ�б� ������
	BOOL IsHpGoods(char* szName);			//������Ʒ���жϸ���Ʒ�Ƿ���HpҩƷ
	BOOL IsMpGoods(char* szName);			//������Ʒ���жϸ���Ʒ�Ƿ���MpҩƷ
	BOOL IsStone(char* szName);				//������Ʒ���жϸ���Ʒ�Ƿ���ǿ��ʯ,����ʯ
	
	BOOL GoToShopForSupply();	//���̵� ���� �� ���� ��Ʒ
	void SellSetGoodsToShop();	//���������趨��Ʒ���̵�
	void BuySetGoodsFromShop();	//���̵깺�������趨��Ʒ

	BOOL GoToDepotForSupply();	//���ֿ� ��� �� ȡ�� ��Ʒ
	void StoreSetGoodsToDepot();	//��������趨��Ʒ���ֿ�
	void PickSetGoodsFromDepot();	//�Ӳֿ�ȡ�������趨��Ʒ

	void ThreadProc_AutoGuaJi(LPVOID lpData);	//�Զ��һ��̺߳���
	void LowHpMpProtect();

	void StartAutoPlay();	//��ʼ�һ�
	void StopAutoPlay();	//ֹͣ�һ�
	BOOL LoopFlag;			//ѭ����־
};

