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

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <cstdint>

#include <fstream>
#include <optional>
#include <unordered_map>
#include <vector>

#include <cosmovm/cpu.hpp>

namespace cosmoasm {

typedef enum KEYWORD: std::uint16_t {
    BEGIN_REG = 0x01,

    // General purpose
    REG_AZ    = BEGIN_REG + 0x00,
    REG_BZ    = BEGIN_REG + 0x01,
    REG_CZ    = BEGIN_REG + 0x02,
    REG_DZ    = BEGIN_REG + 0x03,
    REG_EZ    = BEGIN_REG + 0x04,
    REG_FZ    = BEGIN_REG + 0x05,
    REG_GZ    = BEGIN_REG + 0x06,
    REG_HZ    = BEGIN_REG + 0x07,

    REG_SP    = BEGIN_REG + 0x08, // Stack Ptr
    REG_SB    = BEGIN_REG + 0x09, // Stack Base
    REG_XA    = BEGIN_REG + 0x0A, // eXecution Address
    REG_MO    = BEGIN_REG + 0x0B, // Memory/Data Offset

    BEGIN_INS = REG_MO + 1,

    // Instruction
    INS_WCYL  = BEGIN_INS + cosmovm::WCYL,
    INS_ADD   = BEGIN_INS + cosmovm::ADD,
    INS_ADDI  = BEGIN_INS + cosmovm::ADDI,
    INS_SUB   = BEGIN_INS + cosmovm::SUB,
    INS_SUBI  = BEGIN_INS + cosmovm::SUBI,
    INS_MUL   = BEGIN_INS + cosmovm::MUL,
    INS_MULI  = BEGIN_INS + cosmovm::MULI,
    INS_DIV   = BEGIN_INS + cosmovm::DIV,
    INS_DIVI  = BEGIN_INS + cosmovm::DIVI,
    INS_INC   = BEGIN_INS + cosmovm::INC,
    INS_DEC   = BEGIN_INS + cosmovm::DEC,
    INS_NEG   = BEGIN_INS + cosmovm::NEG,
    INS_CMP   = BEGIN_INS + cosmovm::CMP,
    INS_CMPI  = BEGIN_INS + cosmovm::CMPI,
    INS_AND   = BEGIN_INS + cosmovm::AND,
    INS_ANDI  = BEGIN_INS + cosmovm::ANDI,
    INS_OR    = BEGIN_INS + cosmovm::OR,
    INS_ORI   = BEGIN_INS + cosmovm::ORI,
    INS_XOR   = BEGIN_INS + cosmovm::XOR,
    INS_XORI  = BEGIN_INS + cosmovm::XORI,
    INS_SHL   = BEGIN_INS + cosmovm::SHL,
    INS_SHLI  = BEGIN_INS + cosmovm::SHLI,
    INS_SHR   = BEGIN_INS + cosmovm::SHR,
    INS_SHRI  = BEGIN_INS + cosmovm::SHRI,
    INS_NOT   = BEGIN_INS + cosmovm::NOT,
    INS_MOV   = BEGIN_INS + cosmovm::MOV,
    INS_MOVI  = BEGIN_INS + cosmovm::MOVI,
    INS_LOAD  = BEGIN_INS + cosmovm::LOAD,
    INS_LOADI = BEGIN_INS + cosmovm::LOADI,
    INS_STOR  = BEGIN_INS + cosmovm::STOR,
    INS_STORI = BEGIN_INS + cosmovm::STORI,
    INS_COPY  = BEGIN_INS + cosmovm::COPY,
    INS_COPYI = BEGIN_INS + cosmovm::COPYI,
    INS_LOADB = BEGIN_INS + cosmovm::LOADB,
    INS_LOADBI= BEGIN_INS + cosmovm::LOADBI,
    INS_STORB = BEGIN_INS + cosmovm::STORB,
    INS_STORBI= BEGIN_INS + cosmovm::STORBI,
    INS_COPYB = BEGIN_INS + cosmovm::COPYB,
    INS_COPYBI= BEGIN_INS + cosmovm::COPYBI,
    INS_PUSH  = BEGIN_INS + cosmovm::PUSH,
    INS_PUSHI = BEGIN_INS + cosmovm::PUSHI,
    INS_POP   = BEGIN_INS + cosmovm::POP,
    INS_PUSHF = BEGIN_INS + cosmovm::PUSHF,
    INS_POPF  = BEGIN_INS + cosmovm::POPF,
    INS_CALL  = BEGIN_INS + cosmovm::CALL,
    INS_RET   = BEGIN_INS + cosmovm::RET,
    INS_JMP   = BEGIN_INS + cosmovm::JMP,
    INS_JE    = BEGIN_INS + cosmovm::JE,
    INS_JNE   = BEGIN_INS + cosmovm::JNE,
    INS_JG    = BEGIN_INS + cosmovm::JG,
    INS_JGE   = BEGIN_INS + cosmovm::JGE,
    INS_JL    = BEGIN_INS + cosmovm::JL,
    INS_JLE   = BEGIN_INS + cosmovm::JLE,
    INS_JER   = BEGIN_INS + cosmovm::JER,
    INS_JNER  = BEGIN_INS + cosmovm::JNER,
    INS_JXP   = BEGIN_INS + cosmovm::JXP,
    INS_JNXP  = BEGIN_INS + cosmovm::JNXP,
    INS_LOP   = BEGIN_INS + cosmovm::LOP,
    INS_LOPE  = BEGIN_INS + cosmovm::LOPE,
    INS_LOPNE = BEGIN_INS + cosmovm::LOPNE,
    INS_IN    = BEGIN_INS + cosmovm::IN,
    INS_OUT   = BEGIN_INS + cosmovm::OUT,
    INS_CLER  = BEGIN_INS + cosmovm::CLER,
    INS_CLXP  = BEGIN_INS + cosmovm::CLXP,
    INS_STER  = BEGIN_INS + cosmovm::STER,
    INS_STXP  = BEGIN_INS + cosmovm::STXP,
    INS_STRS  = BEGIN_INS + cosmovm::STRS,
    INS_STSD  = BEGIN_INS + cosmovm::STSD,

    BEGIN_ASM = INS_STSD + 1,
    ASM_LOCATE= BEGIN_ASM + 0x00,
    ASM_TYPSTR= BEGIN_ASM + 0x01,
    ASM_TYPI8 = BEGIN_ASM + 0x02,
    ASM_TYPU8 = BEGIN_ASM + 0x03,
    ASM_TYPI16= BEGIN_ASM + 0x04,
    ASM_TYPU16= BEGIN_ASM + 0x05,
    END       = ASM_TYPU16 + 1,
}KEYWORD;

typedef enum ARG_TYPE: std::uint8_t {
    NONE    = 0b000000,
    REG_REG = 0b001110,
    REG_IMM = 0b001010,
    IMM     = 0b000001,
    REG     = 0b001001,

    NAME    = 0b010001,
    LITERAL = 0b100001,
    INF_INT = 0b110011,
}ARG_TYPE;

std::vector<std::string> split(std::string input, const std::string& delimiter);

std::string trim(const std::string& input);

std::optional<std::int32_t> int_literal(const std::string& input);

bool valid_name(std::string input, std::size_t line_count);

void assemble_instruction(
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::uint16_t, std::string>& references,
    std::vector<std::string>& arguments,
    KEYWORD token,
    ARG_TYPE type,
    std::size_t line_count);

void assemble_command(
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::string, std::uint16_t>& addresses,
    std::vector<std::string>& arguments,
    std::string command,
    KEYWORD token,
    std::size_t line_count);

void assemble(
    std::string input,
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::string, std::uint16_t>& addresses,
    std::unordered_map<std::uint16_t, std::string>& references);

std::size_t fsize(std::ifstream& file);
std::size_t fsize(std::fstream& file);

void write_addresses(
    const std::unordered_map<std::string, std::uint16_t>& addresses,
    std::ofstream& addr_file);

void write_references(
    const std::unordered_map<std::uint16_t, std::string>& references,
    std::ofstream& ref_file);

void link_info(
    const std::unordered_map<std::string, std::uint16_t>& addresses,
    const std::unordered_map<std::uint16_t, std::string>& references);

void assemble(
    std::ifstream& asm_file,
    std::ofstream& code_file,
    std::ofstream& addr_file,
    std::ofstream& ref_file);

void read_addresses(
    std::ifstream& addr_file,
    std::unordered_map<std::string, std::uint16_t>& addresses);

void read_references(
    std::ifstream& ref_file,
    std::unordered_map<std::uint16_t, std::string>& references);

void link(
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::string, std::uint16_t>& addresses,
    std::unordered_map<std::uint16_t, std::string>& references);

void link(
    std::ifstream& code_file,
    std::ifstream& addr_file,
    std::ifstream& ref_file,
    std::ofstream& bin_file);

void link(
    const std::vector<std::tuple<
        std::vector<std::uint8_t>,
        std::unordered_map<std::string, std::uint16_t>,
        std::unordered_map<std::uint16_t, std::string>>>& objects,
    std::vector<std::uint8_t>& output_bytecode,
    std::unordered_map<std::string, std::uint16_t>& output_addr,
    std::unordered_map<std::uint16_t, std::string>& output_ref);

void link(
    std::vector<std::tuple<std::ifstream, std::ifstream, std::ifstream>>& objects,
    std::ofstream& output_addr_file,
    std::ofstream& output_ref_file,
    std::ofstream& output_bin_file);
}

#endif /* ASSEMBLER_HPP */