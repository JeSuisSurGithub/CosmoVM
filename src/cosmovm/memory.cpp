#include <fstream>

#include <cosmovm/memory.hpp>

using namespace cosmovm;

memory::memory()
:
m_mem_buf()
{
}

memory::memory(u16 addr, const std::vector<u8>& buf, u16 sz)
:
m_mem_buf()
{
    load(addr, buf, sz);
}

memory::~memory() {}

u8 memory::read8(u16 addr)
{
    return m_mem_buf.at(addr);
}

u16 memory::read16(u16 addr)
{
    return static_cast<u16>(m_mem_buf.at(addr + 1)) << 8 | m_mem_buf.at(addr);
}

void memory::write8(u16 addr, u8 data)
{
    m_mem_buf.at(addr) = data;
}
void memory::write16(u16 addr, u16 data)
{
    m_mem_buf.at(addr) = data & 0xFF;
    m_mem_buf.at(addr + 1) = data >> 8;
}

void memory::load(u16 offset, const std::vector<u8>& buf, u16 sz)
{
    std::copy(buf.begin(), buf.begin() + sz, m_mem_buf.begin() + offset);
}

const std::array<u8, MEM_SIZE>& memory::get_buf() const
{
    return m_mem_buf;
}

void memory::dump()
{
    std::ofstream dump_file{DUMP_PATH, std::ios::binary | std::ios::out};
    dump_file.write(reinterpret_cast<const char*>(m_mem_buf.data()), MEM_SIZE);
}