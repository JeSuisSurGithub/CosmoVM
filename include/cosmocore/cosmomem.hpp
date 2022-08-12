#ifndef COSMOMEM_HPP
#define COSMOMEM_HPP

#include "cosmocore.hpp"

#include <cstdint>

#include <vector>

namespace cosmocore
{
    constexpr u16i mem_size = UINT16_MAX;
    class cosmomem
    {
        private: std::vector<u8i> m_mem_buf;

        public: cosmomem();
        public: cosmomem(u16i addr, const std::vector<u8i>& buf, u16i sz);
        public: ~cosmomem();

        public: u8i read8(u16i addr);
        public: u16i read16(u16i addr);
        public: void write8(u16i addr, u8i data);
        public: void write16(u16i addr, u16i data);
        public: void load(u16i addr, const std::vector<u8i>& buf, u16i sz);
        public: const std::vector<u8i>& get_buf() const;
    };
}


#endif /* COSMOMEM_HPP */