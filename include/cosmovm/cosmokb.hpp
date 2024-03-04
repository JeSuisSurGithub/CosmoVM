/**
 * CosmoVM an emulator and assembler for an imaginary cpu
 * Copyright (C) 2022 JeFaisDesSpaghettis
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

#ifndef COSMOKB_HPP
#define COSMOKB_HPP

#include "cosmovm.hpp"
#include "cosmobus.hpp"

#include <SDL2/SDL_keyboard.h>

#include <unordered_map>

namespace cosmovm
{
    // Lazy implementation using directly SDL Scancodes
    class cosmokb
    {
        private: u16i m_key_selector;
        private: const u8i* m_sdl_kb_state;
        private: std::shared_ptr<cosmobus>& m_bus;

        public: cosmokb() = delete;
        public: cosmokb(std::shared_ptr<cosmobus>& bus);
        public: ~cosmokb();

        public: u16i set_key_selector(u16i key_selector);
        public: u16i get_requested_key(u16i dummy);
    };
}

#endif /* COSMOKB_HPP */