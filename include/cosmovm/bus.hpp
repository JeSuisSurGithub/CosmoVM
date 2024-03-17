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

#ifndef BUS_HPP
#define BUS_HPP

#include <functional>
#include <memory>
#include <unordered_map>

#include "common.hpp"
#include "memory.hpp"

namespace cosmovm
{
    constexpr u16 PORT_DUMMY_VALUE = 0xFFFF;

    class bus
    {
        private:
            std::shared_ptr<memory>& m_memory;
            std::unordered_map<u16, std::function<u16(u16)>> m_port;

        public:
            bus() = delete;
            bus(std::shared_ptr<memory>& memory_ref);

            void bind_port(u16 port, const std::function<u16(u16)>& func_ptr);
            u16 device_in(u16 port);
            void device_out(u16 port, u16 data);

            u8 mem_read8(u16 addr);
            u16 mem_read16(u16 addr);
            void mem_write8(u16 addr, u8 data);
            void mem_write16(u16 addr, u16 data);
            const std::shared_ptr<memory>& get_memory() const;
    };
}


#endif /* BUS_HPP */