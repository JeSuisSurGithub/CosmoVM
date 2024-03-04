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

#ifndef COSMOCLK_HPP
#define COSMOCLK_HPP

#include "cosmovm.hpp"
#include "cosmobus.hpp"

#include <ctime>

namespace cosmovm
{
    class cosmoclk
    {
        private: std::shared_ptr<cosmobus>& m_bus;

        public: cosmoclk() = delete;
        public: cosmoclk(std::shared_ptr<cosmobus>& bus);
        public: ~cosmoclk();

        public: u16i get_year(u16i dummy);
        public: u16i get_month(u16i dummy);
        public: u16i get_day(u16i dummy);
        public: u16i get_hour(u16i dummy);
        public: u16i get_min(u16i dummy);
        public: u16i get_seconds(u16i dummy);
    };
}

#endif /* COSMOCLK_HPP */