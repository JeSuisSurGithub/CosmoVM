/**
 * CosmoVM an emulator and assembler for an imaginary cpu
 * Copyright (C) 2022 JeSuis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CPU_HPP
#define CPU_HPP

#include <functional>
#include <memory>
#include <unordered_map>

#include "common.hpp"
#include "bus.hpp"

namespace cosmovm
{
    constexpr u16 START_ADDR = 0x0000;
    constexpr u16 REG_COUNT = 13;

    typedef struct registers
    {
        u16 null;
        u16 az{0};
        u16 bz{0};
        u16 cz{0};
        u16 dz{0};
        u16 ez{0};
        u16 fz{0};
        u16 gz{0};
        u16 hz{0};
        u16 sp{0};
        u16 sb{0};
        u16 xa{0};
        u16 mo{0};
    }registers;

    typedef union reg_table
    {
        registers regs{0};
        u16 table[REG_COUNT];
    }reg_table;

    typedef enum FLAGS: u8
    {
        ERROR = 1 << 0,
        EXCEPTION = 1 << 1,
        EQUAL = 1 << 2,
        GREATER = 1 << 3,
        LESSER = 1 << 4,
        RESET = 1 << 5,
        SHUTDOWN = 1 << 6
    }FLAGS;

    typedef enum INSTRUCTION: u8{
        WCYL  = 0x00,

        ADD   = 0x01,
        ADDI  = 0x02,
        SUB   = 0x03,
        SUBI  = 0x04,
        MUL   = 0x05,
        MULI  = 0x06,
        DIV   = 0x07,
        DIVI  = 0x08,
        INC   = 0x09,
        DEC   = 0x0A,
        NEG   = 0x0B,

        CMP   = 0x0C,
        CMPI  = 0x0D,

        AND   = 0x10,
        ANDI  = 0x11,
        OR    = 0x12,
        ORI   = 0x13,
        XOR   = 0x14,
        XORI  = 0x15,
        SHL   = 0x16,
        SHLI  = 0x17,
        SHR   = 0x18,
        SHRI  = 0x19,
        NOT   = 0x1A,

        MOV   = 0x20,
        MOVI  = 0x21,
        LOAD  = 0x22,
        LOADI = 0x23,
        STOR  = 0x24,
        STORI = 0x25,
        COPY  = 0x26,
        COPYI = 0x27,

        LOADB = 0x28,
        LOADBI= 0x29,
        STORB = 0x2A,
        STORBI= 0x2B,
        COPYB = 0x2C,
        COPYBI= 0x2D,

        PUSH  = 0x30,
        PUSHI = 0x31,
        POP   = 0x32,
        PUSHF = 0x33,
        POPF  = 0x34,

        CALL  = 0x35,
        RET   = 0x36,

        JMP   = 0x40,
        JE    = 0x41,
        JNE   = 0x42,
        JG    = 0x43,
        JGE   = 0x44,
        JL    = 0x45,
        JLE   = 0x46,

        JER   = 0x47,
        JNER  = 0x48,
        JXP   = 0x49,
        JNXP  = 0x4A,

        LOP   = 0x4B,
        LOPE  = 0x4C,
        LOPNE = 0x4D,

        IN    = 0x51,
        OUT   = 0x52,

        CLER  = 0x53,
        CLXP  = 0x54,
        STER  = 0x55,
        STXP  = 0x56,
        STRS  = 0x57,
        STSD  = 0x58,
    }INSTRUCTION;

    class cpu
    {
        private:
            reg_table m_regs;
            u16 m_flags{0};
            std::shared_ptr<bus>& m_bus;
            std::unordered_map<INSTRUCTION, std::function<void(u32)>> executers;

        public:
            cpu() = delete;
            cpu(std::shared_ptr<bus>& bus);
            ~cpu();

            void run();
            bool shutdown_flag_set();
            bool exception_flag_set();

        private:
            void reboot();

            u16 mem_read16(u16 addr);
            void mem_write16(u16 addr, u16 data);
            u8 mem_read8(u16 addr);
            void mem_write8(u16 addr, u8 data);
            u16 mem_pop();
            void mem_push(u16 data);

            void set_cmp_flags(u16 operand1, u16 operand2);
    };
}

#endif /* CPU_HPP */