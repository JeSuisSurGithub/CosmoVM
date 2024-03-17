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

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

#include <array>
#include <vector>

#include "common.hpp"

namespace cosmovm
{
    constexpr u32 MEM_SIZE = 0x10000;
    constexpr std::string DUMP_PATH = "mem_dump.bin";

    class memory
    {
        private:
            std::array<u8, MEM_SIZE> m_mem_buf;

        public:
            memory();
            memory(u16 addr, const std::vector<u8>& buf, u16 sz);
            ~memory();

            u8 read8(u16 addr);
            u16 read16(u16 addr);
            void write8(u16 addr, u8 data);
            void write16(u16 addr, u16 data);
            void load(u16 addr, const std::vector<u8>& buf, u16 sz);
            const std::array<u8, MEM_SIZE>& get_buf() const;

            void dump();
    };
}


#endif /* MEMORY_HPP */