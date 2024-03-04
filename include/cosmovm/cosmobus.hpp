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

#ifndef COSMOBUS_HPP
#define COSMOBUS_HPP

#include "cosmovm.hpp"
#include "cosmomem.hpp"

#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace cosmovm
{
    constexpr u16i PORT_DUMMY_VALUE = 0xFFFF;
    class cosmobus
    {
        private: std::shared_ptr<cosmomem>& m_memory;
        private: std::unordered_map<u16i, std::function<u16i(u16i)>> m_port;

        public: cosmobus() = delete;
        public: cosmobus(std::shared_ptr<cosmomem>& memory_ref);

        public: void bind_port(u16i port, const std::function<u16i(u16i)>& func_ptr);
        public: u16i device_in(u16i port);
        public: void device_out(u16i port, u16i data);

        public: u8i mem_read8(u16i addr);
        public: u16i mem_read16(u16i addr);
        public: void mem_write8(u16i addr, u8i data);
        public: void mem_write16(u16i addr, u16i data);
        public: const std::shared_ptr<cosmomem>& get_memory() const;
    };
}


#endif /* COSMOBUS_HPP */