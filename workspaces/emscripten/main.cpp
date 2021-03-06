#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <string.h>

#include "Engine.h"
#include "Supernova.h"
#include "Input.h"


void renderLoop();
EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData);
EM_BOOL fullscreenchange_callback(int eventType, const EmscriptenFullscreenChangeEvent *e, void *userData);
EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData);
EM_BOOL wheel_callback(int eventType, const EmscriptenWheelEvent *e, void *userData);

int originalWidth = 800, originalHeight = 600;

extern "C" {
    int getScreenWidth() {
        return Supernova::Engine::getScreenWidth();
    }
    int getScreenHeight() {
        return Supernova::Engine::getScreenHeight();
    }
    void updateScreenSize(int nWidth, int nHeight){
        Supernova::Engine::onSurfaceChanged(nWidth, nHeight);
    }
}

int main(int argc, char **argv) {

    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    if ((argv[1] != NULL && argv[1] != 0) && (argv[2] != NULL && argv[2] != 0)){
        Supernova::Engine::onStart(atoi(argv[1]), atoi(argv[2]));
    }else{
        Supernova::Engine::onStart();
    }

    int width = Supernova::Engine::getScreenWidth();
    int height = Supernova::Engine::getScreenHeight();

    SDL_Surface *screen = SDL_SetVideoMode( width, height, 16, SDL_OPENGL );
    if ( !screen ) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }

    EMSCRIPTEN_RESULT ret = emscripten_set_keypress_callback(0, 0, 1, key_callback);
    ret = emscripten_set_keydown_callback(0, 0, 1, key_callback);
    ret = emscripten_set_keyup_callback(0, 0, 1, key_callback);
    ret = emscripten_set_fullscreenchange_callback(0, 0, 1, fullscreenchange_callback);
    ret = emscripten_set_click_callback(0, 0, 1, mouse_callback);
    ret = emscripten_set_mousedown_callback(0, 0, 1, mouse_callback);
    ret = emscripten_set_mouseup_callback(0, 0, 1, mouse_callback);
    ret = emscripten_set_dblclick_callback(0, 0, 1, mouse_callback);
    ret = emscripten_set_mousemove_callback(0, 0, 1, mouse_callback);
    ret = emscripten_set_wheel_callback(0, 0, 1, wheel_callback);

    Supernova::Engine::onSurfaceCreated();
    Supernova::Engine::onSurfaceChanged(width, height);

    emscripten_set_main_loop(renderLoop, 0, 1);

    SDL_Quit();

    return 0;
}

void renderLoop(){
    Supernova::Engine::onDraw();

    SDL_GL_SwapBuffers();
}

EM_BOOL fullscreenchange_callback(int eventType, const EmscriptenFullscreenChangeEvent *e, void *userData) {
    if (e->isFullscreen){
        int w, h, fs;
        emscripten_get_canvas_size(&w, &h, &fs);
        originalWidth = w;
        originalHeight = h;

        emscripten_set_canvas_size(e->screenWidth, e->screenHeight);
        updateScreenSize(e->screenWidth, e->screenHeight);
    }else{
        emscripten_set_canvas_size(originalWidth, originalHeight);
        updateScreenSize(originalWidth, originalHeight);
    }

    return 0;
}

int supernova_mouse_button(int button){
    if (button == 0) return S_MOUSE_BUTTON_1;
    if (button == 2) return S_MOUSE_BUTTON_2;
    if (button == 1) return S_MOUSE_BUTTON_3;
    if (button == 3) return S_MOUSE_BUTTON_4;
    if (button == 4) return S_MOUSE_BUTTON_5;
    if (button == 5) return S_MOUSE_BUTTON_6;
    if (button == 6) return S_MOUSE_BUTTON_7;
    if (button == 7) return S_MOUSE_BUTTON_8;

    return -1;
}

EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData) {

    int width = Supernova::Engine::getScreenWidth();
    int height = Supernova::Engine::getScreenHeight();

    if (e->canvasX < 0) return 0;
    if (e->canvasY < 0) return 0;
    if (e->canvasX > width) return 0;
    if (e->canvasY > height) return 0;

    const float normalized_x = (e->canvasX / (float) width) * 2.f - 1.f;
    const float normalized_y = (e->canvasY / (float) height) * 2.f - 1.f;

    Supernova::Engine::onMouseMove(normalized_x, normalized_y);
    if ((e->movementX != 0 || e->movementY != 0) && e->buttons != 0) {
        Supernova::Engine::onMouseDrag(supernova_mouse_button(e->button), normalized_x, normalized_y);
    }

    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN && e->buttons != 0){
        Supernova::Engine::onMousePress(supernova_mouse_button(e->button), normalized_x, normalized_y);
    }
    if (eventType == EMSCRIPTEN_EVENT_MOUSEUP){
        Supernova::Engine::onMouseUp(supernova_mouse_button(e->button), normalized_x, normalized_y);
    }
    if (eventType == EMSCRIPTEN_EVENT_DBLCLICK){
    }

    return 0;
}

EM_BOOL wheel_callback(int eventType, const EmscriptenWheelEvent *e, void *userData) {
    /*
  printf("mouse_wheel, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, canvas: (%ld,%ld), target: (%ld, %ld), delta:(%g,%g,%g), deltaMode:%lu\n",
    e->mouse.screenX, e->mouse.screenY, e->mouse.clientX, e->mouse.clientY,
    e->mouse.ctrlKey ? " CTRL" : "", e->mouse.shiftKey ? " SHIFT" : "", e->mouse.altKey ? " ALT" : "", e->mouse.metaKey ? " META" : "",
    e->mouse.button, e->mouse.buttons, e->mouse.canvasX, e->mouse.canvasY, e->mouse.targetX, e->mouse.targetY,
    (float)e->deltaX, (float)e->deltaY, (float)e->deltaZ, e->deltaMode);
    */

  return 0;
}

void unicode_to_utf8(char *b, unsigned long c) {
	if (c<0x80) *b++=c;
	else if (c<0x800) *b++=192+c/64, *b++=128+c%64;
	else if (c-0xd800u<0x800) return;
	else if (c<0x10000) *b++=224+c/4096, *b++=128+c/64%64, *b++=128+c%64;
	else if (c<0x110000) *b++=240+c/262144, *b++=128+c/4096%64, *b++=128+c/64%64, *b++=128+c%64;
}

size_t utf8len(const char *s)
{
    size_t len = 0;
    for (; *s; ++s) if ((*s & 0xC0) != 0x80) ++len;
    return len;
}

int supernova_input(const char code[32]){
    if (!strcmp(code,"Space")) return S_KEY_SPACE;
    if (!strcmp(code,"Quote")) return S_KEY_APOSTROPHE;  /* ' */
    if (!strcmp(code,"Comma")) return S_KEY_COMMA;  /* , */
    if (!strcmp(code,"Minus")) return S_KEY_MINUS;  /* - */
    if (!strcmp(code,"Period")) return S_KEY_PERIOD;  /* . */
    if (!strcmp(code,"Slash")) return S_KEY_SLASH;  /* / */
    if (!strcmp(code,"Digit0")) return S_KEY_0;
    if (!strcmp(code,"Digit1")) return S_KEY_1;
    if (!strcmp(code,"Digit2")) return S_KEY_2;
    if (!strcmp(code,"Digit3")) return S_KEY_3;
    if (!strcmp(code,"Digit4")) return S_KEY_4;
    if (!strcmp(code,"Digit5")) return S_KEY_5;
    if (!strcmp(code,"Digit6")) return S_KEY_6;
    if (!strcmp(code,"Digit7")) return S_KEY_7;
    if (!strcmp(code,"Digit8")) return S_KEY_8;
    if (!strcmp(code,"Digit9")) return S_KEY_9;
    if (!strcmp(code,"Semicolon")) return S_KEY_SEMICOLON; /* ; */
    if (!strcmp(code,"Equal")) return S_KEY_EQUAL;  /* = */
    if (!strcmp(code,"KeyA")) return S_KEY_A;
    if (!strcmp(code,"KeyB")) return S_KEY_B;
    if (!strcmp(code,"KeyC")) return S_KEY_C;
    if (!strcmp(code,"KeyD")) return S_KEY_D;
    if (!strcmp(code,"KeyE")) return S_KEY_E;
    if (!strcmp(code,"KeyF")) return S_KEY_F;
    if (!strcmp(code,"KeyG")) return S_KEY_G;
    if (!strcmp(code,"KeyH")) return S_KEY_H;
    if (!strcmp(code,"KeyI")) return S_KEY_I;
    if (!strcmp(code,"KeyJ")) return S_KEY_J;
    if (!strcmp(code,"KeyK")) return S_KEY_K;
    if (!strcmp(code,"KeyL")) return S_KEY_L;
    if (!strcmp(code,"KeyM")) return S_KEY_M;
    if (!strcmp(code,"KeyN")) return S_KEY_N;
    if (!strcmp(code,"KeyO")) return S_KEY_O;
    if (!strcmp(code,"KeyP")) return S_KEY_P;
    if (!strcmp(code,"KeyQ")) return S_KEY_Q;
    if (!strcmp(code,"KeyR")) return S_KEY_R;
    if (!strcmp(code,"KeyS")) return S_KEY_S;
    if (!strcmp(code,"KeyT")) return S_KEY_T;
    if (!strcmp(code,"KeyU")) return S_KEY_U;
    if (!strcmp(code,"KeyV")) return S_KEY_V;
    if (!strcmp(code,"KeyW")) return S_KEY_W;
    if (!strcmp(code,"KeyX")) return S_KEY_X;
    if (!strcmp(code,"KeyY")) return S_KEY_Y;
    if (!strcmp(code,"KeyZ")) return S_KEY_Z;
    if (!strcmp(code,"BracketLeft")) return S_KEY_LEFT_BRACKET; /* [ */
    if (!strcmp(code,"Backslash")) return S_KEY_BACKSLASH;  /* \ */
    if (!strcmp(code,"BracketLeft")) return S_KEY_RIGHT_BRACKET;  /* ] */
    if (!strcmp(code,"Backquote")) return S_KEY_GRAVE_ACCENT; /* ` */

    if (!strcmp(code,"Escape")) return S_KEY_ESCAPE;
    if (!strcmp(code,"Enter")) return S_KEY_ENTER;
    if (!strcmp(code,"Tab")) return S_KEY_TAB;
    if (!strcmp(code,"Backspace")) return S_KEY_BACKSPACE;
    if (!strcmp(code,"Insert")) return S_KEY_INSERT;
    if (!strcmp(code,"Delete")) return S_KEY_DELETE;
    if (!strcmp(code,"ArrowRight")) return S_KEY_RIGHT;
    if (!strcmp(code,"ArrowLeft")) return S_KEY_LEFT;
    if (!strcmp(code,"ArrowDown")) return S_KEY_DOWN;
    if (!strcmp(code,"ArrowUp")) return S_KEY_UP;
    if (!strcmp(code,"PageUp")) return S_KEY_PAGE_UP;
    if (!strcmp(code,"PageDown")) return S_KEY_PAGE_DOWN;
    if (!strcmp(code,"Home")) return S_KEY_HOME;
    if (!strcmp(code,"End")) return S_KEY_END;
    if (!strcmp(code,"CapsLock")) return S_KEY_CAPS_LOCK;
    if (!strcmp(code,"ScrollLock")) return S_KEY_SCROLL_LOCK;
    if (!strcmp(code,"NumLock")) return S_KEY_NUM_LOCK;
    if (!strcmp(code,"PrintScreen")) return S_KEY_PRINT_SCREEN;
    if (!strcmp(code,"Pause")) return S_KEY_PAUSE;
    if (!strcmp(code,"F1")) return S_KEY_F1;
    if (!strcmp(code,"F2")) return S_KEY_F2;
    if (!strcmp(code,"F3")) return S_KEY_F3;
    if (!strcmp(code,"F4")) return S_KEY_F4;
    if (!strcmp(code,"F5")) return S_KEY_F5;
    if (!strcmp(code,"F6")) return S_KEY_F6;
    if (!strcmp(code,"F7")) return S_KEY_F7;
    if (!strcmp(code,"F8")) return S_KEY_F8;
    if (!strcmp(code,"F9")) return S_KEY_F9;
    if (!strcmp(code,"F10")) return S_KEY_F10;
    if (!strcmp(code,"F11")) return S_KEY_F11;
    if (!strcmp(code,"F12")) return S_KEY_F12;
    if (!strcmp(code,"Numpad0")) return S_KEY_KP_0;
    if (!strcmp(code,"Numpad1")) return S_KEY_KP_1;
    if (!strcmp(code,"Numpad2")) return S_KEY_KP_2;
    if (!strcmp(code,"Numpad3")) return S_KEY_KP_3;
    if (!strcmp(code,"Numpad4")) return S_KEY_KP_4;
    if (!strcmp(code,"Numpad5")) return S_KEY_KP_5;
    if (!strcmp(code,"Numpad6")) return S_KEY_KP_6;
    if (!strcmp(code,"Numpad7")) return S_KEY_KP_7;
    if (!strcmp(code,"Numpad8")) return S_KEY_KP_8;
    if (!strcmp(code,"Numpad9")) return S_KEY_KP_9;
    if (!strcmp(code,"NumpadDecimal")) return S_KEY_KP_DECIMAL;
    if (!strcmp(code,"NumpadDivide")) return S_KEY_KP_DIVIDE;
    if (!strcmp(code,"NumpadMultiply")) return S_KEY_KP_MULTIPLY;
    if (!strcmp(code,"NumpadSubtract")) return S_KEY_KP_SUBTRACT;
    if (!strcmp(code,"NumpadAdd")) return S_KEY_KP_ADD;
    if (!strcmp(code,"NumpadEnter")) return S_KEY_KP_ENTER;
    if (!strcmp(code,"NumpadEqual")) return S_KEY_KP_EQUAL;
    if (!strcmp(code,"ShiftLeft")) return S_KEY_LEFT_SHIFT;
    if (!strcmp(code,"ControlLeft")) return S_KEY_LEFT_CONTROL;
    if (!strcmp(code,"AltLeft")) return S_KEY_LEFT_ALT;
    if (!strcmp(code,"OSLeft")) return S_KEY_LEFT_SUPER;
    if (!strcmp(code,"ShiftRight")) return S_KEY_RIGHT_SHIFT;
    if (!strcmp(code,"ControlRight")) return S_KEY_RIGHT_CONTROL;
    if (!strcmp(code,"AltRight")) return S_KEY_RIGHT_ALT;
    if (!strcmp(code,"OSRight")) return S_KEY_RIGHT_SUPER;
    if (!strcmp(code,"Menu")) return S_KEY_MENU;


    return 0;
}


int supernova_legacy_input(int code){
    if (code==8) return S_KEY_DELETE;
    if (code==9) return S_KEY_TAB;
    if (code==13) return S_KEY_ENTER;
    if (code==16) return S_KEY_LEFT_SHIFT;
    if (code==17) return S_KEY_LEFT_CONTROL;
    if (code==18) return S_KEY_LEFT_ALT;
    if (code==19) return S_KEY_PAUSE;
    if (code==20) return S_KEY_CAPS_LOCK;
    if (code==27) return S_KEY_ESCAPE;
    if (code==32) return S_KEY_SPACE;
    if (code==33) return S_KEY_PAGE_UP;
    if (code==34) return S_KEY_PAGE_DOWN;
    if (code==35) return S_KEY_END;
    if (code==36) return S_KEY_HOME;
    if (code==37) return S_KEY_LEFT;
    if (code==38) return S_KEY_UP;
    if (code==39) return S_KEY_RIGHT;
    if (code==40) return S_KEY_DOWN;
    if (code==45) return S_KEY_INSERT;
    if (code==46) return S_KEY_DELETE;
    if (code==48) return S_KEY_0;
    if (code==49) return S_KEY_1;
    if (code==50) return S_KEY_2;
    if (code==51) return S_KEY_3;
    if (code==52) return S_KEY_4;
    if (code==53) return S_KEY_5;
    if (code==54) return S_KEY_6;
    if (code==55) return S_KEY_7;
    if (code==56) return S_KEY_8;
    if (code==57) return S_KEY_9;

    if (code==65) return S_KEY_A;
    if (code==66) return S_KEY_B;
    if (code==67) return S_KEY_C;
    if (code==68) return S_KEY_D;
    if (code==69) return S_KEY_E;
    if (code==70) return S_KEY_F;
    if (code==71) return S_KEY_G;
    if (code==72) return S_KEY_H;
    if (code==73) return S_KEY_I;
    if (code==74) return S_KEY_J;
    if (code==75) return S_KEY_K;
    if (code==76) return S_KEY_L;
    if (code==77) return S_KEY_M;
    if (code==78) return S_KEY_N;
    if (code==79) return S_KEY_O;
    if (code==80) return S_KEY_P;
    if (code==81) return S_KEY_Q;
    if (code==82) return S_KEY_R;
    if (code==83) return S_KEY_S;
    if (code==84) return S_KEY_T;
    if (code==85) return S_KEY_U;
    if (code==86) return S_KEY_V;
    if (code==87) return S_KEY_W;
    if (code==88) return S_KEY_X;
    if (code==89) return S_KEY_Y;
    if (code==90) return S_KEY_Z;

    if (code==91) return S_KEY_LEFT_SUPER;
    if (code==92) return S_KEY_RIGHT_SUPER;
    if (code==93) return 0; //SELECT key
    if (code==96) return S_KEY_KP_0;
    if (code==97) return S_KEY_KP_1;
    if (code==98) return S_KEY_KP_2;
    if (code==99) return S_KEY_KP_3;
    if (code==100) return S_KEY_KP_4;
    if (code==101) return S_KEY_KP_5;
    if (code==102) return S_KEY_KP_6;
    if (code==103) return S_KEY_KP_7;
    if (code==104) return S_KEY_KP_8;
    if (code==105) return S_KEY_KP_9;
    if (code==106) return S_KEY_KP_MULTIPLY;
    if (code==107) return S_KEY_KP_ADD;
    if (code==109) return S_KEY_KP_SUBTRACT;
    if (code==110) return S_KEY_KP_DECIMAL;
    if (code==111) return S_KEY_KP_DIVIDE;
    if (code==112) return S_KEY_F1;
    if (code==113) return S_KEY_F2;
    if (code==114) return S_KEY_F3;
    if (code==115) return S_KEY_F4;
    if (code==116) return S_KEY_F5;
    if (code==117) return S_KEY_F6;
    if (code==118) return S_KEY_F7;
    if (code==119) return S_KEY_F8;
    if (code==120) return S_KEY_F9;
    if (code==121) return S_KEY_F10;
    if (code==122) return S_KEY_F11;
    if (code==123) return S_KEY_F12;
    if (code==144) return S_KEY_NUM_LOCK;
    if (code==145) return S_KEY_SCROLL_LOCK;
    if (code==186) return S_KEY_SEMICOLON;
    if (code==187) return S_KEY_EQUAL;
    if (code==188) return S_KEY_COMMA;
    if (code==189) return S_KEY_MINUS;
    if (code==190) return S_KEY_PERIOD;
    if (code==191) return S_KEY_SLASH;
    if (code==192) return S_KEY_GRAVE_ACCENT;
    if (code==219) return S_KEY_LEFT_BRACKET;
    if (code==220) return S_KEY_BACKSLASH;
    if (code==222) return S_KEY_APOSTROPHE;

    return 0;
}

EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData){

  const char *key=e->key;
  char keybuff[5] = {0};
  if ( (!key)||(!(*key)) ){
      unicode_to_utf8(keybuff, e->keyCode);
      key = keybuff;
  }
  if (!(*key)){
      unicode_to_utf8(keybuff, e->which);
      key = keybuff;
  }

  int code = supernova_input(e->code);
  if (code==0){
      code = supernova_legacy_input(e->which);
  }
  if (code==0){
      code = supernova_legacy_input(e->keyCode);
  }

   int skey=0;
   if ((!strcmp(key,"Tab"))||(*key=='\t')) skey=1;
   if ((!strcmp(key,"Backspace"))||(*key=='\b')) skey=2;
   if ((!strcmp(key,"Enter"))||(*key=='\r')) skey=4;
   if ((!strcmp(key,"Escape"))||(*key=='\e')) skey=8;

   if (eventType == EMSCRIPTEN_EVENT_KEYDOWN){
       Supernova::Engine::onKeyPress(code);
       if (skey==1) Supernova::Engine::onTextInput("\t");
       if (skey==2) Supernova::Engine::onTextInput("\b");
       if (skey==4) Supernova::Engine::onTextInput("\r");
       if (skey==8) Supernova::Engine::onTextInput("\e");
   }else if (eventType == EMSCRIPTEN_EVENT_KEYUP){
       Supernova::Engine::onKeyUp(code);
   }else if (eventType == EMSCRIPTEN_EVENT_KEYPRESS){
       if ((utf8len(key)==1)&&(!skey)){
           Supernova::Engine::onTextInput(key);
       }
    }

 return 0;
}
