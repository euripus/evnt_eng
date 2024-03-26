#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>

#define INPUT_KEYBOARD_MASK    0x0000ffff
#define INPUT_MOUSE_MASK       0x2000000f
#define INPUT_MOUSEWHEEL_MASK  0x1000000f
#define INPUT_MASK             (INPUT_KEYBOARD_MASK | INPUT_MOUSE_MASK | INPUT_MOUSEWHEEL_MASK)
#define INPUT_COMBINATION_MASK (~INPUT_MASK)

namespace evnt
{
inline static std::string const PREFIX_KEY    = "Key_";
inline static std::string const PREFIX_BUTTON = "Button";
inline static char const        SEPARATOR     = '+';

class Keyboard
{
public:
    //! Key enumeration.
    enum Key
    {
        Key_Invalid = -1,

        KeyBase = 0,

        Key_0 = KeyBase,
        Key_1,
        Key_2,
        Key_3,
        Key_4,
        Key_5,
        Key_6,
        Key_7,
        Key_8,
        Key_9,

        Key_A,
        Key_B,
        Key_C,
        Key_D,
        Key_E,
        Key_F,
        Key_G,
        Key_H,
        Key_I,
        Key_J,
        Key_K,
        Key_L,
        Key_M,
        Key_N,
        Key_O,
        Key_P,
        Key_Q,
        Key_R,
        Key_S,
        Key_T,
        Key_U,
        Key_V,
        Key_W,
        Key_X,
        Key_Y,
        Key_Z,

        Key_F1,
        Key_F2,
        Key_F3,
        Key_F4,
        Key_F5,
        Key_F6,
        Key_F7,
        Key_F8,
        Key_F9,
        Key_F10,
        Key_F11,
        Key_F12,
        Key_F13,
        Key_F14,
        Key_F15,
        Key_F16,
        Key_F17,
        Key_F18,
        Key_F19,
        Key_F20,
        Key_F21,
        Key_F22,
        Key_F23,
        Key_F24,

        Key_UpArrow,
        Key_DownArrow,
        Key_LeftArrow,
        Key_RightArrow,

        Key_Home,
        Key_End,
        Key_PageUp,
        Key_PageDown,
        Key_Insert,
        Key_Delete,

        Key_Escape,

        Key_NumLock,
        Key_NumPad0,
        Key_NumPad1,
        Key_NumPad2,
        Key_NumPad3,
        Key_NumPad4,
        Key_NumPad5,
        Key_NumPad6,
        Key_NumPad7,
        Key_NumPad8,
        Key_NumPad9,
        Key_NumPadEnter,
        Key_NumSubtract,   // (-) on numeric keypad
        Key_NumAdd,        // (+) on numeric keypad
        Key_NumMultiply,   // (*) on numeric keypad
        Key_NumDivide,     // (/) on numeric keypad
        Key_NumPoint,      // PERIOD (decimal point) on numeric keypad
        Key_NumComma,
        Key_Num00,
        Key_Num000,
        Key_NumLeftParen,
        Key_NumRightParen,
        Key_NumLeftBrace,
        Key_NumRightBrace,
        Key_NumTab,
        Key_NumBackspace,
        Key_NumA,
        Key_NumB,
        Key_NumC,
        Key_NumD,
        Key_NumE,
        Key_NumF,
        Key_NumXor,
        Key_NumPower,
        Key_NumPercent,
        Key_NumLess,
        Key_NumGreater,
        Key_NumAmpersand,
        Key_NumDblAmpersand,
        Key_NumVerticalBar,
        Key_NumDblVerticalBar,
        Key_NumColon,
        Key_NumHash,
        Key_NumSpace,
        Key_NumAt,
        Key_NumExclam,
        Key_NumMemStore,
        Key_NumMemRecall,
        Key_NumMemClear,
        Key_NumMemAdd,
        Key_NumMemSubtract,
        Key_NumMemMultiply,
        Key_NumMemDivide,
        Key_NumPlusMinus,
        Key_NumClear,
        Key_NumClearEntry,
        Key_NumBinary,
        Key_NumOctal,
        Key_NumDecimal,
        Key_NumHexadecimal,

        Key_LeftBracket,   // Left square bracket [
        Key_LeftCtrl,      // Left CTRL
        Key_LeftAlt,       // Left ALT
        Key_LeftShift,     // Left SHIFT
        Key_LeftWin,       // Left Windows logo key

        Key_RightBracket,   // Right square bracket ]
        Key_RightCtrl,      // Right CTRL
        Key_RightAlt,       // Right ALT
        Key_RightShift,     // Right SHIFT
        Key_RightWin,       // Right Windows logo key

        Key_PrintScreen,
        Key_ScrollLock,
        Key_Pause,

        Key_Spacebar,
        Key_Backspace,
        Key_Enter,   // ENTER on main keyboard
        Key_Tab,

        Key_Apps,   // Application key
        Key_CapsLock,

        Key_Slash,        // (/) On main keyboard
        Key_Backslash,    // (\)
        Key_Comma,        // (,)
        Key_Semicolon,    // (;)
        Key_Period,       // (.) On main keyboard
        Key_Grave,        // (`) Grave accent
        Key_Apostrophe,   // (')
        Key_Minus,        // (-) On main keyboard
        Key_Equals,       // (=) On main keyboard

        Key_Execute,
        Key_Help,
        Key_Menu,
        Key_Select,
        Key_Stop,
        Key_Redo,
        Key_Undo,
        Key_Cut,
        Key_Copy,
        Key_Paste,
        Key_Find,

        Key_Mute,
        Key_VolumeUp,
        Key_VolumeDown,

        Key_International1,
        Key_International2,
        Key_International3,
        Key_International4,
        Key_International5,
        Key_International6,
        Key_International7,
        Key_International8,
        Key_International9,

        Key_Lang1,
        Key_Lang2,
        Key_Lang3,
        Key_Lang4,
        Key_Lang5,
        Key_Lang6,
        Key_Lang7,
        Key_Lang8,
        Key_Lang9,

        Key_AltErase,
        Key_SysReq,
        Key_Cancel,
        Key_Clear,
        Key_Prior,
        Key_Return2,
        Key_Separator,
        Key_Out,
        Key_Oper,
        Key_ClearAgain,
        Key_CrSel,
        Key_ExSel,

        Key_ThousandsSeparator,
        Key_DecimalSeparator,
        Key_CurrencyUnit,
        Key_CurrencySubUnit,

        Key_AudioNext,
        Key_AudioPrev,
        Key_AudioStop,
        Key_AudioPlay,
        Key_AudioMute,
        Key_Media,

        Key_WWW,
        Key_Mail,
        Key_Calculator,
        Key_Computer,

        Key_ACSearch,
        Key_ACHome,
        Key_ACBack,
        Key_ACForward,
        Key_ACStop,
        Key_ACRefresh,
        Key_ACBookmarks,

        KeyMax,
        KeyCount = KeyMax - KeyBase
    };

    inline static bool IsModifier(int32_t key)
    {
        switch(key)
        {
            case Keyboard::Key_LeftShift:
            case Keyboard::Key_RightShift:
            case Keyboard::Key_LeftCtrl:
            case Keyboard::Key_RightCtrl:
            case Keyboard::Key_LeftAlt:
            case Keyboard::Key_RightAlt:
                return true;
            default:
                return false;
        }
    }
};

class Mouse
{
public:
    enum Button
    {
        Button_Invalid = -1,

        ButtonBase = 0x20000000,

        Button_0 = ButtonBase,   // left
        Button_1,                // right
        Button_2,                // middle
        Button_3,
        Button_4,
        Button_5,
        Button_6,
        Button_7,

        ButtonMax,
        ButtonCount = ButtonMax - ButtonBase
    };

    enum Wheel
    {
        WheelBase = 0x10000000,

        Wheel_Up = WheelBase,
        Wheel_Down
    };
};
}   // namespace evnt

#endif   // KEYBOARD_H
