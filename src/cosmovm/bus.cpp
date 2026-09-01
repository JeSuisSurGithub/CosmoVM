#include <cosmovm/bus.hpp>

#include <format>

using namespace cosmovm;

bus::bus(std::shared_ptr<memory>& memory_ref)
:
m_memory(memory_ref),
m_port()
{
}

void bus::bind_port(u16 port, const std::function<u16(u16)>& func_ptr)
{
    if (m_port.find(port) != m_port.end())
        throw std::invalid_argument(std::format("[BUS] Port {} already exists", port));
    m_port[port] = func_ptr;
}

u16 bus::device_in(u16 port)
{
    if (m_port.find(port) == m_port.end())
        throw std::invalid_argument(std::format("[BUS] Port {} isn't bound", port));
    // Dummy value
    return m_port.at(port)(PORT_DUMMY_VALUE);
}

void bus::device_out(u16 port, u16 data)
{
    if (m_port.find(port) == m_port.end())
        throw std::invalid_argument(std::format("[BUS] Port {} isn't bound", port));
    // Return value discarded
     m_port.at(port)(data);
}

u8 bus::mem_read8(u16 addr)
{
    return m_memory->read8(addr);
}

u16 bus::mem_read16(u16 addr)
{
    return m_memory->read16(addr);
}

void bus::mem_write8(u16 addr, u8 data)
{
    m_memory->write8(addr, data);
}

void bus::mem_write16(u16 addr, u16 data)
{
    m_memory->write16(addr, data);
}

const std::shared_ptr<memory>& bus::get_memory() const
{
    return m_memory;
}