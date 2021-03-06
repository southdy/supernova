#include "Events.h"
#include "platform/Log.h"
#include "Engine.h"
#include "LuaBind.h"
#include "Input.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

using namespace Supernova;


void (*Events::onDrawFunc)();
int Events::onDrawLuaFunc;

void (*Events::onUpdateFunc)();
int Events::onUpdateLuaFunc;

void (*Events::onTouchPressFunc)(float, float);
int Events::onTouchPressLuaFunc;

void (*Events::onTouchUpFunc)(float, float);
int Events::onTouchUpLuaFunc;

void (*Events::onTouchDragFunc)(float, float);
int Events::onTouchDragLuaFunc;

void (*Events::onMousePressFunc)(int, float, float);
int Events::onMousePressLuaFunc;

void (*Events::onMouseUpFunc)(int, float, float);
int Events::onMouseUpLuaFunc;

void (*Events::onMouseDragFunc)(int, float, float);
int Events::onMouseDragLuaFunc;

void (*Events::onMouseMoveFunc)(float, float);
int Events::onMouseMoveLuaFunc;

void (*Events::onKeyPressFunc)(int);
int Events::onKeyPressLuaFunc;

void (*Events::onKeyUpFunc)(int);
int Events::onKeyUpLuaFunc;


Events::Events() {
}

Events::~Events() {
}

void Events::luaCallback(int nargs, int nresults, int msgh){
    int status = lua_pcall(LuaBind::getLuaState(), nargs, nresults, msgh);
    if (status != 0){
        Log::Error(LOG_TAG, "Lua Error: %s\n", lua_tostring(LuaBind::getLuaState(),-1));
    }
}

void Events::onDraw(void (*onDrawFunc)()){
    Events::onDrawFunc = onDrawFunc;
}

int Events::onDraw(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onDrawLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onDraw(){
    if (onDrawFunc != NULL){
        onDrawFunc();
    }
    if (onDrawLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onDrawLuaFunc);
        luaCallback(0, 0, 0);
    }
}

void Events::onUpdate(void (*onUpdateFunc)()){
    Events::onUpdateFunc = onUpdateFunc;
}

int Events::onUpdate(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onUpdateLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onUpdate(){
    if (onUpdateFunc != NULL){
        onUpdateFunc();
    }
    if (onUpdateLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onUpdateLuaFunc);
        luaCallback(0, 0, 0);
    }
}

void Events::onTouchPress(void (*onTouchPressFunc)(float, float)){
    Events::onTouchPressFunc = onTouchPressFunc;
}

int Events::onTouchPress(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onTouchPressLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onTouchPress(float x, float y){
    if (onTouchPressFunc != NULL){
        onTouchPressFunc(x, y);
    }
    if (onTouchPressLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onTouchPressLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), x);
        lua_pushnumber(LuaBind::getLuaState(), y);
        luaCallback(2, 0, 0);
    }
    Input::addTouchPressed();
    Input::setTouchPosition(x, y);
}

void Events::onTouchUp(void (*onTouchUpFunc)(float, float)){
    Events::onTouchUpFunc = onTouchUpFunc;
}

int Events::onTouchUp(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onTouchUpLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onTouchUp(float x, float y){
    if (onTouchUpFunc != NULL){
        onTouchUpFunc(x, y);
    }
    if (onTouchUpLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onTouchUpLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), x);
        lua_pushnumber(LuaBind::getLuaState(), y);
        luaCallback(2, 0, 0);
    }
    Input::releaseTouchPressed();
    Input::setTouchPosition(x, y);
}

void Events::onTouchDrag(void (*onTouchDragFunc)(float, float)){
    Events::onTouchDragFunc = onTouchDragFunc;
}

int Events::onTouchDrag(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onTouchDragLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onTouchDrag(float x, float y){
    if (onTouchDragFunc != NULL){
        onTouchDragFunc(x, y);
    }
    if (onTouchDragLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onTouchDragLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), x);
        lua_pushnumber(LuaBind::getLuaState(), y);
        luaCallback(2, 0, 0);
    }
    Input::setTouchPosition(x, y);
}

void Events::onMousePress(void (*onMousePressFunc)(int, float, float)){
    Events::onMousePressFunc = onMousePressFunc;
}

int Events::onMousePress(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onMousePressLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onMousePress(int button, float x, float y){
    if (onMousePressFunc != NULL){
        onMousePressFunc(button, x, y);
    }
    if (onMousePressLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onMousePressLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), button);
        lua_pushnumber(LuaBind::getLuaState(), x);
        lua_pushnumber(LuaBind::getLuaState(), y);
        luaCallback(3, 0, 0);
    }
    Input::addMousePressed(button);
    Input::setMousePosition(x, y);
}

void Events::onMouseUp(void (*onMouseUpFunc)(int, float, float)){
    Events::onMouseUpFunc = onMouseUpFunc;
}

int Events::onMouseUp(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onMouseUpLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onMouseUp(int button, float x, float y){
    if (onMouseUpFunc != NULL){
        onMouseUpFunc(button, x, y);
    }
    if (onMouseUpLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onMouseUpLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), button);
        lua_pushnumber(LuaBind::getLuaState(), x);
        lua_pushnumber(LuaBind::getLuaState(), y);
        luaCallback(3, 0, 0);
    }
    Input::releaseMousePressed(button);
    Input::setMousePosition(x, y);
}

void Events::onMouseDrag(void (*onMouseDragFunc)(int, float, float)){
    Events::onMouseDragFunc = onMouseDragFunc;
}

int Events::onMouseDrag(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onMouseDragLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onMouseDrag(int button, float x, float y){
    if (onMouseDragFunc != NULL){
        onMouseDragFunc(button, x, y);
    }
    if (onMouseDragLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onMouseDragLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), button);
        lua_pushnumber(LuaBind::getLuaState(), x);
        lua_pushnumber(LuaBind::getLuaState(), y);
        luaCallback(3, 0, 0);
    }
    Input::setMousePosition(x, y);
}

void Events::onMouseMove(void (*onMouseMoveFunc)(float, float)){
    Events::onMouseMoveFunc = onMouseMoveFunc;
}

int Events::onMouseMove(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onMouseMoveLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onMouseMove(float x, float y){
    if (onMouseMoveFunc != NULL){
        onMouseMoveFunc(x, y);
    }
    if (onMouseMoveLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onMouseMoveLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), x);
        lua_pushnumber(LuaBind::getLuaState(), y);
        luaCallback(2, 0, 0);
    }
    Input::setMousePosition(x, y);
}

void Events::onKeyPress(void (*onKeyPressFunc)(int)){
    Events::onKeyPressFunc = onKeyPressFunc;
}

int Events::onKeyPress(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onKeyPressLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onKeyPress(int key){
    if (onKeyPressFunc != NULL){
        onKeyPressFunc(key);
    }
    if (onKeyPressLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onKeyPressLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), key);
        luaCallback(1, 0, 0);
    }
    Input::addKeyPressed(key);
}

void Events::onKeyUp(void (*onKeyUpFunc)(int)){
    Events::onKeyUpFunc = onKeyUpFunc;
}

int Events::onKeyUp(lua_State *L){
    
    if (lua_type(L, 2) == LUA_TFUNCTION){
        Events::onKeyUpLuaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
    }else{
        //TODO: return error in Lua
    }
    return 0;
}

void Events::call_onKeyUp(int key){
    if (onKeyUpFunc != NULL){
        onKeyUpFunc(key);
    }
    if (onKeyUpLuaFunc != 0){
        lua_rawgeti(LuaBind::getLuaState(), LUA_REGISTRYINDEX, Events::onKeyUpLuaFunc);
        lua_pushnumber(LuaBind::getLuaState(), key);
        luaCallback(1, 0, 0);
    }
    Input::releaseKeyPressed(key);
}
