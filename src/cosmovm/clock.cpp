#include <cosmovm/clock.hpp>

using namespace cosmovm;

clock::clock(std::shared_ptr<bus>& bus)
:
m_bus(bus)
{
    m_bus->bind_port(0x31, std::bind(&clock::get_year, this, std::placeholders::_1));
    m_bus->bind_port(0x32, std::bind(&clock::get_month, this, std::placeholders::_1));
    m_bus->bind_port(0x33, std::bind(&clock::get_day, this, std::placeholders::_1));
    m_bus->bind_port(0x34, std::bind(&clock::get_hour, this, std::placeholders::_1));
    m_bus->bind_port(0x35, std::bind(&clock::get_min, this, std::placeholders::_1));
    m_bus->bind_port(0x36, std::bind(&clock::get_seconds, this, std::placeholders::_1));
}

clock::~clock()
{
}

u16 clock::get_year(u16)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_year + 1900;
}

u16 clock::get_month(u16)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_mon + 1;
}

u16 clock::get_day(u16)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_mday;
}

u16 clock::get_hour(u16)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_hour;
}

u16 clock::get_min(u16)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_min;
}

u16 clock::get_seconds(u16)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_sec;
}
