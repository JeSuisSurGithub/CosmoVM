#include <cosmocore/cosmoclk.hpp>

using namespace cosmocore;

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

u16i cosmoclk::get_year(u16i dummy)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_year + 1900;
}

u16i cosmoclk::get_month(u16i dummy)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_mon + 1;
}

u16i cosmoclk::get_day(u16i dummy)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_mday;
}

u16i cosmoclk::get_hour(u16i dummy)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_hour;
}

u16i cosmoclk::get_min(u16i dummy)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_min;
}

u16i cosmoclk::get_seconds(u16i dummy)
{
    time_t tt = time(NULL);
    struct tm* utc_tm = gmtime(&tt);
    return utc_tm->tm_sec;
}
