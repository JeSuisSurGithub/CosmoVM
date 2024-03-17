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

#ifndef DISK_HPP
#define DISK_HPP

#include <cstring>

#include <fstream>

#include "common.hpp"
#include "bus.hpp"

namespace cosmovm
{
    constexpr usz SECTOR_SIZE = 0x200;

    typedef enum DISK_MODES
    {
        READ = 0,
        WRITE = 1,
    }DISK_MODES;

    class disk
    {
        private:
            std::size_t m_file_size;
            DISK_MODES m_mode;
            u16 m_lba;
            std::array<u8, SECTOR_SIZE> m_buf;
            u16 m_buf_index;

            std::fstream m_file;
            std::shared_ptr<bus>& m_bus;

        public:
            disk() = delete;
            disk(std::shared_ptr<bus>& bus, const std::string& disk_path);
            ~disk();

            u16 set_mode(u16 mode);
            u16 set_lba(u16 lba);
            u16 do_it(u16 dummy);

            u16 request_read_write(u16 data_in);
            u16 get_sectors_count(u16 dummy);
            u16 end(u16 dummy);
    };
}

#endif /* DISK_HPP */