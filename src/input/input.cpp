#include "input.h"
#include "keybinding.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <cassert>
#include <map>
#include <sstream>

namespace evnt
{
static std::map<std::string, int32_t> s_key_names;

struct KeyDescription
{
    int32_t     id;
    std::string name;
};

// All standard keys
// "+" should not appear in names as it is used as a separator
static const KeyDescription s_keys_descriptions[] = {
    {Keyboard::Key_0, "0"},
    {Keyboard::Key_1, "1"},
    {Keyboard::Key_2, "2"},
    {Keyboard::Key_3, "3"},
    {Keyboard::Key_4, "4"},
    {Keyboard::Key_5, "5"},
    {Keyboard::Key_6, "6"},
    {Keyboard::Key_7, "7"},
    {Keyboard::Key_8, "8"},
    {Keyboard::Key_9, "9"},
    {Keyboard::Key_A, "A"},
    {Keyboard::Key_B, "B"},
    {Keyboard::Key_C, "C"},
    {Keyboard::Key_D, "D"},
    {Keyboard::Key_E, "E"},
    {Keyboard::Key_F, "F"},
    {Keyboard::Key_G, "G"},
    {Keyboard::Key_H, "H"},
    {Keyboard::Key_I, "I"},
    {Keyboard::Key_J, "J"},
    {Keyboard::Key_K, "K"},
    {Keyboard::Key_L, "L"},
    {Keyboard::Key_M, "M"},
    {Keyboard::Key_N, "N"},
    {Keyboard::Key_O, "O"},
    {Keyboard::Key_P, "P"},
    {Keyboard::Key_Q, "Q"},
    {Keyboard::Key_R, "R"},
    {Keyboard::Key_S, "S"},
    {Keyboard::Key_T, "T"},
    {Keyboard::Key_U, "U"},
    {Keyboard::Key_V, "V"},
    {Keyboard::Key_W, "W"},
    {Keyboard::Key_X, "X"},
    {Keyboard::Key_Y, "Y"},
    {Keyboard::Key_Z, "Z"},
    {Keyboard::Key_F1, "F1"},
    {Keyboard::Key_F2, "F2"},
    {Keyboard::Key_F3, "F3"},
    {Keyboard::Key_F4, "F4"},
    {Keyboard::Key_F5, "F5"},
    {Keyboard::Key_F6, "F6"},
    {Keyboard::Key_F7, "F7"},
    {Keyboard::Key_F8, "F8"},
    {Keyboard::Key_F9, "F9"},
    {Keyboard::Key_F10, "F10"},
    {Keyboard::Key_F11, "F11"},
    {Keyboard::Key_F12, "F12"},
    {Keyboard::Key_F13, "F13"},
    {Keyboard::Key_F14, "F14"},
    {Keyboard::Key_F15, "F15"},
    {Keyboard::Key_F16, "F16"},
    {Keyboard::Key_F17, "F17"},
    {Keyboard::Key_F18, "F18"},
    {Keyboard::Key_F19, "F19"},
    {Keyboard::Key_F20, "F20"},
    {Keyboard::Key_F21, "F21"},
    {Keyboard::Key_F22, "F22"},
    {Keyboard::Key_F23, "F23"},
    {Keyboard::Key_F24, "F24"},
    {Keyboard::Key_UpArrow, "Up"},
    {Keyboard::Key_DownArrow, "Down"},
    {Keyboard::Key_LeftArrow, "Left"},
    {Keyboard::Key_RightArrow, "Right"},
    {Keyboard::Key_Home, "Home"},
    {Keyboard::Key_End, "End"},
    {Keyboard::Key_PageUp, "PageUp"},
    {Keyboard::Key_PageDown, "PageDown"},
    {Keyboard::Key_Insert, "Insert"},
    {Keyboard::Key_Delete, "Delete"},
    {Keyboard::Key_Escape, "Escape"},
    {Keyboard::Key_NumLock, "NumLock"},
    {Keyboard::Key_NumPad0, "Numpad0"},
    {Keyboard::Key_NumPad1, "Numpad1"},
    {Keyboard::Key_NumPad2, "Numpad2"},
    {Keyboard::Key_NumPad3, "Numpad3"},
    {Keyboard::Key_NumPad4, "Numpad4"},
    {Keyboard::Key_NumPad5, "Numpad5"},
    {Keyboard::Key_NumPad6, "Numpad6"},
    {Keyboard::Key_NumPad7, "Numpad7"},
    {Keyboard::Key_NumPad8, "Numpad8"},
    {Keyboard::Key_NumPad9, "Numpad9"},
    {Keyboard::Key_NumPadEnter, "NumpadReturn"},
    {Keyboard::Key_NumSubtract, "Numpad-"},
    {Keyboard::Key_NumAdd, "NumpadPlus"},
    {Keyboard::Key_NumMultiply, "Multiply"},
    {Keyboard::Key_NumDivide, "Numpad/"},
    {Keyboard::Key_NumPoint, "Numpad."},
    {Keyboard::Key_NumComma, "Numpad,"},
    {Keyboard::Key_Num00, "Numpad00"},
    {Keyboard::Key_Num000, "Numpad000"},
    {Keyboard::Key_NumLeftParen, "Numpad("},
    {Keyboard::Key_NumRightParen, "Numpad)"},
    {Keyboard::Key_NumLeftBrace, "Numpad{"},
    {Keyboard::Key_NumRightBrace, "Numpad}"},
    {Keyboard::Key_NumTab, "NumpadTab"},
    {Keyboard::Key_NumBackspace, "NumpadBackspace"},
    {Keyboard::Key_NumA, "NumpadA"},
    {Keyboard::Key_NumB, "NumpadB"},
    {Keyboard::Key_NumC, "NumpadC"},
    {Keyboard::Key_NumD, "NumpadD"},
    {Keyboard::Key_NumE, "NumpadE"},
    {Keyboard::Key_NumF, "NumpadF"},
    {Keyboard::Key_NumXor, "NumpadXor"},
    {Keyboard::Key_NumPower, "NumpadPower"},
    {Keyboard::Key_NumPercent, "Numpad%"},
    {Keyboard::Key_NumLess, "Numpad<"},
    {Keyboard::Key_NumGreater, "Numpad>"},
    {Keyboard::Key_NumAmpersand, "Numpad&"},
    {Keyboard::Key_NumDblAmpersand, "Numpad&&"},
    {Keyboard::Key_NumVerticalBar, "Numpad|"},
    {Keyboard::Key_NumDblVerticalBar, "Numpad||"},
    {Keyboard::Key_NumColon, "Numpad:"},
    {Keyboard::Key_NumHash, "Numpad#"},
    {Keyboard::Key_NumSpace, "NumpadSpace"},
    {Keyboard::Key_NumAt, "Numpad@"},
    {Keyboard::Key_NumExclam, "Numpad!"},
    {Keyboard::Key_NumMemStore, "NumpadMemStore"},
    {Keyboard::Key_NumMemRecall, "NumpadMemRecall"},
    {Keyboard::Key_NumMemClear, "NumpadMemClear"},
    {Keyboard::Key_NumMemAdd, "NumpadMemAdd"},
    {Keyboard::Key_NumMemSubtract, "NumpadMemSubtract"},
    {Keyboard::Key_NumMemMultiply, "NumpadMemMultiply"},
    {Keyboard::Key_NumMemDivide, "NumpadMemDivide"},
    {Keyboard::Key_NumPlusMinus, "NumpadPlusMinus"},
    {Keyboard::Key_NumClear, "NumpadClear"},
    {Keyboard::Key_NumClearEntry, "NumpadClearEntry"},
    {Keyboard::Key_NumBinary, "NumBinary"},
    {Keyboard::Key_NumOctal, "NumOctal"},
    {Keyboard::Key_NumDecimal, "NumDecimal"},
    {Keyboard::Key_NumHexadecimal, "NumHexadecimal"},
    {Keyboard::Key_LeftBracket, "["},
    {Keyboard::Key_LeftCtrl, "LeftControl"},
    {Keyboard::Key_LeftAlt, "LeftAlt"},
    {Keyboard::Key_LeftShift, "LeftShift"},
    {Keyboard::Key_LeftWin, "LeftStart"},
    {Keyboard::Key_RightBracket, "]"},
    {Keyboard::Key_RightCtrl, "RightControl"},
    {Keyboard::Key_RightAlt, "RightAlt"},
    {Keyboard::Key_RightShift, "RightShift"},
    {Keyboard::Key_RightWin, "RightStart"},
    {Keyboard::Key_PrintScreen, "PrintScreen"},
    {Keyboard::Key_ScrollLock, "ScrollLock"},
    {Keyboard::Key_Pause, "Pause"},
    {Keyboard::Key_Spacebar, "Space"},
    {Keyboard::Key_Backspace, "Backspace"},
    {Keyboard::Key_Enter, "Return"},
    {Keyboard::Key_Tab, "Tab"},
    {Keyboard::Key_Apps, "AppMenu"},
    {Keyboard::Key_CapsLock, "Capital"},
    {Keyboard::Key_Slash, "/"},
    {Keyboard::Key_Backslash, "Backslash"},
    {Keyboard::Key_Comma, ","},
    {Keyboard::Key_Semicolon, ";"},
    {Keyboard::Key_Period, "."},
    {Keyboard::Key_Grave, "`"},
    {Keyboard::Key_Apostrophe, "'"},
    {Keyboard::Key_Minus, "-"},
    {Keyboard::Key_Equals, "="},
    {Keyboard::Key_Execute, "Execute"},
    {Keyboard::Key_Help, "Help"},
    {Keyboard::Key_Menu, "Menu"},
    {Keyboard::Key_Select, "Select"},
    {Keyboard::Key_Stop, "Stop"},
    {Keyboard::Key_Redo, "Redo"},
    {Keyboard::Key_Undo, "Undo"},
    {Keyboard::Key_Cut, "Cut"},
    {Keyboard::Key_Copy, "Copy"},
    {Keyboard::Key_Paste, "Paste"},
    {Keyboard::Key_Find, "Find"},
    {Keyboard::Key_Mute, "Mute"},
    {Keyboard::Key_VolumeUp, "VolumeUp"},
    {Keyboard::Key_VolumeDown, "VolumeDown"},
    {Keyboard::Key_International1, "International1"},
    {Keyboard::Key_International2, "International2"},
    {Keyboard::Key_International3, "International3"},
    {Keyboard::Key_International4, "International4"},
    {Keyboard::Key_International5, "International5"},
    {Keyboard::Key_International6, "International6"},
    {Keyboard::Key_International7, "International7"},
    {Keyboard::Key_International8, "International8"},
    {Keyboard::Key_International9, "International9"},
    {Keyboard::Key_Lang1, "Lang1"},
    {Keyboard::Key_Lang2, "Lang2"},
    {Keyboard::Key_Lang3, "Lang3"},
    {Keyboard::Key_Lang4, "Lang4"},
    {Keyboard::Key_Lang5, "Lang5"},
    {Keyboard::Key_Lang6, "Lang6"},
    {Keyboard::Key_Lang7, "Lang7"},
    {Keyboard::Key_Lang8, "Lang8"},
    {Keyboard::Key_Lang9, "Lang9"},
    {Keyboard::Key_AltErase, "AltErase"},
    {Keyboard::Key_SysReq, "SysReq"},
    {Keyboard::Key_Cancel, "Cancel"},
    {Keyboard::Key_Clear, "Clear"},
    {Keyboard::Key_Prior, "Prior"},
    {Keyboard::Key_Return2, "Return2"},
    {Keyboard::Key_Separator, "Separator"},
    {Keyboard::Key_Out, "Out"},
    {Keyboard::Key_Oper, "Oper"},
    {Keyboard::Key_ClearAgain, "ClearAgain"},
    {Keyboard::Key_CrSel, "CrSel"},
    {Keyboard::Key_ExSel, "ExSel"},
    {Keyboard::Key_ThousandsSeparator, "ThousandsSeparator"},
    {Keyboard::Key_DecimalSeparator, "DecimalSeparator"},
    {Keyboard::Key_CurrencyUnit, "CurrencyUnit"},
    {Keyboard::Key_CurrencySubUnit, "CurrencySubUnit"},
    {Keyboard::Key_AudioNext, "AudioNext"},
    {Keyboard::Key_AudioPrev, "AudioPrev"},
    {Keyboard::Key_AudioStop, "AudioStop"},
    {Keyboard::Key_AudioPlay, "AudioPlay"},
    {Keyboard::Key_AudioMute, "AudioMute"},
    {Keyboard::Key_Media, "Media"},
    {Keyboard::Key_WWW, "WWW"},
    {Keyboard::Key_Mail, "Mail"},
    {Keyboard::Key_Calculator, "Calculator"},
    {Keyboard::Key_Computer, "Computer"},
    {Keyboard::Key_ACSearch, "ACSearch"},
    {Keyboard::Key_ACHome, "ACHome"},
    {Keyboard::Key_ACBack, "ACBack"},
    {Keyboard::Key_ACForward, "ACForward"},
    {Keyboard::Key_ACStop, "ACStop"},
    {Keyboard::Key_ACRefresh, "ACRefresh"},
    {Keyboard::Key_ACBookmarks, "ACBookmarks"},
};

void Input::onCursorPos(int32_t xpos, int32_t ypos)
{
    setMousePosAbs(glm::ivec2(xpos, ypos));

    if(m_mouse_cursor_callback)
    {
        m_mouse_cursor_callback(xpos, ypos);
    }
}

void Input::onMouseButton(int32_t button_code, bool press)
{
    m_mouse_buttons_state[button_code - Mouse::ButtonBase] = press;

    if(m_mouse_button_callback)
    {
        m_mouse_button_callback(button_code, press);
    }
}

void Input::onMouseWheel(int32_t xoffset, int32_t yoffset)
{
    m_mouse_wheel = yoffset;

    if(m_mouse_wheel_callback)
    {
        m_mouse_wheel_callback(xoffset, yoffset);
    }
}

void Input::onKey(int32_t key_code, bool press)
{
    m_key_id                                    = key_code;
    m_keys_states[key_code - Keyboard::KeyBase] = press;

    if(press)
    {
        KeyBinding::KeyPressed(key_code);
    }

    if(m_key_callback)
    {
        m_key_callback(key_code, press);
    }
}

Input::Input() : m_key_id{-1}, m_mouse_position{0, 0}, m_mouse_wheel{0}
{
    reset();
}

void Input::reset()
{
    std::fill(std::begin(m_keys_states), std::end(m_keys_states), false);
    std::fill(std::begin(m_mouse_buttons_state), std::end(m_mouse_buttons_state), false);
    m_key_id      = -1;
    m_mouse_wheel = 0;
}

bool Input::getMouseButton(int32_t button_id) const
{
    assert(button_id >= Mouse::ButtonBase && button_id < Mouse::ButtonMax);

    int32_t button_idx = button_id - Mouse::ButtonBase;
    return m_mouse_buttons_state[button_idx];
}

int32_t Input::getMouseButtonClicked() const
{
    int32_t button_idx = -1;

    for(int32_t i = Mouse::ButtonBase; i < Mouse::ButtonMax; i++)
    {
        button_idx = i - Mouse::ButtonBase;
        if(m_mouse_buttons_state[button_idx])
        {
            return i;
        }
    }
    return button_idx;
}

bool Input::isKeyPressed(int32_t key_id) const
{
    assert(key_id >= Keyboard::KeyBase && key_id < Keyboard::KeyMax);
    return m_keys_states[key_id - Keyboard::KeyBase];
}

std::string Input::GetKeyName(int32_t key)
{
    if(key == -1)
    {
        return std::string();
    }

    std::string name;

    std::string modifier;
    if(key & INPUT_COMBINATION_MASK)
    {
        // key combination
        modifier = GetKeyName((key >> 16) & 0x0fff);
        key &= INPUT_MASK;
    }

    if(key >= Mouse::ButtonBase && key < Mouse::ButtonMax)
    {
        std::ostringstream oss;
        oss << PREFIX_BUTTON << (key - Mouse::ButtonBase + 1);
        name = oss.str();
    }
    else if(key == Mouse::Wheel_Up)
    {
        name = "WheelUp";
    }
    else if(key == Mouse::Wheel_Down)
    {
        name = "WheelDown";
    }
    else if(key >= Keyboard::KeyBase && key < Keyboard::KeyMax)
    {
        static_assert(std::size(s_keys_descriptions) == size_t(Keyboard::KeyMax - Keyboard::KeyBase));

        const KeyDescription & entity = s_keys_descriptions[key - Keyboard::KeyBase];
        assert(entity.id == key);
        name = entity.name;
    }

    if(!modifier.empty())
    {
        return modifier + SEPARATOR + name;
    }
    else
    {
        return name;
    }
}

int32_t Input::GetKeyId(const std::string & key_name)
{
    // If a noneset key, return -1
    if(key_name.empty() || key_name == KEY_NONE)
    {
        return -1;
    }

    size_t sep = key_name.find(SEPARATOR);
    if(sep != std::string::npos)
    {
        int32_t modifier = GetKeyId(key_name.substr(0, sep));
        int32_t key      = GetKeyId(key_name.substr(sep + 1));
        return (modifier < 0) ? key : (modifier << 16 | key);
    }

    if(!key_name.compare(0, PREFIX_KEY.length(), PREFIX_KEY))
    {
        try
        {
            int32_t key = boost::lexical_cast<int32_t>(key_name.substr(PREFIX_KEY.length()));
            return key;
        }
        catch(const boost::bad_lexical_cast &)
        {}
    }

    if(!key_name.compare(0, PREFIX_BUTTON.length(), PREFIX_BUTTON))
    {
        try
        {
            int32_t key = boost::lexical_cast<int32_t>(key_name.substr(PREFIX_BUTTON.length()));
            if(key >= 0 && key < Mouse::ButtonCount)
            {
                return Mouse::ButtonBase + key - 1;
            }
        }
        catch(const boost::bad_lexical_cast &)
        {}
    }

    if(s_key_names.empty())
    {
        // Initialize the key name -> id map.
        for(size_t i = 0; i < size_t(std::size(s_keys_descriptions)); i++)
        {
            s_key_names[s_keys_descriptions[i].name] = s_keys_descriptions[i].id;
        }
        s_key_names["WheelUp"]   = Mouse::Wheel_Up;
        s_key_names["WheelDown"] = Mouse::Wheel_Down;
    }

    auto it = s_key_names.find(key_name);
    if(it != s_key_names.end())
    {
        return it->second;
    }

    return -1;
}
}   // namespace evnt
