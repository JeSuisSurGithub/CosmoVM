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

#include <cosmovm/disk.hpp>

using namespace cosmovm;

disk::disk(std::shared_ptr<bus>& bus, const std::string& disk_path)
:
m_file_size(),
m_mode(DISK_MODES::READ),
m_lba(0),
m_buf(),
m_buf_index(0),
m_file(),
m_bus(bus)
{
    m_file.open(disk_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!m_file.is_open())
        throw std::invalid_argument(std::format("[DISK] Couldn't access {}", disk_path));

    m_file.seekg(0, std::ios::end);
    m_file_size = m_file.tellg();
    m_file.seekg(0, std::ios::beg);

    if (m_file_size % SECTOR_SIZE)
        throw std::invalid_argument("[DISK] Incomplete sectors");

    m_bus->bind_port(0x61, std::bind(&disk::set_mode, this, std::placeholders::_1));
    m_bus->bind_port(0x62, std::bind(&disk::set_lba, this, std::placeholders::_1));
    m_bus->bind_port(0x63, std::bind(&disk::do_it, this, std::placeholders::_1));
    m_bus->bind_port(0x64, std::bind(&disk::request_read_write, this, std::placeholders::_1));
    m_bus->bind_port(0x65, std::bind(&disk::get_sectors_count, this, std::placeholders::_1));
    m_bus->bind_port(0x66, std::bind(&disk::end, this, std::placeholders::_1));
}

disk::~disk()
{
    m_file.close();
}

u16 disk::set_mode(u16 mode)
{
    m_buf_index = 0;
    std::memset(m_buf.data(), 0, SECTOR_SIZE);
    if (mode == DISK_MODES::READ)
        m_mode = static_cast<DISK_MODES>(mode);
    else if (mode == DISK_MODES::WRITE)
        m_mode = static_cast<DISK_MODES>(mode);
    else throw std::invalid_argument(std::format("[DISK] Unknown mode {}", mode));

    // Return dummy
    return PORT_DUMMY_VALUE;
}

u16 disk::set_lba(u16 lba)
{
    m_buf_index = 0;
    std::memset(m_buf.data(), 0, SECTOR_SIZE);
    m_lba = lba;
    if (m_mode == DISK_MODES::READ)
        m_file.seekg(m_lba * SECTOR_SIZE);
    else if (m_mode == DISK_MODES::WRITE)
        m_file.seekp(m_lba * SECTOR_SIZE);
    // Return dummy
    return PORT_DUMMY_VALUE;
}

u16 disk::do_it(u16)
{
    if (m_mode == DISK_MODES::READ)
        m_file.read(reinterpret_cast<char*>(m_buf.data()), SECTOR_SIZE);
    else if (m_mode == DISK_MODES::WRITE)
        m_file.write(reinterpret_cast<const char*>(m_buf.data()), SECTOR_SIZE);
    // Return dummy
    return PORT_DUMMY_VALUE;
}

u16 disk::request_read_write(u16 data_in)
{
    u16 data = 0;
    if (m_buf_index != SECTOR_SIZE)
    {
        if (m_mode == DISK_MODES::READ)
        {
            data = m_buf.at(m_buf_index++);
        }
        else if (m_mode == DISK_MODES::WRITE)
        {
            m_buf[m_buf_index++] = data_in;
            data = PORT_DUMMY_VALUE;
        }
    }
    return data;
}

u16 disk::get_sectors_count(u16)
{
    return m_file_size / SECTOR_SIZE;
}

u16 disk::end(u16)
{
    return m_buf_index == SECTOR_SIZE;
}