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

#include <cosmocore/cosmomem.hpp>

using namespace cosmocore;

cosmomem::cosmomem()
{
    m_mem_buf.resize(mem_size);
}

cosmomem::cosmomem(u16i addr, const std::vector<u8i>& buf, u16i sz)
{
    m_mem_buf.resize(mem_size);
    load(addr, buf, sz);
}

cosmomem::~cosmomem() {}

u8i cosmomem::read8(u16i addr)
{
    return m_mem_buf.at(addr);
}

u16i cosmomem::read16(u16i addr)
{
    return static_cast<u16i>(m_mem_buf.at(addr + 1)) << 8 | m_mem_buf.at(addr);
}

void cosmomem::write8(u16i addr, u8i data)
{
    m_mem_buf.at(addr) = data;
}
void cosmomem::write16(u16i addr, u16i data)
{
    m_mem_buf.at(addr) = data & 0xFF;
    m_mem_buf.at(addr + 1) = data >> 8;
}

void cosmomem::load(u16i offset, const std::vector<u8i>& buf, u16i sz)
{
    m_mem_buf.insert(m_mem_buf.begin() + offset, buf.begin(), buf.begin() + sz);
}

const std::vector<u8i>& cosmomem::get_buf() const
{
    return m_mem_buf;
}