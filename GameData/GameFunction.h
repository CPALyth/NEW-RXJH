#pragma once

#include "HookGameMainThread.h"
#include "global_Var.h"
#include "BaseGame.h"
#include "CAutoPlay.h"
#include <random>    //�����


BOOL HookRefreshScreen(BOOL IsDisableRefresh=FALSE);
BOOL FindWay(int x, int y);	//Ѱ·��ָ������ŷ���
int* GetRoleXY(int* Pos);
void UseGoodsByName(const char* szGoodName);
void UseSkillByName(const char* szSkillName);
void SelNearMonster(const char* szMonName);
