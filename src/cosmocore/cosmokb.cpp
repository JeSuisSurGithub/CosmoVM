#include <cosmocore/cosmokb.hpp>

using namespace cosmocore;

cosmokb::cosmokb(std::shared_ptr<cosmobus>& bus)
:
m_key_selector(0),
m_sdl_kb_state(SDL_GetKeyboardState(NULL)),
m_bus(bus)
{
    m_bus->bind_port(0x51, std::bind(&cosmokb::set_key_selector, this, std::placeholders::_1));
    m_bus->bind_port(0x52, std::bind(&cosmokb::get_requested_key, this, std::placeholders::_1));
}

cosmokb::~cosmokb()
{
}

u16i cosmokb::set_key_selector(u16i key_selector)
{
    m_key_selector = key_selector;
    return PORT_DUMMY_VALUE;
}

u16i cosmokb::get_requested_key(u16i dummy)
{
    return m_sdl_kb_state[m_key_selector];
}