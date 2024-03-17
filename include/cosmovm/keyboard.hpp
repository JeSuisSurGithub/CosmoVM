/**
 * CosmoVM an emulator and assembler for an imaginary cpu
 * Copyright (C) 2022 JeSuis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <SDL2/SDL_keyboard.h>

#include "common.hpp"
#include "bus.hpp"

namespace cosmovm
{
    // Lazy implementation using directly SDL Scancodes
    class keyboard
    {
        private:
            u16 m_key_selector;
            const u8* m_sdl_kb_state;
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