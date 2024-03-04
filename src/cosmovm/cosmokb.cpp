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

#include <cosmovm/cosmokb.hpp>

using namespace cosmovm;

cosmokb::cosmokb(std::shared_ptr<cosmobus>& bus)
:
m_key_selector(0),
m_sdl_kb_state(SDL_GetKeyboardState(NULL)),
m_bus(bus)
{
    m_bus->bind_port(0x51, std::bind(&cosmokb::set_key_selector, this, std::placeholders::_1));
    m_bus->bind_port(0x52, std::bind(&cosmokb::get_requested_key, this, std::placeholders::_1));
}

cosmokb::~cosmokb()
{
}

u16i cosmokb::set_key_selector(u16i key_selector)
{
    m_key_selector = key_selector;
    return PORT_DUMMY_VALUE;
}

u16i cosmokb::get_requested_key(u16i)
{
    return m_sdl_kb_state[m_key_selector];
}