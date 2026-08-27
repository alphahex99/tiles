#include "Keybinds.hpp"

// IT(name)
#define KEY_DEF()                                                                                                      \
    /* Alphanumeric keys */                                                                                            \
    IT(KEY_APOSTROPHE)                                                                                                 \
    IT(KEY_COMMA)                                                                                                      \
    IT(KEY_MINUS)                                                                                                      \
    IT(KEY_PERIOD)                                                                                                     \
    IT(KEY_SLASH)                                                                                                      \
    IT(KEY_ZERO)                                                                                                       \
    IT(KEY_ONE)                                                                                                        \
    IT(KEY_TWO)                                                                                                        \
    IT(KEY_THREE)                                                                                                      \
    IT(KEY_FOUR)                                                                                                       \
    IT(KEY_FIVE)                                                                                                       \
    IT(KEY_SIX)                                                                                                        \
    IT(KEY_SEVEN)                                                                                                      \
    IT(KEY_EIGHT)                                                                                                      \
    IT(KEY_NINE)                                                                                                       \
    IT(KEY_SEMICOLON)                                                                                                  \
    IT(KEY_EQUAL)                                                                                                      \
    IT(KEY_A)                                                                                                          \
    IT(KEY_B)                                                                                                          \
    IT(KEY_C)                                                                                                          \
    IT(KEY_D)                                                                                                          \
    IT(KEY_E)                                                                                                          \
    IT(KEY_F)                                                                                                          \
    IT(KEY_G)                                                                                                          \
    IT(KEY_H)                                                                                                          \
    IT(KEY_I)                                                                                                          \
    IT(KEY_J)                                                                                                          \
    IT(KEY_K)                                                                                                          \
    IT(KEY_L)                                                                                                          \
    IT(KEY_M)                                                                                                          \
    IT(KEY_N)                                                                                                          \
    IT(KEY_O)                                                                                                          \
    IT(KEY_P)                                                                                                          \
    IT(KEY_Q)                                                                                                          \
    IT(KEY_R)                                                                                                          \
    IT(KEY_S)                                                                                                          \
    IT(KEY_T)                                                                                                          \
    IT(KEY_U)                                                                                                          \
    IT(KEY_V)                                                                                                          \
    IT(KEY_W)                                                                                                          \
    IT(KEY_X)                                                                                                          \
    IT(KEY_Y)                                                                                                          \
    IT(KEY_Z)                                                                                                          \
    IT(KEY_LEFT_BRACKET)                                                                                               \
    IT(KEY_BACKSLASH)                                                                                                  \
    IT(KEY_RIGHT_BRACKET)                                                                                              \
    IT(KEY_GRAVE)                                                                                                      \
    /* Function keys */                                                                                                \
    IT(KEY_SPACE)                                                                                                      \
    IT(KEY_ESCAPE)                                                                                                     \
    IT(KEY_ENTER)                                                                                                      \
    IT(KEY_TAB)                                                                                                        \
    IT(KEY_BACKSPACE)                                                                                                  \
    IT(KEY_INSERT)                                                                                                     \
    IT(KEY_DELETE)                                                                                                     \
    IT(KEY_RIGHT)                                                                                                      \
    IT(KEY_LEFT)                                                                                                       \
    IT(KEY_DOWN)                                                                                                       \
    IT(KEY_UP)                                                                                                         \
    IT(KEY_PAGE_UP)                                                                                                    \
    IT(KEY_PAGE_DOWN)                                                                                                  \
    IT(KEY_HOME)                                                                                                       \
    IT(KEY_END)                                                                                                        \
    IT(KEY_CAPS_LOCK)                                                                                                  \
    IT(KEY_SCROLL_LOCK)                                                                                                \
    IT(KEY_NUM_LOCK)                                                                                                   \
    IT(KEY_PRINT_SCREEN)                                                                                               \
    IT(KEY_PAUSE)                                                                                                      \
    IT(KEY_F1)                                                                                                         \
    IT(KEY_F2)                                                                                                         \
    IT(KEY_F3)                                                                                                         \
    IT(KEY_F4)                                                                                                         \
    IT(KEY_F5)                                                                                                         \
    IT(KEY_F6)                                                                                                         \
    IT(KEY_F7)                                                                                                         \
    IT(KEY_F8)                                                                                                         \
    IT(KEY_F9)                                                                                                         \
    IT(KEY_F10)                                                                                                        \
    IT(KEY_F11)                                                                                                        \
    IT(KEY_F12)                                                                                                        \
    IT(KEY_LEFT_SHIFT)                                                                                                 \
    IT(KEY_LEFT_CONTROL)                                                                                               \
    IT(KEY_LEFT_ALT)                                                                                                   \
    IT(KEY_LEFT_SUPER)                                                                                                 \
    IT(KEY_RIGHT_SHIFT)                                                                                                \
    IT(KEY_RIGHT_CONTROL)                                                                                              \
    IT(KEY_RIGHT_ALT)                                                                                                  \
    IT(KEY_RIGHT_SUPER)                                                                                                \
    IT(KEY_KB_MENU)                                                                                                    \
    /* Keypad keys */                                                                                                  \
    IT(KEY_KP_0)                                                                                                       \
    IT(KEY_KP_1)                                                                                                       \
    IT(KEY_KP_2)                                                                                                       \
    IT(KEY_KP_3)                                                                                                       \
    IT(KEY_KP_4)                                                                                                       \
    IT(KEY_KP_5)                                                                                                       \
    IT(KEY_KP_6)                                                                                                       \
    IT(KEY_KP_7)                                                                                                       \
    IT(KEY_KP_8)                                                                                                       \
    IT(KEY_KP_9)                                                                                                       \
    IT(KEY_KP_DECIMAL)                                                                                                 \
    IT(KEY_KP_DIVIDE)                                                                                                  \
    IT(KEY_KP_MULTIPLY)                                                                                                \
    IT(KEY_KP_SUBTRACT)                                                                                                \
    IT(KEY_KP_ADD)                                                                                                     \
    IT(KEY_KP_ENTER)                                                                                                   \
    IT(KEY_KP_EQUAL)

// IT(name)
#define MOUSE_BUTTON_DEF()                                                                                             \
    IT(MOUSE_BUTTON_LEFT)                                                                                              \
    IT(MOUSE_BUTTON_RIGHT)                                                                                             \
    IT(MOUSE_BUTTON_MIDDLE)                                                                                            \
    IT(MOUSE_BUTTON_SIDE)                                                                                              \
    IT(MOUSE_BUTTON_EXTRA)                                                                                             \
    IT(MOUSE_BUTTON_FORWARD)                                                                                           \
    IT(MOUSE_BUTTON_BACK)

static KeyboardKey GetKeyboardKey(std::string_view jsonValue)
{
#define IT(name)                                                                                                       \
    if (jsonValue == #name)                                                                                            \
    {                                                                                                                  \
        return name;                                                                                                   \
    }

    KEY_DEF()

#undef IT

    // TODO: exception? std::optional?
    return KEY_NULL;
}

static MouseButton GetMouseButton(std::string_view jsonValue)
{
#define IT(name)                                                                                                       \
    if (jsonValue == #name)                                                                                            \
    {                                                                                                                  \
        return name;                                                                                                   \
    }

    MOUSE_BUTTON_DEF()

#undef IT

    // TODO: exception? std::optional?
    return MOUSE_BUTTON_LEFT;
}

void Keybinds::Load(json::ondemand::parser &parser, const fs::path &path)
{
    // TODO
}
