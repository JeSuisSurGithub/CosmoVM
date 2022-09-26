/**
 * CosmoVM an emulator and assembler for an imaginary cpu
 * Copyright (C) 2022 JeFaisDesSpaghettis
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

#include <cosmocore/cosmocpu.hpp>

using namespace cosmocore;

cosmocpu::cosmocpu(std::shared_ptr<cosmobus>& bus)
: m_bus(bus)
{
    m_regs.regs.yz = START_ADDR;
    // ALL INSTRUCTION START
    executers =
    {
        // WCYL
        {0x00, [](u32i) -> void {}},

        // ADD
        {0x01, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16i result = operand1 + operand2;
            dest_reg = result;
        }},
        // ADDi
        {0x02, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            u16i result = operand1 + operand2;
            dest_reg = result;
        }},

        // AND
        {0x03, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16i result = operand1 & operand2;
            dest_reg = result;
        }},
        // ANDi
        {0x04, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            u16i result = operand1 & operand2;
            dest_reg = result;
        }},

        // CALL
        {0x05, [this](u32i instruction) -> void
        {
            mem_push(m_regs.regs.zz);
            mem_push(m_regs.regs.yz);
            u16i addr = (instruction >> 8) & 0xFFFF;
            m_regs.regs.yz = addr;
        }},
        // CLE
        {0x06, [this](u32i) -> void
        {
            m_flags &= ~FLAGS::ERROR;
        }},
        // CLI
        /*{0x07, [this](u32i) -> void
        {
            m_flags &= ~FLAGS::INTERRUPT;
        }},*/
        // CLXP
        {0x08, [this](u32i) -> void
        {
            m_flags &= ~FLAGS::EXCEPTION;
        }},

        // CMP
        {0x09, [this](u32i instruction) -> void
        {
            u16i operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            set_cmp_flags(operand1, operand2);
        }},
        // CMPi
        {0x0A, [this](u32i instruction) -> void
        {
            u16i operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            set_cmp_flags(operand1, operand2);
        }},

        // DEC
        {0x0B, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF]--;
        }},

        // DIV
        {0x0C, [this](u32i instruction) -> void
        {
            u16i operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16i quotient = operand1 / operand2;
            u16i remainder = operand1 % operand2;
            m_regs.regs.iz = quotient;
            m_regs.regs.jz = remainder;
        }},
        // DIVi
        {0x0D, [this](u32i instruction) -> void
        {
            u16i operand1 = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            u16i quotient = operand1 / operand2;
            u16i remainder = operand1 % operand2;
            m_regs.regs.iz = quotient;
            m_regs.regs.jz = remainder;
        }},

        // IN
        {0x0E, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = m_bus->device_in((instruction >> 16) & 0xFFFF);
        }},
        // INb
        {0x0F, [this](u32i instruction) -> void
        {
            // WIP
        }},

        // INC
        {0x10, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF]++;
        }},

        // JE
        {0x11, [this](u32i instruction) -> void
        {
            if (m_flags & FLAGS::EQUAL)
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JER
        {0x12, [this](u32i instruction) -> void
        {
            if (m_flags & FLAGS::ERROR)
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JG
        {0x13, [this](u32i instruction) -> void
        {
            if (m_flags & FLAGS::GREATER)
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JGE
        {0x14, [this](u32i instruction) -> void
        {
            if ((m_flags & FLAGS::GREATER) || (m_flags & FLAGS::EQUAL))
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JLE
        {0x15, [this](u32i instruction) -> void
        {
            if ((m_flags & FLAGS::LESSER) || (m_flags & FLAGS::EQUAL))
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JMP
        {0x16, [this](u32i instruction) -> void
        {
            m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JNE
        {0x17, [this](u32i instruction) -> void
        {
            if (!(m_flags & FLAGS::EQUAL))
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JNER
        {0x18, [this](u32i instruction) -> void
        {
            if (!(m_flags & FLAGS::ERROR))
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JNXP
        {0x19, [this](u32i instruction) -> void
        {
            if (!(m_flags & FLAGS::EXCEPTION))
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // JXP
        {0x1A, [this](u32i instruction) -> void
        {
            if (m_flags & FLAGS::EXCEPTION)
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},

        // LOP
        {0x1B, [this](u32i instruction) -> void
        {
            m_regs.regs.jz--;
            if (m_regs.regs.jz != 0)
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // LOPE
        {0x1C, [this](u32i instruction) -> void
        {
            m_regs.regs.jz--;
            if ((m_regs.regs.jz != 0) && (m_flags & FLAGS::EQUAL))
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},
        // LOPNE
        {0x1D, [this](u32i instruction) -> void
        {
            m_regs.regs.jz--;
            if ((m_regs.regs.jz != 0) && !(m_flags & FLAGS::EQUAL))
                m_regs.regs.yz = (instruction >> 8) & 0xFFFF;
        }},

        // MOV
        {0x1E, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = m_regs.table[(instruction >> 16) & 0xFF];
        }},
        // MOVrm
        {0x1F, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_read16(m_regs.table[(instruction >> 16) & 0xFF]);
        }},
        // MOVri
        {0x20, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = (instruction >> 16) & 0xFFFF;
        }},

        // MOVmr
        {0x21, [this](u32i instruction) -> void
        {
            mem_write16(m_regs.table[(instruction >> 8) & 0xFF], m_regs.table[(instruction >> 16) & 0xFF]);
        }},
        // MOVmm
        {0x22, [this](u32i instruction) -> void
        {
            mem_write16(m_regs.table[(instruction >> 8) & 0xFF], mem_read16(m_regs.table[(instruction >> 16) & 0xFF]));
        }},
        // MOVmi
        {0x23, [this](u32i instruction) -> void
        {
            mem_write16(m_regs.table[(instruction >> 8) & 0xFF], (instruction >> 16) & 0xFFFF);
        }},
        // MOVbrm
        {0x3E, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_read8(m_regs.table[(instruction >> 16) & 0xFF]);
        }},
        // MOVbmr
        {0x24, [this](u32i instruction) -> void
        {
            mem_write8(m_regs.table[(instruction >> 8) & 0xFF], m_regs.table[(instruction >> 16) & 0xFF] & 0xFF);
        }},
        // MOVbmm
        {0x25, [this](u32i instruction) -> void
        {
            mem_write8(m_regs.table[(instruction >> 8) & 0xFF], mem_read8(m_regs.table[(instruction >> 16) & 0xFF]));
        }},
        // MOVbmi
        {0x26, [this](u32i instruction) -> void
        {
            mem_write8(m_regs.table[(instruction >> 8) & 0xFF], (instruction >> 16) & 0xFF);
        }},

        // MUL
        {0x27, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16i result = operand1 * operand2;
            dest_reg = result;
        }},
        // MULi
        {0x28, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            u16i result = operand1 * operand2;
            dest_reg = result;
        }},

        // NEG
        {0x29, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = (~m_regs.table[(instruction >> 8) & 0xFFFF] + 1);
        }},

        // NOT
        {0x2A, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = ~m_regs.table[(instruction >> 8) & 0xFFFF];
        }},

        // OR
        {0x2B, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16i result = operand1 | operand2;
            dest_reg = result;
        }},
        // ORi
        {0x2C, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            u16i result = operand1 | operand2;
            dest_reg = result;
        }},

        // OUT
        {0x2D, [this](u32i instruction) -> void
        {
            m_bus->device_out((instruction >> 16) & 0xFFFF, m_regs.table[(instruction >> 8) & 0xFF]);
        }},
        // OUTb
        {0x2E, [this](u32i instruction) -> void
        {
            // WIP
        }},

        // POP
        {0x2F, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] = mem_pop();
        }},
        // POPF
        {0x30, [this](u32i instruction) -> void
        {
            m_flags = mem_pop();
        }},

        // PUSH
        {0x31, [this](u32i instruction) -> void
        {
            mem_push(m_regs.table[(instruction >> 8) & 0xFF]);
        }},
        // PUSHF
        {0x32, [this](u32i instruction) -> void
        {
            mem_push(m_flags);
        }},
        // RET
        {0x33, [this](u32i instruction) -> void
        {
            m_regs.regs.yz = mem_pop();
            m_regs.regs.zz = mem_pop();
        }},

        // SHL
        {0x34, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] <<= m_regs.table[(instruction >> 16) & 0xFF];
        }},
        // SHLi
        {0x3F, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] <<= (instruction >> 16) & 0xFFFF;
        }},
        // SHR
        {0x35, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] >>= m_regs.table[(instruction >> 16) & 0xFF];
        }},
        // SHRi
        {0x40, [this](u32i instruction) -> void
        {
            m_regs.table[(instruction >> 8) & 0xFF] >>= (instruction >> 16) & 0xFFFF;
        }},

        // STE
        {0x36, [this](u32i instruction) -> void
        {
            m_flags |= FLAGS::ERROR;
        }},
        /*// STI
        {0x37, [this](u32i instruction) -> void
        {
            m_flags |= FLAGS::INTERRUPT;
        }},*/
        // STXP
        {0x38, [this](u32i instruction) -> void
        {
            m_flags |= FLAGS::EXCEPTION;
        }},
        // STRS
        {0x39, [this](u32i instruction) -> void
        {
            m_flags |= FLAGS::RESET;
        }},
        // STSD
        {0x41, [this](u32i instruction) -> void
        {
            m_flags |= FLAGS::SHUTDOWN;
        }},
        // SUB
        {0x3A, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16i result = operand1 - operand2;
            dest_reg = result;
        }},
        // SUBi
        {0x3B, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            u16i result = operand1 - operand2;
            dest_reg = result;
        }},

        // XOR
        {0x3C, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = m_regs.table[(instruction >> 16) & 0xFF];
            u16i result = operand1 ^ operand2;
            dest_reg = result;
        }},
        // XORi
        {0x3D, [this](u32i instruction) -> void
        {
            u16i& dest_reg = m_regs.table[(instruction >> 8) & 0xFF];
            u16i operand1 = dest_reg;
            u16i operand2 = (instruction >> 16) & 0xFFFF;
            u16i result = operand1 ^ operand2;
            dest_reg = result;
        }}
    };
    // ALL INSTRUCTION END
}

cosmocpu::~cosmocpu() {}

void cosmocpu::run()
{
    if (shutdown_flag_set())
        return;

    // Fetch
    u32i instruction{0};
    u16i instruction_low = mem_read16(m_regs.regs.zz + m_regs.regs.yz);
    u16i instruction_high = mem_read16(m_regs.regs.zz + m_regs.regs.yz + 2);
    instruction =
        (static_cast<u32i>(instruction_high) << 16) | instruction_low;
    m_regs.regs.yz += 4;

    u8i opcode = (instruction & 0xFF);
    executers.at(opcode)(instruction);
    if (m_flags & FLAGS::RESET)
        reboot();
}

bool cosmocpu::shutdown_flag_set()
{
    return (m_flags & FLAGS::SHUTDOWN);
}

void cosmocpu::reboot()
{
    m_flags = 0;
    std::memset(m_regs.table, 0, REG_COUNT * 2);
}

u16i cosmocpu::mem_read16(u16i addr)
{
    return m_bus->mem_read16(addr + m_regs.regs.zz);
}

void cosmocpu::mem_write16(u16i addr, u16i data)
{
    m_bus->mem_write16(addr + m_regs.regs.zz, data);
}

u8i cosmocpu::mem_read8(u16i addr)
{
    return m_bus->mem_read8(addr + m_regs.regs.zz);
}

void cosmocpu::mem_write8(u16i addr, u8i data)
{
    m_bus->mem_write8(addr + m_regs.regs.zz, data);
}

u16i cosmocpu::mem_pop()
{
    m_regs.regs.wz -= 2;
    u16i data = m_bus->mem_read16(m_regs.regs.xz + m_regs.regs.wz);
    return data;
}

void cosmocpu::mem_push(u16i data)
{
    m_bus->mem_write16(m_regs.regs.xz + m_regs.regs.wz, data);
    m_regs.regs.wz += 2;
}

void cosmocpu::set_cmp_flags(u16i operand1, u16i operand2)
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