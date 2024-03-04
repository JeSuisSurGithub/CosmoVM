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

#include <cosmovm/cosmobus.hpp>

using namespace cosmovm;

cosmobus::cosmobus(std::shared_ptr<cosmomem>& memory_ref)
:
m_memory(memory_ref),
m_port()
{
}

void cosmobus::bind_port(u16i port, const std::function<u16i(u16i)>& func_ptr)
{
    if (m_port.find(port) != m_port.end())
        throw std::invalid_argument("[COSMOBUS] Port " + std::to_string(port) + " already exists");
    m_port[port] = func_ptr;
}

u16i cosmobus::device_in(u16i port)
{
    if (m_port.find(port) == m_port.end())
        throw std::invalid_argument("[COSMOBUS] Port " + std::to_string(port) + " isn't bound");
    // Dummy value
    return m_port[port](PORT_DUMMY_VALUE);
}

void cosmobus::device_out(u16i port, u16i data)
{
    if (m_port.find(port) == m_port.end())
        throw std::invalid_argument("[COSMOBUS] Port " + std::to_string(port) + " isn't bound");
    // Return value discarded
    m_port[port](data);
}

u8i cosmobus::mem_read8(u16i addr)
{
    return m_memory->read8(addr);
}

u16i cosmobus::mem_read16(u16i addr)
{
    return m_memory->read16(addr);
}

void cosmobus::mem_write8(u16i addr, u8i data)
{
    m_memory->write8(addr, data);
}

void cosmobus::mem_write16(u16i addr, u16i data)
{
    m_memory->write16(addr, data);
}

const std::shared_ptr<cosmomem>& cosmobus::get_memory() const
{
    return m_memory;
}