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

#include <cstring>
#include <iostream>
#include <stdexcept>

#include <cosmovm/cpu.hpp>

using namespace cosmovm;

cpu::cpu(std::shared_ptr<bus>& bus)
: m_regs(), m_bus(bus)
{
    m_regs.regs.xa = START_ADDR;
    // ALL INSTRUCTION START
    executers =
    {
        {WCYL, [](u32) -> void {}},

        {ADD, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16 result = operand1 + operand2;
            dest_reg = result;
        }},
        {ADDI, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            u16 result = operand1 + operand2;
            dest_reg = result;
        }},
        {SUB, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16 result = operand1 - operand2;
            dest_reg = result;
        }},
        {SUBI, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            u16 result = operand1 - operand2;
            dest_reg = result;
        }},
        {MUL, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16 result = operand1 * operand2;
            dest_reg = result;
        }},
        {MULI, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            u16 result = operand1 * operand2;
            dest_reg = result;
        }},
        {DIV, [this](u32 instruction) -> void
        {
            u16 operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16 quotient = operand1 / operand2;
            u16 remainder = operand1 % operand2;
            m_regs.regs.gz = quotient;
            m_regs.regs.hz = remainder;
        }},
        {DIVI, [this](u32 instruction) -> void
        {
            u16 operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            u16 quotient = operand1 / operand2;
            u16 remainder = operand1 % operand2;
            m_regs.regs.gz = quotient;
            m_regs.regs.hz = remainder;
        }},
        {INC, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF]++;
        }},
        {DEC, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF]--;
        }},
        {NEG, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = (~m_regs.table[(instruction >> 8) & 0xFFFF] + 1);
        }},

        {CMP, [this](u32 instruction) -> void
        {
            u16 operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            set_cmp_flags(operand1, operand2);
        }},
        {CMPI, [this](u32 instruction) -> void
        {
            u16 operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            set_cmp_flags(operand1, operand2);
        }},
        {AND, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16 result = operand1 & operand2;
            dest_reg = result;
        }},
        {ANDI, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            u16 result = operand1 & operand2;
            dest_reg = result;
        }},
        {OR, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16 result = operand1 | operand2;
            dest_reg = result;
        }},
        {ORI, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            u16 result = operand1 | operand2;
            dest_reg = result;
        }},
        {XOR, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16 result = operand1 ^ operand2;
            dest_reg = result;
        }},
        {XORI, [this](u32 instruction) -> void
        {
            u16& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16 operand1 = dest_reg;
            u16 operand2 = (instruction >> 16) & 0xFFFF;
            u16 result = operand1 ^ operand2;
            dest_reg = result;
        }},
        {SHL, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] <<= m_regs.table[(instruction >> 16) & 0xFF];
        }},
        {SHLI, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] <<= (instruction >> 16) & 0xFFFF;
        }},
        {SHR, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] >>= m_regs.table[(instruction >> 16) & 0xFF];
        }},
        {SHRI, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] >>= (instruction >> 16) & 0xFFFF;
        }},
        {NOT, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = ~m_regs.table[(instruction >> 8) & 0xFFFF];
        }},

        {MOV, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = m_regs.table[(instruction >> 16) & 0xFF];
        }},
        {MOVI, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = (instruction >> 16) & 0xFFFF;
        }},
        {LOAD, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_read16(m_regs.table[(instruction >> 16) & 0xFF]);
        }},
        {LOADI, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_read16((instruction >> 16) & 0xFFFF);
        }},
        {STOR, [this](u32 instruction) -> void
        {
            mem_write16(m_regs.table[(instruction >> 8) & 0xFF], m_regs.table[(instruction >> 16) & 0xFF]);
        }},
        {STORI, [this](u32 instruction) -> void
        {
            mem_write16(m_regs.table[(instruction >> 8) & 0xFF], (instruction >> 16) & 0xFFFF);
        }},
        {COPY, [this](u32 instruction) -> void
        {
            mem_write16(m_regs.table[(instruction >> 8) & 0xFF], mem_read16(m_regs.table[(instruction >> 16) & 0xFF]));
        }},
        {COPYI, [this](u32 instruction) -> void
        {
            mem_write16(m_regs.table[(instruction >> 8) & 0xFF], mem_read16((instruction >> 16) & 0xFFFF));
        }},

        {LOADB, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_read8(m_regs.table[(instruction >> 16) & 0xFF]);
        }},
        {LOADBI, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_read8((instruction >> 16) & 0xFFFF);
        }},
        {STORB, [this](u32 instruction) -> void
        {
            mem_write8(m_regs.table[(instruction >> 8) & 0xFF], m_regs.table[(instruction >> 16) & 0xFF]);
        }},
        {STORBI, [this](u32 instruction) -> void
        {
            mem_write8(m_regs.table[(instruction >> 8) & 0xFF], (instruction >> 16) & 0xFFFF);
        }},
        {COPYB, [this](u32 instruction) -> void
        {
            mem_write8(m_regs.table[(instruction >> 8) & 0xFF], mem_read8(m_regs.table[(instruction >> 16) & 0xFF]));
        }},
        {COPYBI, [this](u32 instruction) -> void
        {
            mem_write8(m_regs.table[(instruction >> 8) & 0xFF], mem_read8((instruction >> 16) & 0xFFFF));
        }},

        {PUSH, [this](u32 instruction) -> void
        {
            mem_push(m_regs.table[(instruction >> 8) & 0xFF]);
        }},
        {PUSHI, [this](u32 instruction) -> void
        {
            mem_push((instruction >> 16) & 0xFFFF);
        }},
        {POP, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_pop();
        }},
        {PUSHF, [this](u32) -> void
        {
            mem_push(m_flags);
        }},
        {POPF, [this](u32) -> void
        {
            m_flags = mem_pop();
        }},

        {CALL, [this](u32 instruction) -> void
        {
            mem_push(m_regs.regs.mo);
            mem_push(m_regs.regs.xa);
            u16 addr = (instruction >> 16) & 0xFFFF;
            m_regs.regs.xa = addr;
        }},
        {RET, [this](u32) -> void
        {
            m_regs.regs.xa = mem_pop();
            m_regs.regs.mo = mem_pop();
        }},

        {JMP, [this](u32 instruction) -> void
        {
            m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JE, [this](u32 instruction) -> void
        {
            if (m_flags & FLAGS::EQUAL)
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JNE, [this](u32 instruction) -> void
        {
            if (!(m_flags & FLAGS::EQUAL))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JG, [this](u32 instruction) -> void
        {
            if (m_flags & FLAGS::GREATER)
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JGE, [this](u32 instruction) -> void
        {
            if ((m_flags & FLAGS::GREATER) || (m_flags & FLAGS::EQUAL))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JL, [this](u32 instruction) -> void
        {
            if ((m_flags & FLAGS::LESSER))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JLE, [this](u32 instruction) -> void
        {
            if ((m_flags & FLAGS::LESSER) || (m_flags & FLAGS::EQUAL))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},

        {JER, [this](u32 instruction) -> void
        {
            if (m_flags & FLAGS::ERROR)
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JNER, [this](u32 instruction) -> void
        {
            if (!(m_flags & FLAGS::ERROR))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JXP, [this](u32 instruction) -> void
        {
            if (m_flags & FLAGS::EXCEPTION)
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {JNXP, [this](u32 instruction) -> void
        {
            if (!(m_flags & FLAGS::EXCEPTION))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},

        {LOP, [this](u32 instruction) -> void
        {
            m_regs.regs.hz--;
            if (m_regs.regs.hz != 0)
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {LOPE, [this](u32 instruction) -> void
        {
            m_regs.regs.hz--;
            if ((m_regs.regs.hz != 0) && (m_flags & FLAGS::EQUAL))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},
        {LOPNE, [this](u32 instruction) -> void
        {
            m_regs.regs.hz--;
            if ((m_regs.regs.hz != 0) && !(m_flags & FLAGS::EQUAL))
                m_regs.regs.xa = (instruction >> 16) & 0xFFFF;
        }},

        {IN, [this](u32 instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = m_bus->device_in((instruction >> 16) & 0xFFFF);
        }},
        {OUT, [this](u32 instruction) -> void
        {
            m_bus->device_out((instruction >> 16) & 0xFFFF, m_regs.table[(instruction >> 8) & 0xFF]);
        }},

        {CLER, [this](u32) -> void
        {
            m_flags &= ~FLAGS::ERROR;
        }},
        {CLXP, [this](u32) -> void
        {
            m_flags &= ~FLAGS::EXCEPTION;
        }},
        {STER, [this](u32) -> void
        {
            m_flags |= FLAGS::ERROR;
        }},
        {STXP, [this](u32) -> void
        {
            m_flags |= FLAGS::EXCEPTION;
        }},
        {STRS, [this](u32) -> void
        {
            m_flags |= FLAGS::RESET;
        }},
        {STSD, [this](u32) -> void
        {
            m_flags |= FLAGS::SHUTDOWN;
        }},
    };
    // ALL INSTRUCTION END
}

cpu::~cpu() {}

void cpu::run()
{
    if (shutdown_flag_set()) return;

    // Fetch
    u16 instruction_low = mem_read16(m_regs.regs.xa);
    u16 instruction_high = mem_read16(m_regs.regs.xa + 2);
    u32 instruction =
        (static_cast<u32>(instruction_high) << 16) | instruction_low;
    std::clog << std::format("[CPU] Opcode: 0x{:08X} FLAG {:08B}", instruction, m_flags) << std::endl;

    std::clog <<
        std::format("[CPU] AZ {:04X} BZ {:04X} CZ {:04X} DZ {:04X} EZ {:04X} FZ {:04X}",
            m_regs.regs.az, m_regs.regs.bz, m_regs.regs.cz, m_regs.regs.dz, m_regs.regs.ez, m_regs.regs.fz)
        << std::endl;

    std::clog <<
        std::format("[CPU] GZ {:04X} HZ {:04X} SP {:04X} SB {:04X} XA {:04X} MO {:04X}",
            m_regs.regs.gz, m_regs.regs.hz, m_regs.regs.sp, m_regs.regs.sb, m_regs.regs.xa, m_regs.regs.mo)
        << std::endl;

    // Check
    u8 opcode = (instruction & 0xFF);
    const auto& executer = executers.find(static_cast<INSTRUCTION>(opcode));
    if (executer == executers.end()) {
        throw std::logic_error(std::format("[CPU] Illegal instruction 0x{:02X}", opcode));
    }

    // Next
    m_regs.regs.xa += 4;
    executer->second(instruction);
    if (m_flags & FLAGS::RESET)
        reboot();
}

bool cpu::shutdown_flag_set()
{
    return (m_flags & FLAGS::SHUTDOWN);
}

bool cpu::exception_flag_set()
{
    return (m_flags & FLAGS::EXCEPTION);
}

void cpu::reboot()
{
    m_flags = 0;
    std::memset(m_regs.table, 0, REG_COUNT * 2);
}

u16 cpu::mem_read16(u16 addr)
{
    return m_bus->mem_read16(m_regs.regs.mo + addr);
}

void cpu::mem_write16(u16 addr, u16 data)
{
    m_bus->mem_write16(m_regs.regs.mo + addr, data);
}

u8 cpu::mem_read8(u16 addr)
{
    return m_bus->mem_read8(m_regs.regs.mo + addr);
}

void cpu::mem_write8(u16 addr, u8 data)
{
    m_bus->mem_write8(m_regs.regs.mo + addr, data);
}

u16 cpu::mem_pop()
{
    m_regs.regs.sp -= 2;
    u16 data = m_bus->mem_read16(m_regs.regs.sb + m_regs.regs.sp);
    return data;
}

void cpu::mem_push(u16 data)
{
    m_bus->mem_write16(m_regs.regs.sb + m_regs.regs.sp, data);
    m_regs.regs.sp += 2;
}

void cpu::set_cmp_flags(u16 operand1, u16 operand2)
{
    if (operand1 == operand2)
        m_flags |= FLAGS::EQUAL;
    else m_flags &= ~FLAGS::EQUAL;

    if (operand1 < operand2)
        m_flags |= FLAGS::LESSER;
    else m_flags &= ~FLAGS::LESSER;

    if (operand1 > operand2)
        m_flags |= FLAGS::GREATER;
    else m_flags &= ~FLAGS::GREATER;
}