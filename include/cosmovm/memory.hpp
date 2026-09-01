#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <string>
#include <vector>

#include "common.hpp"

namespace cosmovm
{
    constexpr u32 MEM_SIZE = 0x10000;
    constexpr std::string DUMP_PATH = "mem_dump.bin";

    class memory
    {
        private:
            std::array<u8, MEM_SIZE> m_mem_buf;

        public:
            memory();
            memory(u16 addr, const std::vector<u8>& buf, u16 sz);
            ~memory();

            u8 read8(u16 addr);
            u16 read16(u16 addr);
            void write8(u16 addr, u8 data);
            void write16(u16 addr, u16 data);
            void load(u16 addr, const std::vector<u8>& buf, u16 sz);
            const std::array<u8, MEM_SIZE>& get_buf() const;

            void dump();
    };
}


#endif /* MEMORY_HPP */