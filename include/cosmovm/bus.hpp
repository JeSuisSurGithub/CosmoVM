#ifndef BUS_HPP
#define BUS_HPP

#include <format>
#include <functional>
#include <memory>
#include <unordered_map>

#include "common.hpp"
#include "memory.hpp"

namespace cosmovm
{
    constexpr u16 PORT_DUMMY_VALUE = 0xFFFF;

    class bus
    {
        private:
            std::shared_ptr<memory>& m_memory;
            std::unordered_map<u16, std::function<u16(u16)>> m_port;

        public:
            bus() = delete;
            bus(std::shared_ptr<memory>& memory_ref);

            void bind_port(u16 port, const std::function<u16(u16)>& func_ptr);
            u16 device_in(u16 port);
            void device_out(u16 port, u16 data);

            u8 mem_read8(u16 addr);
            u16 mem_read16(u16 addr);
            void mem_write8(u16 addr, u8 data);
            void mem_write16(u16 addr, u16 data);
            const std::shared_ptr<memory>& get_memory() const;
    };
}


#endif /* BUS_HPP */