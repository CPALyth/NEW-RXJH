#pragma once
#include <Windows.h>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
extern lua_State* g_L;
void RegAllLuaFunc(lua_State* L);

//----------------------���º�����Ϊ lua���õ�C++������ʽ------------------------//
int cLua_DbgPrint(lua_State* L);
int cLua_FindWay(lua_State* L);
int cLua_GetRoleXY(lua_State* L);
int cLua_UseGoods(lua_State* L);
int cLua_UseSkill(lua_State* L);
int cLua_SelNearMonster(lua_State* L);
int cLua_Wait(lua_State* L);