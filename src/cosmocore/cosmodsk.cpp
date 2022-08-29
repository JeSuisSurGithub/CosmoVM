#include <cosmocore/cosmodsk.hpp>

using namespace cosmocore;

cosmodsk::cosmodsk(std::shared_ptr<cosmobus>& bus)
:
m_lba(0),
m_cache_index(0),
m_mode(DISK_MODES::READ),
m_bus(bus)
{
    m_cache.resize(SECTOR_SIZE);
    m_file.open(DISK_PATH, std::ios::in | std::ios::out | std::ios::binary);
    if (!m_file.is_open())
        throw std::invalid_argument("[COSMODSK] Couldn't access " + DISK_PATH);
    m_file.seekg(0, std::ios::end);
    m_file_size = m_file.tellg();
    m_file.seekg(0, std::ios::beg);
    if (m_file_size % SECTOR_SIZE)
        throw std::invalid_argument("[COSMODSK] Incomplete sectors");

    m_bus->bind_port(0x61, std::bind(&cosmodsk::set_mode, this, std::placeholders::_1));
    m_bus->bind_port(0x62, std::bind(&cosmodsk::set_lba, this, std::placeholders::_1));
    m_bus->bind_port(0x63, std::bind(&cosmodsk::do_it, this, std::placeholders::_1));
    m_bus->bind_port(0x64, std::bind(&cosmodsk::request_read_write, this, std::placeholders::_1));
    m_bus->bind_port(0x65, std::bind(&cosmodsk::get_sectors_count, this, std::placeholders::_1));
}

cosmodsk::~cosmodsk()
{
    m_file.close();
}

u16i cosmodsk::set_mode(u16i mode)
{
    m_cache_index = 0;
    std::memset(m_cache.data(), 0, SECTOR_SIZE);
    if (mode == DISK_MODES::READ)
        m_mode = static_cast<DISK_MODES>(mode);
    else if (mode == DISK_MODES::WRITE)
        m_mode = static_cast<DISK_MODES>(mode);
    else throw std::invalid_argument("[COSMODSK] Unknown mode " + std::to_string(mode));

    // Return dummy
    return PORT_DUMMY_VALUE;
}

u16i cosmodsk::set_lba(u16i lba)
{
    m_cache_index = 0;
    std::memset(m_cache.data(), 0, SECTOR_SIZE);
    m_lba = lba;
    if (m_mode == DISK_MODES::READ)
        m_file.seekg(m_lba * SECTOR_SIZE);
    else if (m_mode == DISK_MODES::WRITE)
        m_file.seekp(m_lba * SECTOR_SIZE);
    // Return dummy
    return PORT_DUMMY_VALUE;
}

u16i cosmodsk::do_it(u16i dummy)
{
    if (m_mode == DISK_MODES::READ)
        m_file.read(reinterpret_cast<char*>(m_cache.data()), SECTOR_SIZE);
    else if (m_mode == DISK_MODES::WRITE)
        m_file.write(reinterpret_cast<const char*>(m_cache.data()), SECTOR_SIZE);
    // Return dummy
    return PORT_DUMMY_VALUE;
}

u16i cosmodsk::request_read_write(u16i data_in)
{
    u16i data{0};
    if (m_mode == DISK_MODES::READ)
    {
        data = m_cache.at(m_cache_index);
        ++m_cache_index;
    }
    else if (m_mode == DISK_MODES::WRITE)
    {
        m_cache.at(m_cache_index) = data_in;
        ++m_cache_index;
        data = PORT_DUMMY_VALUE;
    }
    return data;
}

u16i cosmodsk::get_sectors_count(u16i dummy)
{
    return m_file_size / SECTOR_SIZE;
}