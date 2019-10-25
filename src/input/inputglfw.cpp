#include "inputglfw.h"
#include <GLFW/glfw3.h>

namespace evnt
{
Keyboard::Key MapKeyCode(int platformKeyCode)
{
    Keyboard::Key key = Keyboard::Key_Invalid;

    switch(platformKeyCode)
    {
        case GLFW_KEY_SPACE:
            key = Keyboard::Key_Spacebar;
            break;
        case GLFW_KEY_APOSTROPHE:
            key = Keyboard::Key_Apostrophe;
            break;
        case GLFW_KEY_COMMA:
            key = Keyboard::Key_Comma;
            break;
        case GLFW_KEY_MINUS:
            key = Keyboard::Key_Minus;
            break;
        case GLFW_KEY_PERIOD:
            key = Keyboard::Key_Period;
            break;
        case GLFW_KEY_SLASH:
            key = Keyboard::Key_Slash;
            break;
        case GLFW_KEY_0:
            key = Keyboard::Key_0;
            break;
        case GLFW_KEY_1:
            key = Keyboard::Key_1;
            break;
        case GLFW_KEY_2:
            key = Keyboard::Key_2;
            break;
        case GLFW_KEY_3:
            key = Keyboard::Key_3;
            break;
        case GLFW_KEY_4:
            key = Keyboard::Key_4;
            break;
        case GLFW_KEY_5:
            key = Keyboard::Key_5;
            break;
        case GLFW_KEY_6:
            key = Keyboard::Key_6;
            break;
        case GLFW_KEY_7:
            key = Keyboard::Key_7;
            break;
        case GLFW_KEY_8:
            key = Keyboard::Key_8;
            break;
        case GLFW_KEY_9:
            key = Keyboard::Key_9;
            break;
        case GLFW_KEY_SEMICOLON:
            key = Keyboard::Key_Semicolon;
            break;
        case GLFW_KEY_EQUAL:
            key = Keyboard::Key_Equals;
            break;
        case GLFW_KEY_A:
            key = Keyboard::Key_A;
            break;
        case GLFW_KEY_B:
            key = Keyboard::Key_B;
            break;
        case GLFW_KEY_C:
            key = Keyboard::Key_C;
            break;
        case GLFW_KEY_D:
            key = Keyboard::Key_D;
            break;
        case GLFW_KEY_E:
            key = Keyboard::Key_E;
            break;
        case GLFW_KEY_F:
            key = Keyboard::Key_F;
            break;
        case GLFW_KEY_G:
            key = Keyboard::Key_G;
            break;
        case GLFW_KEY_H:
            key = Keyboard::Key_H;
            break;
        case GLFW_KEY_I:
            key = Keyboard::Key_I;
            break;
        case GLFW_KEY_J:
            key = Keyboard::Key_J;
            break;
        case GLFW_KEY_K:
            key = Keyboard::Key_K;
            break;
        case GLFW_KEY_L:
            key = Keyboard::Key_L;
            break;
        case GLFW_KEY_M:
            key = Keyboard::Key_M;
            break;
        case GLFW_KEY_N:
            key = Keyboard::Key_N;
            break;
        case GLFW_KEY_O:
            key = Keyboard::Key_O;
            break;
        case GLFW_KEY_P:
            key = Keyboard::Key_P;
            break;
        case GLFW_KEY_Q:
            key = Keyboard::Key_Q;
            break;
        case GLFW_KEY_R:
            key = Keyboard::Key_R;
            break;
        case GLFW_KEY_S:
            key = Keyboard::Key_S;
            break;
        case GLFW_KEY_T:
            key = Keyboard::Key_T;
            break;
        case GLFW_KEY_U:
            key = Keyboard::Key_U;
            break;
        case GLFW_KEY_V:
            key = Keyboard::Key_V;
            break;
        case GLFW_KEY_W:
            key = Keyboard::Key_W;
            break;
        case GLFW_KEY_X:
            key = Keyboard::Key_X;
            break;
        case GLFW_KEY_Y:
            key = Keyboard::Key_Y;
            break;
        case GLFW_KEY_Z:
            key = Keyboard::Key_Z;
            break;
        case GLFW_KEY_LEFT_BRACKET:
            key = Keyboard::Key_LeftBracket;
            break;
        case GLFW_KEY_BACKSLASH:
            key = Keyboard::Key_Backslash;
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            key = Keyboard::Key_RightBracket;
            break;
        case GLFW_KEY_GRAVE_ACCENT:
            key = Keyboard::Key_Grave;
            break;
        case GLFW_KEY_ESCAPE:
            key = Keyboard::Key_Escape;
            break;
        case GLFW_KEY_ENTER:
            key = Keyboard::Key_Enter;
            break;
        case GLFW_KEY_TAB:
            key = Keyboard::Key_Tab;
            break;
        case GLFW_KEY_BACKSPACE:
            key = Keyboard::Key_Backspace;
            break;
        case GLFW_KEY_INSERT:
            key = Keyboard::Key_Insert;
            break;
        case GLFW_KEY_DELETE:
            key = Keyboard::Key_Delete;
            break;
        case GLFW_KEY_RIGHT:
            key = Keyboard::Key_RightArrow;
            break;
        case GLFW_KEY_LEFT:
            key = Keyboard::Key_LeftArrow;
            break;
        case GLFW_KEY_DOWN:
            key = Keyboard::Key_DownArrow;
            break;
        case GLFW_KEY_UP:
            key = Keyboard::Key_UpArrow;
            break;
        case GLFW_KEY_PAGE_UP:
            key = Keyboard::Key_PageUp;
            break;
        case GLFW_KEY_PAGE_DOWN:
            key = Keyboard::Key_PageDown;
            break;
        case GLFW_KEY_HOME:
            key = Keyboard::Key_Home;
            break;
        case GLFW_KEY_END:
            key = Keyboard::Key_End;
            break;
        case GLFW_KEY_CAPS_LOCK:
            key = Keyboard::Key_CapsLock;
            break;
        case GLFW_KEY_SCROLL_LOCK:
            key = Keyboard::Key_ScrollLock;
            break;
        case GLFW_KEY_NUM_LOCK:
            key = Keyboard::Key_NumLock;
            break;
        case GLFW_KEY_PRINT_SCREEN:
            key = Keyboard::Key_PrintScreen;
            break;
        case GLFW_KEY_PAUSE:
            key = Keyboard::Key_Pause;
            break;
        case GLFW_KEY_F1:
            key = Keyboard::Key_F1;
            break;
        case GLFW_KEY_F2:
            key = Keyboard::Key_F2;
            break;
        case GLFW_KEY_F3:
            key = Keyboard::Key_F3;
            break;
        case GLFW_KEY_F4:
            key = Keyboard::Key_F4;
            break;
        case GLFW_KEY_F5:
            key = Keyboard::Key_F5;
            break;
        case GLFW_KEY_F6:
            key = Keyboard::Key_F6;
            break;
        case GLFW_KEY_F7:
            key = Keyboard::Key_F7;
            break;
        case GLFW_KEY_F8:
            key = Keyboard::Key_F8;
            break;
        case GLFW_KEY_F9:
            key = Keyboard::Key_F9;
            break;
        case GLFW_KEY_F10:
            key = Keyboard::Key_F10;
            break;
        case GLFW_KEY_F11:
            key = Keyboard::Key_F11;
            break;
        case GLFW_KEY_F12:
            key = Keyboard::Key_F12;
            break;
        case GLFW_KEY_F13:
            key = Keyboard::Key_F13;
            break;
        case GLFW_KEY_F14:
            key = Keyboard::Key_F14;
            break;
        case GLFW_KEY_F15:
            key = Keyboard::Key_F15;
            break;
        case GLFW_KEY_F16:
            key = Keyboard::Key_F16;
            break;
        case GLFW_KEY_F17:
            key = Keyboard::Key_F17;
            break;
        case GLFW_KEY_F18:
            key = Keyboard::Key_F18;
            break;
        case GLFW_KEY_F19:
            key = Keyboard::Key_F19;
            break;
        case GLFW_KEY_F20:
            key = Keyboard::Key_F20;
            break;
        case GLFW_KEY_F21:
            key = Keyboard::Key_F21;
            break;
        case GLFW_KEY_F22:
            key = Keyboard::Key_F22;
            break;
        case GLFW_KEY_F23:
            key = Keyboard::Key_F23;
            break;
        case GLFW_KEY_F24:
            key = Keyboard::Key_F24;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            key = Keyboard::Key_LeftShift;
            break;
        case GLFW_KEY_LEFT_CONTROL:
            key = Keyboard::Key_LeftCtrl;
            break;
        case GLFW_KEY_LEFT_ALT:
            key = Keyboard::Key_LeftAlt;
            break;
        case GLFW_KEY_LEFT_SUPER:
            key = Keyboard::Key_LeftWin;
            break;
        case GLFW_KEY_RIGHT_SHIFT:
            key = Keyboard::Key_RightShift;
            break;
        case GLFW_KEY_RIGHT_CONTROL:
            key = Keyboard::Key_RightCtrl;
            break;
        case GLFW_KEY_RIGHT_ALT:
            key = Keyboard::Key_RightAlt;
            break;
        case GLFW_KEY_RIGHT_SUPER:
            key = Keyboard::Key_RightWin;
            break;
        case GLFW_KEY_MENU:
            key = Keyboard::Key_Menu;
            break;
        default:
            key = Keyboard::Key_Invalid;
            break;
    }

    return key;
}

InputGLFW::InputGLFW() {}
}   // namespace evnt
