#pragma once
// ��Ϸ���ݽṹ ��ƫ�ƹ���
#include <Windows.h>

// ���ͱ��� �� �ǽṹ��Ա��������
typedef unsigned __int64 QWORD;
void DbgOutput(const char *szFormat, ...);
HWND GetGameWndHandle();	// ��ȡ��Ϸ���ھ��
DWORD TwoPoint_Distance(float x1, float y1, float x2, float y2);	// �����������ľ���
BOOL WSASendData(LPCVOID lpData, DWORD dwBufSize);	// �������ݵ�������

#define AllTask 0x2332
#define SelfTask 0x2333
void PrintAllTask(DWORD dwTaskListID);

// ���ж���ͨ�õĺ�
#define offset_Obj_Type 0x8		// ��������
#define offset_Obj_Index 0xC	// ���������ж����б���±�

#pragma pack(1)	// ��һ�ֽڶ��룬�������ո�
// ������ȡ�ֿ���Ʒʱ���޸ĵ�����,��A0���ֽ�
struct TStorePickDepotData
{
	BYTE byData1[0xA];
	QWORD nqCmd;		// ���E6 CE BD 93 00 00 00 03
						// ȡ�00 00 00 00 00 00 00 05
	DWORD dwType1;		// addr+0x12��[������Ʒ����+0x54]
	BYTE byData2[0x4];	   
	WORD nwSaveNum;		// addr+0x1A�����ж���				Ҫ���������
	BYTE byData3[0xE];	   
	QWORD nqID2;		// addr+0x2A��[������Ʒ����+0x5C]
	DWORD dwType3;		// addr+0x32��[������Ʒ����+0x54]
	BYTE byData4[0x4];	   
	WORD nwGoodsNum;	// addr+0x3A��[������Ʒ����+0xC4C]	��ǰ�������Ǳ���
	BYTE byData5[0x7];	   
	BYTE byIndex;		// addr+0x43��[������Ʒ����+0x1FC]	�ڱ����е��±�
	BYTE byData6[0x5D];	// A0-43=5D
};

//�̵�����Ʒʱ���޸ĵ�����,��A0���ֽ�
#define BuyGoods 0x01
#define SellGoods 0x02
struct TBuySellData
{
	BYTE byData1[0x2];
	DWORD dwCmd;			// addr+0x2��00880092������CALL��008C0094��������ȡ�ֿ�CALL
	DWORD dwBuySellType;	// addr+0x6��1����2����
	BYTE byData2[0x4];		// addr+0xA��0xEB1B5614
	DWORD dwType1;			// addr+0xE��[��Ʒ����+0x54]
	BYTE byData3[0x4];		   
	WORD nwGoodsNum1;		// addr+0x16��Ҫ�������۵�����
	BYTE byData4[0xE];		   
	QWORD nqID2;			// addr+0x26��[������Ʒ����+0x5C]
	DWORD dwType2;			// addr+0x2E��[��Ʒ����+0x54]
	BYTE byData5[0x4];		   
	WORD nwGoodsNum2;		// addr+0x36��Ҫ���۵�����
	BYTE byData6[0x7];		   
	BYTE byIndex;			// addr+0x3F��[������Ʒ����+0x1FC]	�ڱ����е��±�
	BYTE byData7[0x61];		// A0-3F=61
};
#pragma pack()	//�ָ�Ĭ�϶���

//�������Զ���[0x02C186D8]
struct TRoleProperty 
{
	char* szRoleName;					// ��ɫ����[�������Զ���+0x00]
	BYTE byLevel;						// �ȼ���[�������Զ���+0x34]
	BYTE byTransferNum;					// תְ������[�������Զ���+0x35]
	char* szRole����;					// ������[�������Զ���+0x36]
	DWORD dwHp;							// ��ǰHP��[�������Զ���+0x80]
	DWORD dwMp;							// ��ǰMP��[�������Զ���+0x84]
	DWORD dwAngry;						// ��ǰ��ŭ��[�������Զ���+0x88]
	DWORD dwHpMax;						// ���HP��[�������Զ���+0x8C]
	DWORD dwMpMax;						// ���MP��[�������Զ���+0x90]
	DWORD dwAngryMax;					// ����ŭ��[�������Զ���+0x94]
	QWORD nqExp;						// ��ǰ���飺[�������Զ���+0x98]
	QWORD nqExpMax;						// �������飺[�������Զ���+0xA0]
	DWORD dw����;						// ����ֵ��[�������Զ���+0xAC]
	DWORD dw����;						// �ģ�[�������Զ���+0xB0]
	DWORD dw����;						// ����[�������Զ���+0xB4]
	DWORD dw����;						// �壺[�������Զ���+0xB8]
	DWORD dw����;						// �꣺[�������Զ���+0xBC]
	DWORD dw����;						// ������[�������Զ���+0xC8]
	DWORD dw����;						// ������[�������Զ���+0xCC]
	DWORD dw����;						// ���У�[�������Զ���+0xD0]
	DWORD dw�ر�;						// �رܣ�[�������Զ���+0xD4]
	QWORD nqMoney;						// ��Ǯ��[�������Զ���+0xE4]
	WORD nw����;							// ��ǰ���أ�[�������Զ���+0xEC]
	WORD nw����Max;						// ����أ�[�������Զ���+0xEE]
	BYTE byPoint����;					// δ���������㣺[�������Զ���+0xF0]
	BYTE byQiGongPoints[32];			// �����������㣺[�������Զ���+0xF4+4*i]
	DWORD dwHpPercent;					// Ѫ���ٷֱ�
	DWORD dwMpPercent;					// �����ٷֱ�
	TRoleProperty* GetData();	
	void PrintMsg();
};

//������Ʒ����[ [02E3D3E4]+43C+4*�±� ]
struct TGoodsObj 
{
	DWORD dwGoodsObj;	// ��Ʒ������
	char* szName;		// ��Ʒ���֣�[��Ʒ����+0x64]
	char* szMsg;		// ��Ʒ����������[��Ʒ����+0xF9]
	char* szAttribute;	// ��Ʒ���ԣ�[��Ʒ����+0x238]
						   
	DWORD dwType1;		// Type1��[��Ʒ����+0x54]
	QWORD nqID2;		// ID2��[��Ʒ����+0x5C]
	WORD nwNum;			// ��Ʒ������[��Ʒ����+0xC4C]
	BYTE byIndex;		// ���б���±꣺[��Ʒ����+0x1FC]
};

//�����б�
struct TBackpackList
{	
	TGoodsObj tlist[36];	//�����б������36����Ʒ����
	TBackpackList* GetData();
	void PrintMsg();
	BOOL UseGoodsByIndex(DWORD dwIndex);		// ʹ�ñ�����Ʒ,���� �±�
	int GetGoodsIndexByName(char* szName);		// ��ѯ������Ʒ�����򷵻��±꣬���򷵻�-1
	BOOL UseGoodsByName(char* szName);			// ʹ�ñ�����Ʒ,���� ����
	DWORD SelectGoods(DWORD dwIndex);			// ѡ�б���ָ�������Ʒ
	DWORD GetGoodsNumByName(char* szName);		// ��ѯ����ָ����Ʒ����
	DWORD GetAllHpNum();						// ��ѯ����HpҩƷ����
	DWORD GetAllMpNum();						// ��ѯ����MpҩƷ����
	BOOL IsBackpackFull();						// �жϱ����Ƿ���

	BOOL MoveGoodsToDepot();					// �ƶ�������ѡ����Ʒ���ֿ�
	BOOL MoveGoodsToDepot(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ���ֿ�
	BOOL MoveGoodsToDepot(char* szName, WORD nwIndex);	// �ƶ�����ָ�����ֵ���Ʒ���ֿ�,��ָ������

	enum EquipType
	{
		dwClothes,			// �·�
		dwHandL, dwHandR,	// ���һ���
		dwWeapon,			// ����
		dwShoes,			// Ь��
		dwArmor,			// ����
		dwNeck,				// ����
		dwEarL, dwEarR,		// ���Ҷ���
		dwRingL, dwRingR	// ���ҽ�ָ
	};
	BOOL MoveGoodsToEquip(DWORD dwIndex);			// �ƶ�������ѡ����Ʒ��װ���б�ָ����
	BOOL MoveGoodsToEquip_Clothes(char* szName);	// �ƶ�����ָ�����ֵ���Ʒ��װ���·���
	BOOL MoveGoodsToEquip_HandL(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ������
	BOOL MoveGoodsToEquip_HandR(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ���һ���
	BOOL MoveGoodsToEquip_Weapon(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ������
	BOOL MoveGoodsToEquip_Shoes(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ��Ь��
	BOOL MoveGoodsToEquip_Armor(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ������
	BOOL MoveGoodsToEquip_Neck(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ������
	BOOL MoveGoodsToEquip_EarL(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ�������
	BOOL MoveGoodsToEquip_EarR(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ���Ҷ���
	BOOL MoveGoodsToEquip_RingL(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ�����ָ
	BOOL MoveGoodsToEquip_RingR(char* szName);		// �ƶ�����ָ�����ֵ���Ʒ��װ���ҽ�ָ
};

//�ֿ��б�
struct TDepotList
{
	TGoodsObj tlist[60];	//�ֿ��б������60����Ʒ����
	TDepotList* GetData();
	void PrintMsg();
	int GetGoodsIndexByName(char* szName);		// ��ѯ�ֿ���Ʒ�����򷵻��±꣬���򷵻�-1
	DWORD GetGoodsNumByName(char* szName);		// ��ѯ�ֿ���Ʒ����
	DWORD SelectGoods(DWORD dwIndex);			// ѡ�вֿ�ָ�������Ʒ
												   
	BOOL MoveGoodsToBackPack();					// �ƶ��ֿ���ѡ����Ʒ������
	BOOL MoveGoodsToBackpack(char* szName);		// �ƶ��ֿ�ָ�����ֵ���Ʒ������
	BOOL MoveGoodsToBackpack(char* szName, WORD nwIndex);	// �ƶ��ֿ�ָ�����ֵ���Ʒ������,��ָ������
};

//�̵��б�
struct TShopList 
{
	DWORD GetShopBase();	// ��ȡ�̵��б��ַ
	TGoodsObj tlist[60];	// �̵��б������60����Ʒ����
	TShopList* GetData();
	void PrintMsg();
	int GetGoodsIndexByName(char* szName);		// ��ѯ�̵���Ʒ�����򷵻��±꣬���򷵻�-1
	BOOL BuyGoodsByName(char* szName, WORD nwIndex);	// ͨ����Ʒ���֣�����ָ����������Ʒ
	BOOL SellGoodsByName(char* szName, WORD nwIndex);	// ͨ����Ʒ���֣�����ָ����������Ʒ
};

//��Χ����[04280BA0+4*i]
struct TNearObj 
{
	DWORD dwNearObj;			// ��Χ����
	DWORD dwType;				// �������ͣ�[��Χ����+0x8]��0x2E��ͨ���0x31���
	DWORD dwIndexInAllObj;		// ����ID��[��Χ����+0xC]
	BOOL IsSelected;			// ѡ��״̬��[��Χ����+0x354] 
	float flDistance;			// ����ҵľ��룺[��Χ����+0x35C]
	char* szName;				// ��������ƣ�[��Χ����+0x360]   ������֣�[��Χ����+0x18]
	BYTE IsDead;				// �Ƿ�������[��Χ����+0x3C0]
	BYTE IsAbleKill;			// �Ƿ���ñ�ɱ��: [��Χ����+0x3C8]
	DWORD dwHP;					// Ѫ����[��Χ����+0x5F4]    
	DWORD dwLevel;				// �ȼ���[��Χ����+0x5F8]   
	float flX;					// ����X��[��Χ����+0x1060]
	float flY;					// ����Y��[��Χ����+0x1068]
};

#define Size_NearList 50
//��Χ�����б�
typedef struct TNearList
{
	TNearObj tlist[Size_NearList];	// ��Χ�����б����Ű���Size_NearList����Χ����
	TNearList* GetData();
	void PrintMsg();
	DWORD GetNpcID(char* szName);
	BOOL ChatWithNPC(char* szName);	// ����NPC���֣���ָ��NPC�Ի�
	BOOL ClickChatMenuOption(DWORD dwIndex);	// ����Ի���˵�ѡ��(��������)
	BOOL EndChatWithNPC(char* szNpcName);		// ������NPC�Ի�
	BOOL OpenShop(char* szNpcName);				// �򿪲ֿ�,���/����
}TNearList;

//��������[ [02E3DD58]+43C+4*i ]
typedef struct TActionObj 
{
	char* szName;		// �������ƣ�[��������+0x64]
	DWORD dwActionID;	// ����ID��[��������+0x54]
}TActionObj;

//���������б�
typedef struct TActionList 
{
	TActionObj tlist[12];	// �����б� ��12��ʵ�õĶ�������
	TActionList* GetData();
	void PrintMsg();
	BOOL UseActionByIndex(DWORD dwIndex);	// ʹ�ö��� ͨ���±�
	BOOL UseActionByName(char* szActionName);	// ʹ�ö��� ͨ��������
}TActionList;

//��Ҷ���[02E65A24]
#define offset_PlayerObj_dwSelIndex 0x1A64
#define offset_PlayerObj_flCurX 0x1C9C
#define offset_PlayerObj_flDesX 0x1C60
typedef struct TPlayerObj
{
	DWORD dwSelIndex;	// ��ҵ�ǰѡ�еĶ����ID,Ҳ�����ж���������±�,δѡ��Ϊ0xFFFF
						// [��Ҷ���+0x1A64]
	float flDesX;		// [��Ҷ���+0x1C60]
	float flDesY;		// [��Ҷ���+0x1C68]
	float flCurX;		// [��Ҷ���+0x1C9C]
	float flCurY;		// [��Ҷ���+0x1CA4]

	TPlayerObj* GetData();
	void PrintMsg();
	BOOL SelObj(DWORD dwIndexInAllObj);	// ѡ�����
	BOOL CancelSel();		// ȡ��ѡ�ж���
	BOOL SelNearMonster();	// �Զ�ѡ����
	BOOL SelNearMonster(const char* szMonName);	// ���ݹ�����ѡ������Ĺ���
	BOOL AutoBeatMonster();	// �Զ��չ����
	BOOL AutoBeatMonsterBySkill(char* szSkillName);	// �Զ����ܴ��
	DWORD GetSelObjType();	// ���ѡ�ж��������
	BOOL IsSelObjDead();	// ѡ��Ĺ����Ƿ�����
	BOOL IsSelObjAbleKill();// ѡ��Ĺ����Ƿ�ɱ���ɱ
	BOOL RunToXY(float flx, float fly);	// �ܵ�ָ������
}TPlayerObj;

// ���ܶ���[ [02E3EF64]+0x43C+4*i ]
typedef struct TSkillObj
{
	DWORD dwSkillObj;			// ���ܶ���
	DWORD dwIndexForSkill;		// λ�ڼ��ܵĵڼ���
	DWORD dwType;				// ���ͣ�[���ܶ���+0x8]
	DWORD dwIndexForAll;		// ID��[���ܶ���+0xC]
	char* szName;				// ���ƣ�[���ܶ���+0x64]
	DWORD byLevelNeed;			// �ȼ���Ҫ��[���ܶ���+0xB4]
	BOOL IsLearned;             // �����Ƿ���ѧ��[���ܶ���+0x1FE]
	DWORD dwExpNeed;			// ������Ҫ��[���ܶ���+0x270]
}TSkillObj;

// �����б�
typedef struct TSkillList
{
	TSkillObj tlist[32];		// ���������б�32�����ܣ���8���Ǽ����飬��������
	TSkillList* GetData();
	void PrintMsg();
	BOOL DropSkillToF1F10(char *szSkillName,DWORD dwIndexF1F10);
	int DropSkillToF1F10(char *szSkillName);
	BOOL LearnSkillByIndex(DWORD dwIndex);		// �������ܣ�ͨ�������б��±�
	BOOL LearnSkillByName(char* szSkillName);	// �������ܣ�ͨ����������
	BOOL IsLearnable(DWORD dwIndex);			// �����Ƿ������
}TSkillList;

// ���������[ [02E3EF08]+0*4+0x43C ]
typedef struct TShortcutObj
{
	DWORD dwScObj;	// ���������
	DWORD dwType;	// ���ͣ�[���������+0x08]
	char* szName;	// ���ƣ�[���������+0x64]
}TShortcutObj;

// ������б�
typedef struct TShortcutList
{
	TShortcutObj tlist[10];	// ������б���10�����������
	TShortcutList* GetData();
	void PrintMsg();
	BOOL UseSkillByName(char* szSkillName);
	BOOL UseSkillByIndex(DWORD dwIndex);
	int GetIndexByName(char* szSkillName);
	BOOL GetScNullIndex();
}TShortcutList;

// ������Ʒ����[04280BA0+4*i]
#define offset_TGroundGoodsObj_szName 0x94
typedef struct TGroundGoodsObj
{
	DWORD dwType;	// �������ͣ�[��Χ����+0x8]��0x33Ϊ������Ʒ
	DWORD dwIndexInAllObj;		// ����ID��[��Χ����+0xC]
	char* szName;	// ���ƣ�[��Χ����+0x94]
}TGroundGoodsObj;

// ������Ʒ�б�
typedef struct TGroundGoodsList 
{
	TGroundGoodsObj tlist[Size_NearList];
	TGroundGoodsList* GetData();
	void PrintMsg();
	BOOL SelGroundGoods();	// ѡ�е�����Ʒ
	BOOL PickupGoods();		// �Զ�����
}TGroundGoodsList;
