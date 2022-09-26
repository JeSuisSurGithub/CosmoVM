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

#ifndef COSMOMEM_HPP
#define COSMOMEM_HPP

#include "cosmocore.hpp"

#include <cstdint>

#include <vector>

namespace cosmocore
{
    constexpr u16i mem_size = UINT16_MAX;
    class cosmomem
    {
        private: std::vector<u8i> m_mem_buf;

        public: cosmomem();
        public: cosmomem(u16i addr, const std::vector<u8i>& buf, u16i sz);
        public: ~cosmomem();

        public: u8i read8(u16i addr);
        public: u16i read16(u16i addr);
        public: void write8(u16i addr, u8i data);
        public: void write16(u16i addr, u16i data);
        public: void load(u16i addr, const std::vector<u8i>& buf, u16i sz);
        public: const std::vector<u8i>& get_buf() const;
    };
}


#endif /* COSMOMEM_HPP */