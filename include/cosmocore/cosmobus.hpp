#ifndef COSMOBUS_HPP
#define COSMOBUS_HPP

#include "cosmocore.hpp"
#include "cosmomem.hpp"

#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace cosmocore
{
    constexpr u16i PORT_DUMMY_VALUE = 0xFFFF;
    class cosmobus
    {
        private: std::shared_ptr<cosmomem>& m_memory;
        private: std::unordered_map<u16i, std::function<u16i(u16i)>> m_port;

        public: cosmobus() = delete;
        public: cosmobus(std::shared_ptr<cosmomem>& memory_ref);

        public: void bind_port(u16i port, const std::function<u16i(u16i)>& func_ptr);
        public: u16i device_in(u16i port);
        public: void device_out(u16i port, u16i data);

        public: u8i mem_read8(u16i addr);
        public: u16i mem_read16(u16i addr);
        public: void mem_write8(u16i addr, u8i data);
        public: void mem_write16(u16i addr, u16i data);
        public: const std::shared_ptr<cosmomem>& get_memory() const;
    };
}


#endif /* COSMOBUS_HPP */