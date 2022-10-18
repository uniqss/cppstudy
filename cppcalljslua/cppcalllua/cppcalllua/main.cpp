#include "LuaStackAutoProtector.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int luaAdd(lua_State* L, int a, int b)
{
	LuaStackAutoProtector o(L);
	lua_getglobal(L, "add");

	lua_pushnumber(L, a);

	lua_pushnumber(L, b);

	lua_call(L, 2, 1);

	int sum = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return sum;
}

int luaTestWriteFile(lua_State* L)
{
	LuaStackAutoProtector o(L);
	lua_getglobal(L, "testWriteFile");

	lua_call(L, 0, 1);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int main(int argc, char* argv[])
{
	lua_State* L;

	L = luaL_newstate();

	luaL_openlibs(L);

	luaL_dofile(L, "myFile.lua");
	int ret = luaAdd(L, 3, 4);
	printf("luaAdd ret:%d\n", ret);

	ret = luaTestWriteFile(L);
	printf("luaTestWriteFile ret:%d\n", ret);

	lua_close(L);

	printf("Press enter to exit...");
	getchar();

	return 0;
}
