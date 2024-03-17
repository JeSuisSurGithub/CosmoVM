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

#ifndef CLOCK_HPP
#define CLOCK_HPP

#include "common.hpp"
#include "bus.hpp"

namespace cosmovm
{
    class clock
    {
        private:
            std::shared_ptr<bus>& m_bus;

        public:
            clock() = delete;
            clock(std::shared_ptr<bus>& bus);
            ~clock();

            u16 get_year(u16 dummy);
            u16 get_month(u16 dummy);
            u16 get_day(u16 dummy);
            u16 get_hour(u16 dummy);
            u16 get_min(u16 dummy);
            u16 get_seconds(u16 dummy);
    };
}

#endif /* CLOCK_HPP */