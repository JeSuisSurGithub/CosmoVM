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

#include <cosmovm/cosmoclk.hpp>

using namespace cosmovm;

cosmoclk::cosmoclk(std::shared_ptr<cosmobus>& bus)
:
m_bus(bus)
{
    m_bus->bind_port(0x31, std::bind(&cosmoclk::get_year, this, std::placeholders::_1));
    m_bus->bind_port(0x32, std::bind(&cosmoclk::get_day, this, std::placeholders::_1));
    m_bus->bind_port(0x33, std::bind(&cosmoclk::get_hour, this, std::placeholders::_1));
    m_bus->bind_port(0x34, std::bind(&cosmoclk::get_hour, this, std::placeholders::_1));
    m_bus->bind_port(0x35, std::bind(&cosmoclk::get_min, this, std::placeholders::_1));
    m_bus->bind_port(0x36, std::bind(&cosmoclk::get_seconds, this, std::placeholders::_1));
}

cosmoclk::~cosmoclk()
{
}

u16i cosmoclk::get_year(u16i)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_year + 1900;
}

u16i cosmoclk::get_month(u16i)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_mon + 1;
}

u16i cosmoclk::get_day(u16i)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_mday;
}

u16i cosmoclk::get_hour(u16i)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_hour;
}

u16i cosmoclk::get_min(u16i)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_min;
}

u16i cosmoclk::get_seconds(u16i)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_sec;
}
