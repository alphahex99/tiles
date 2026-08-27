#ifndef _KEYBINDS_HPP
#define _KEYBINDS_HPP

#include "raylib.h"
#include <string>

#include "Jsonc.hpp"

class Keybinds
{
    template <typename T> struct Keybind
    {
        const std::string key;
        T value;
    };

    using KeyboardKey = Keybind<::KeyboardKey>;
    using MouseButton = Keybind<::MouseButton>;

#define KEY(name)                                                                                                      \
    KeyboardKey KEY_##name                                                                                             \
    {                                                                                                                  \
        "KEY_" #name,                                                                                                  \
        {                                                                                                              \
        }                                                                                                              \
    }

#define MOUSE_BUTTON(name)                                                                                             \
    MouseButton MOUSE_BUTTON_##name                                                                                    \
    {                                                                                                                  \
        "MOUSE_BUTTON_" #name,                                                                                         \
        {                                                                                                              \
        }                                                                                                              \
    }

  public:
    MOUSE_BUTTON(SELECTION_ADD);
    MOUSE_BUTTON(SELECTION_REMOVE);

    void Load(json::ondemand::parser &parser, const fs::path &path);
};

#endif
