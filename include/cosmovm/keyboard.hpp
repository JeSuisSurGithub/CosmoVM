#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <SDL3/SDL_keyboard.h>

#include "common.hpp"
#include "bus.hpp"

namespace cosmovm
{
    // Lazy implementation using directly SDL Scancodes
    class keyboard
    {
        private:
            u16 m_key_selector;
            const bool* m_sdl_kb_state;
            std::shared_ptr<bus>& m_bus;

        public:
            keyboard() = delete;
            keyboard(const keyboard&) = delete;
            keyboard(std::shared_ptr<bus>& bus);
            ~keyboard();

            u16 set_key_selector(u16 key_selector);
            u16 get_requested_key(u16 dummy);
            u16 get_pressed_key(u16 dummy);
    };
}

#endif /* KEYBOARD_HPP */