#ifndef COSMOCPU_HPP
#define COSMOCPU_HPP

#include "cosmocore.hpp"
#include "cosmobus.hpp"

#include <cstdint>
#include <cstring>

#include <functional>
#include <memory>
#include <unordered_map>

namespace cosmocore
{
    constexpr u16i START_ADDR = 0x0000;
    constexpr u16i REG_COUNT = 15;

    typedef struct _registers
    {
        u16i reserved;
        u16i az{0};
        u16i bz{0};
        u16i cz{0};
        u16i dz{0};
        u16i ez{0};
        u16i fz{0};
        u16i gz{0};
        u16i hz{0};
        u16i iz{0};
        u16i jz{0};
        u16i wz{0};
        u16i xz{0};
        u16i yz{0};
        u16i zz{0};
    }registers;

    typedef union _reg_table
    {
        registers regs{0};
        u16i table[REG_COUNT];
    }reg_table;

    typedef enum _FLAGS
    {
        ERROR = 1 << 0,
        EQUAL = 1 << 1,
        EXCEPTION = 1 << 2,
        GREATER = 1 << 3,
        //INTERRUPT = 1 << 4,
        LESSER = 1 << 5,
        RESET = 1 << 6,
        SHUTDOWN = 1 << 7
    }FLAGS;

    class cosmocpu
    {
        private: reg_table m_regs;
        private: u16i m_flags{0};
        private: std::shared_ptr<cosmobus>& m_bus;
        private: std::unordered_map<u16i, std::function<void(u32i)>> executers;

        public: cosmocpu() = delete;
        public: cosmocpu(std::shared_ptr<cosmobus>& bus);
        public: ~cosmocpu();

        public: void run();
        public: bool shutdown_flag_set();

        private: void reboot();

        private: u16i mem_read16(u16i addr);
        private: void mem_write16(u16i addr, u16i data);
        private: u8i mem_read8(u16i addr);
        private: void mem_write8(u16i addr, u8i data);
        private: u16i mem_pop();
        private: void mem_push(u16i data);

        private: void set_cmp_flags(u16i operand1, u16i operand2);
    };
}

#endif /* COSMOCPU_HPP */