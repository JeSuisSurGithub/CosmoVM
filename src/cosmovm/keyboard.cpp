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

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <cosmovm/keyboard.hpp>

using namespace cosmovm;

keyboard::keyboard(std::shared_ptr<bus>& bus)
:
m_key_selector(0),
m_sdl_kb_state(SDL_GetKeyboardState(NULL)),
m_bus(bus)
{
    m_bus->bind_port(0x51, std::bind(&keyboard::set_key_selector, this, std::placeholders::_1));
    m_bus->bind_port(0x52, std::bind(&keyboard::get_requested_key, this, std::placeholders::_1));
    m_bus->bind_port(0x53, std::bind(&keyboard::get_pressed_key, this, std::placeholders::_1));
}

keyboard::~keyboard()
{
}

u16 keyboard::set_key_selector(u16 key_selector)
{
    m_key_selector = key_selector;
    return PORT_DUMMY_VALUE;
}

u16 keyboard::get_requested_key(u16)
{
    return m_sdl_kb_state[m_key_selector];
}

u16 keyboard::get_pressed_key(u16)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            return event.key.keysym.scancode;
        }
    }
    return SDL_SCANCODE_UNKNOWN;
}