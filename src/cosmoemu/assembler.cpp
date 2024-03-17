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

#include <cstdint>

#include <algorithm>
#include <fstream>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "assembler.hpp"

namespace cosmoasm {

const std::unordered_map<std::string, std::pair<KEYWORD, ARG_TYPE>> TOKENS = {
    {"AZ", {REG_AZ, NONE}},
    {"BZ", {REG_BZ, NONE}},
    {"CZ", {REG_CZ, NONE}},
    {"DZ", {REG_DZ, NONE}},
    {"EZ", {REG_EZ, NONE}},
    {"FZ", {REG_FZ, NONE}},
    {"GZ", {REG_GZ, NONE}},
    {"HZ", {REG_HZ, NONE}},

    {"SP", {REG_SP, NONE}},
    {"SB", {REG_SB, NONE}},
    {"XA", {REG_XA, NONE}},
    {"MO", {REG_MO, NONE}},

    {"WCYL",   {INS_WCYL  , NONE}},

    {"ADD",    {INS_ADD   , REG_REG}},
    {"ADDI",   {INS_ADDI  , REG_IMM}},
    {"SUB",    {INS_SUB   , REG_REG}},
    {"SUBI",   {INS_SUBI  , REG_IMM}},
    {"MUL",    {INS_MUL   , REG_REG}},
    {"MULI",   {INS_MULI  , REG_IMM}},
    {"DIV",    {INS_DIV   , REG_REG}},
    {"DIVI",   {INS_DIVI  , REG_IMM}},
    {"INC",    {INS_INC   , REG}},
    {"DEC",    {INS_DEC   , REG}},
    {"NEG",    {INS_NEG   , REG}},

    {"CMP",    {INS_CMP   , REG_REG}},
    {"CMPI",   {INS_CMPI  , REG_IMM}},

    {"AND",    {INS_AND   , REG_REG}},
    {"ANDI",   {INS_ANDI  , REG_IMM}},
    {"OR",     {INS_OR    , REG_REG}},
    {"ORI",    {INS_ORI   , REG_IMM}},
    {"XOR",    {INS_XOR   , REG_REG}},
    {"XORI",   {INS_XORI  , REG_IMM}},
    {"SHL",    {INS_SHL   , REG_REG}},
    {"SHLI",   {INS_SHLI  , REG_IMM}},
    {"SHR",    {INS_SHR   , REG_REG}},
    {"SHRI",   {INS_SHRI  , REG_IMM}},
    {"NOT",    {INS_NOT   , REG}},

    {"MOV",    {INS_MOV   , REG_REG}},
    {"MOVI",   {INS_MOVI  , REG_IMM}},
    {"LOAD",   {INS_LOAD  , REG_REG}},
    {"LOADI",  {INS_LOADI , REG_IMM}},
    {"STOR",   {INS_STOR  , REG_REG}},
    {"STORI",  {INS_STORI , REG_IMM}},
    {"COPY",   {INS_COPY  , REG_REG}},
    {"COPYI",  {INS_COPYI , REG_IMM}},

    {"LOADB",  {INS_LOADB , REG_REG}},
    {"LOADBI", {INS_LOADBI, REG_IMM}},
    {"STORB",  {INS_STORB , REG_REG}},
    {"STORBI", {INS_STORBI, REG_IMM}},
    {"COPYB",  {INS_COPYB , REG_REG}},
    {"COPYBI", {INS_COPYBI, REG_IMM}},

    {"PUSH",   {INS_PUSH  , REG}},
    {"PUSHI",  {INS_PUSHI , IMM}},
    {"POP",    {INS_POP   , REG}},
    {"PUSHF",  {INS_PUSHF , NONE}},
    {"POPF",   {INS_POPF  , NONE}},

    {"CALL",   {INS_CALL  , IMM}},
    {"RET",    {INS_RET   , NONE}},

    {"JMP",    {INS_JMP   , IMM}},
    {"JE",     {INS_JE    , IMM}},
    {"JNE",    {INS_JNE   , IMM}},
    {"JG",     {INS_JG    , IMM}},
    {"JGE",    {INS_JGE   , IMM}},
    {"JL",     {INS_JL    , IMM}},
    {"JLE",    {INS_JLE   , IMM}},
    {"JER",    {INS_JER   , IMM}},
    {"JNER",   {INS_JNER  , IMM}},
    {"JXP",    {INS_JXP   , IMM}},
    {"JNXP",   {INS_JNXP  , IMM}},

    {"LOP",    {INS_LOP   , IMM}},
    {"LOPE",   {INS_LOPE  , IMM}},
    {"LOPNE",  {INS_LOPNE , IMM}},

    {"IN",     {INS_IN    , REG_IMM}},
    {"OUT",    {INS_OUT   , REG_IMM}},

    {"CLER",   {INS_CLER  , NONE}},
    {"CLXP",   {INS_CLXP  , NONE}},
    {"STER",   {INS_STER  , NONE}},
    {"STXP",   {INS_STXP  , NONE}},
    {"STRS",   {INS_STRS  , NONE}},
    {"STSD",   {INS_STSD  , NONE}},

    {"LOCATE", {ASM_LOCATE, NAME}},
    {"STR",    {ASM_TYPSTR, LITERAL}},
    {"I8",     {ASM_TYPI8 , INF_INT}},
    {"U8",     {ASM_TYPU8 , INF_INT}},
    {"I16",    {ASM_TYPI16, INF_INT}},
    {"U16",    {ASM_TYPU16, INF_INT}},
};

std::vector<std::string> split(std::string input, const std::string& delimiter)
{
    std::vector<std::string> tokens;

    std::size_t pos = 0;

    while ((pos = input.find(delimiter)) != std::string::npos)
    {
        tokens.push_back(input.substr(0, pos));
        input.erase(0, pos + delimiter.length());
    }
    tokens.push_back(input.substr(0, pos));
    return tokens;
}

std::string trim(const std::string& input)
{
    const auto begin = input.find_first_not_of(" \t");
    if (begin == std::string::npos)
        return "";

    const auto end = input.find_last_not_of(" \t");
    const auto range = end - begin + 1;

    return input.substr(begin, range);
}

std::optional<std::int32_t> int_literal(const std::string& input)
{
    std::int32_t base = 10;
    bool is_signed = false;
    std::string integer = input;

    if (input.length() > 2 &&
        input.substr(0, 2) == "0b" &&
        std::all_of(input.begin() + 2, input.end(), [](const char& c) {
            return c == '0' || c == '1';
        }))
    {
        base = 2;
        is_signed = false;
        integer = input.substr(2);
    } else if ( input.length() > 2 &&
                input.substr(0, 2) == "0x" &&
                std::all_of(input.begin() + 2, input.end(), [](const char& c) {
                    return std::isxdigit(c);
                }))
    {
        base = 16;
        is_signed = false;
        integer = input.substr(2);
    } else if ( input.length() > 1 &&
                input.at(0) == '-' &&
                std::all_of(input.begin() + 1, input.end(), [](const char& c) {
                    return std::isdigit(c);
                }))
    {
        base = 10;
        is_signed = true;
        integer = input;
    } else if ( std::all_of(input.begin(), input.end(), [](const char& c) {
                    return std::isdigit(c);
                }))
    {
        base = 10;
        is_signed = false;
        integer = input;
    } else {
        return std::nullopt;
    }

    return (is_signed) ? std::stol(integer, nullptr, base) : std::stoul(integer, nullptr, base);
}

bool valid_name(std::string input, std::size_t line_count) {
    bool correct = std::all_of(input.begin(), input.end(),
        [](char const &c) {
            return (std::isalnum(c)) || (c == '_');
        }) && (std::isalpha(input.at(0)) || input.at(0) == '_');

    std::transform(input.begin(), input.end(), input.begin(), ::toupper);

    if (TOKENS.find(input) != TOKENS.end()) {
        throw std::invalid_argument(
            std::format("[ASSEMBLER] Line {}: Forbidden keyword {}",
                line_count, input));
    }
    return correct;
}

void assemble_instruction(
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::uint16_t, std::string>& references,
    std::vector<std::string>& arguments,
    KEYWORD token,
    ARG_TYPE type,
    std::size_t line_count)
{

    // Check argument type
    switch(type) {
        case REG_REG:
            {
                std::string& arg1 = arguments.at(0);
                std::string& arg2 = arguments.at(1);

                // Ignore case
                std::transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);
                std::transform(arg2.begin(), arg2.end(), arg2.begin(), ::toupper);

                auto try_as_reg1 = TOKENS.find(arg1);
                auto try_as_reg2 = TOKENS.find(arg2);
                if (try_as_reg1 == TOKENS.end() || try_as_reg2 == TOKENS.end() ||
                    try_as_reg1->second.first >= BEGIN_INS || try_as_reg2->second.first >= BEGIN_INS) {
                    throw std::invalid_argument(
                        std::format("[ASSEMBLER] Line {}: Invalid registers {}, {}",
                            line_count, arg1, arg2));
                }
                bytecode.push_back((token - BEGIN_INS) & 0xFF);
                bytecode.push_back(try_as_reg1->second.first & 0xFF);
                bytecode.push_back(try_as_reg2->second.first & 0xFF);
                bytecode.push_back(0);
            }
            break;
        case REG_IMM:
            {
                std::string& arg1 = arguments.at(0);
                std::string& arg2 = arguments.at(1);

                // Ignore case
                std::transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);

                auto try_as_reg1 = TOKENS.find(arg1);
                if (try_as_reg1 == TOKENS.end() || try_as_reg1->second.first >= BEGIN_INS) {
                    throw std::invalid_argument(
                        std::format("[ASSEMBLER] Line {}: Invalid register {}",
                            line_count, arg1));
                }
                std::uint16_t op2 = 0;
                auto try_as_int2 = int_literal(arg2);
                if (try_as_int2.has_value() && (static_cast<std::uint16_t>(try_as_int2.value()) == try_as_int2.value())) {
                    op2 = try_as_int2.value();
                } else if (valid_name(arg2, line_count)) {
                    references[bytecode.size()] = arg2;
                } else {
                    throw std::invalid_argument(
                        std::format("[ASSEMBLER] Line {}: Invalid immediate or label {}",
                            line_count, arg2));
                }
                bytecode.push_back((token - BEGIN_INS) & 0xFF);
                bytecode.push_back(try_as_reg1->second.first & 0xFF);
                bytecode.push_back(op2 & 0xFF);
                bytecode.push_back((op2 >> 8) & 0xFF);
            }
            break;
        case REG:
            {
                std::string& arg1 = arguments.at(0);

                // Ignore case
                std::transform(arg1.begin(), arg1.end(), arg1.begin(), ::toupper);

                auto try_as_reg1 = TOKENS.find(arg1);
                if (try_as_reg1 == TOKENS.end() || try_as_reg1->second.first >= BEGIN_INS) {
                    throw std::invalid_argument(
                        std::format("[ASSEMBLER] Line {}: Invalid register {}",
                            line_count, arg1));
                }
                bytecode.push_back((token - BEGIN_INS) & 0xFF);
                bytecode.push_back(try_as_reg1->second.first & 0xFF);
                bytecode.push_back(0);
                bytecode.push_back(0);
            }
            break;
        case IMM:
            {
                std::string& arg1 = arguments.at(0);

                std::uint16_t op1 = 0;
                auto try_as_int1 = int_literal(arg1);
                if (try_as_int1.has_value() && (static_cast<std::uint16_t>(try_as_int1.value()) == try_as_int1.value())) {
                    op1 = try_as_int1.value_or(0);
                } else if (valid_name(arg1, line_count)) {
                    references[bytecode.size()] = arg1;
                } else {
                    throw std::invalid_argument(
                        std::format("[ASSEMBLER] Line {}: Invalid immediate or label {}",
                            line_count, arg1));
                }
                bytecode.push_back((token - BEGIN_INS) & 0xFF);
                bytecode.push_back(0);
                bytecode.push_back(op1 & 0xFF);
                bytecode.push_back((op1 >> 8) & 0xFF);
            }
            break;
        case NONE:
            {
                bytecode.push_back((token - BEGIN_INS) & 0xFF);
                bytecode.push_back(0);
                bytecode.push_back(0);
                bytecode.push_back(0);
            }
            break;
        default:
            {
                throw std::logic_error(
                    std::format("[ASSEMBLER] Line {}: Internal error, unknown argument type {}",
                        line_count, static_cast<std::uint16_t>(type)));
            }
            break;
    }
}

void assemble_command(
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::string, std::uint16_t>& addresses,
    std::vector<std::string>& arguments,
    std::string command,
    KEYWORD token,
    std::size_t line_count)
{
    switch (token)
        {
            case ASM_LOCATE:
                {
                    if (valid_name(arguments.at(0), line_count)) {
                        auto preexisting_addr_name = addresses.find(arguments.at(0));
                        if (preexisting_addr_name != addresses.end()) {
                            throw std::invalid_argument(
                                std::format("[ASSEMBLER] Line {}: Duplicate location {} with line {}",
                                    line_count, arguments.at(0), preexisting_addr_name->second));
                        } else {
                            addresses[arguments.at(0)] = bytecode.size();
                        }
                    } else {
                            throw std::invalid_argument(
                                std::format("[ASSEMBLER] Line {}: Invalid location name {}",
                                    line_count, arguments.at(0)));
                    }
                }
                break;
            case ASM_TYPSTR:
                {
                    if (std::all_of(arguments.at(0).begin(), arguments.at(0).end(),
                        [](char const &c) {
                            return std::isprint(c);
                        }))
                    {
                        bytecode.insert(bytecode.end(), arguments.at(0).begin(), arguments.at(0).end() - 1);
                    } else {
                        throw std::invalid_argument(
                            std::format("[ASSEMBLER] Line {}: Unsupported characters in {}",
                                line_count, arguments.at(0)));
                    }
                }
                break;
            case ASM_TYPI16:
                {
                    for (const auto& arg : arguments) {
                        auto try_as_int = int_literal(arg);
                        if (try_as_int.has_value()) {
                            std::int32_t res = try_as_int.value();
                            if (static_cast<std::int16_t>(res) == res) {
                                bytecode.push_back(res & 0xFF);
                                bytecode.push_back((res >> 8) & 0xFF);
                            } else {
                                throw std::invalid_argument(
                                    std::format("[ASSEMBLER] Line {}: Invalid 16-bit signed integer {}",
                                        line_count, arg));
                            }
                        } else {
                            throw std::invalid_argument(
                                std::format("[ASSEMBLER] Line {}: Invalid integer {}",
                                    line_count, arg));
                        }
                    }
                }
                break;
            case ASM_TYPU16:
                {
                    for (const auto& arg : arguments) {
                        auto try_as_int = int_literal(arg);
                        if (try_as_int.has_value()) {
                            std::int32_t res = try_as_int.value();
                            if (static_cast<std::uint16_t>(res) == res) {
                                bytecode.push_back(res & 0xFF);
                                bytecode.push_back((res >> 8) & 0xFF);
                            } else {
                                throw std::invalid_argument(
                                    std::format("[ASSEMBLER] Line {}: Invalid 16-bit unsigned integer {}",
                                        line_count, arg));
                            }
                        } else {
                            throw std::invalid_argument(
                                std::format("[ASSEMBLER] Line {}: Invalid integer {}",
                                    line_count, arg));
                        }
                    }
                }
                break;
            case ASM_TYPI8:
                {
                    for (const auto& arg : arguments) {
                        auto try_as_int = int_literal(arg);
                        if (try_as_int.has_value()) {
                            std::int32_t res = try_as_int.value();
                            if (static_cast<std::int8_t>(res) == res) {
                                bytecode.push_back(res & 0xFF);
                            } else {
                                throw std::invalid_argument(
                                    std::format("[ASSEMBLER] Line {}: Invalid 8-bit signed integer {}",
                                        line_count, arg));
                            }
                        } else {
                            throw std::invalid_argument(
                                std::format("[ASSEMBLER] Line {}: Invalid integer {}",
                                    line_count, arg));
                        }
                    }
                }
                break;
            case ASM_TYPU8:
                {
                    for (const auto& arg : arguments) {
                        auto try_as_int = int_literal(arg);
                        if (try_as_int.has_value()) {
                            std::int32_t res = try_as_int.value();
                            if (static_cast<std::uint8_t>(res) == res) {
                                bytecode.push_back(res & 0xFF);
                            } else {
                                throw std::invalid_argument(
                                    std::format("[ASSEMBLER] Line {}: Invalid 8-bit unsigned integer {}",
                                        line_count, arg));
                            }
                        } else {
                            throw std::invalid_argument(
                                std::format("[ASSEMBLER] Line {}: Invalid integer {}",
                                    line_count, arg));
                        }
                    }
                }
                break;
            default:
                {
                    throw std::logic_error(
                        std::format("[ASSEMBLER] Line {}: Unknown assembler directive {}",
                            line_count, command));
                }
                break;
        }
}

void assemble(
    std::string input,
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::string, std::uint16_t>& addresses,
    std::unordered_map<std::uint16_t, std::string>& references)
{
    // Split by newline
    auto stmt_list = split(input, "\n");

    // Delete leading and trailing whitespace for each
    std::transform(stmt_list.begin(), stmt_list.end(), stmt_list.begin(), trim);

    std::size_t line_count = 0;

    for (const auto& stmt : stmt_list) {
        ++line_count;
        if (stmt.empty() || stmt.at(0) == ';') continue;

        // Get assembler directive or instruction
        const auto space_pos = stmt.find_first_of(" ");
        std::string command = stmt.substr(0, space_pos);

        // Ignore case
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);

        // Check if it exists
        auto token_info = TOKENS.find(command);
        if (token_info == TOKENS.end()) {
            throw std::invalid_argument(
                std::format("[ASSEMBLER] Line {}: Invalid directive/instruction {}", line_count, command));
        }
        auto& [token, type] = (token_info->second);

        // Get arguments
        std::string stmt_arg = (space_pos != std::string::npos) ? stmt.substr(space_pos + 1) : "";
        std::vector<std::string> arguments;

        // Special exception for str directive
        if (token == ASM_TYPSTR) {
            arguments.push_back(stmt_arg.substr(stmt_arg.find_first_of("/") + 1, stmt_arg.find_last_of("/")));
        } else {
            arguments = split(stmt_arg.substr(0, stmt_arg.find_first_of(";")), ",");
        }

        // Clean each arguments
        std::transform(arguments.begin(), arguments.end(), arguments.begin(), trim);
        arguments.erase(
            std::remove_if(arguments.begin(), arguments.end(),
                [](const std::string& str)
                {
                    return str.empty();
                }),
            arguments.end());

        // Check argument count
        if (((type != INF_INT) && (type & 0b11) != arguments.size()) ||
            ((type == INF_INT) && (arguments.size() < 1))) {
            throw std::invalid_argument(std::format("[ASSEMBLER] Line {}: Invalid argument count for {}", line_count, command));
        }

        // Assemble!

        // CPU instruction
        if (BEGIN_INS <= token && token < BEGIN_ASM) {
            assemble_instruction(bytecode, references, arguments, token, type, line_count);
        }
        // Assembler directive
        else if (BEGIN_ASM <= token && token < END) {
            assemble_command(bytecode, addresses, arguments, command, token, line_count);
        }
        else {
            throw std::logic_error(std::format("[ASSEMBLER] Line {}: Invalid directive/instruction {}", line_count, command));
        }
    }
    return;
}

std::size_t fsize(std::ifstream& file)
{
    std::size_t original_pos = file.tellg();

    file.seekg(0, std::ios::end);
    std::size_t file_size = file.tellg();

    file.seekg(original_pos);
    return file_size;
}
std::size_t fsize(std::fstream& file)
{
    std::size_t original_pos = file.tellg();

    file.seekg(0, std::ios::end);
    std::size_t file_size = file.tellg();

    file.seekg(original_pos);
    return file_size;
}

void write_addresses(
    const std::unordered_map<std::string, std::uint16_t>& addresses,
    std::ofstream& addr_file)
{
    for (const auto& address: addresses) {
        addr_file.write(address.first.data(), address.first.length() + 1);
        addr_file.write(reinterpret_cast<const char*>(&address.second), sizeof(address.second));
    }
}

void write_references(
    const std::unordered_map<std::uint16_t, std::string>& references,
    std::ofstream& ref_file)
{
    for (const auto& ref: references) {
        ref_file.write(reinterpret_cast<const char*>(&ref.first), sizeof(ref.first));
        ref_file.write(ref.second.data(), ref.second.length() + 1);
    }
}

void link_info(
    const std::unordered_map<std::string, std::uint16_t>& addresses,
    const std::unordered_map<std::uint16_t, std::string>& references)
{
    std::clog << "[LINKER] Addresses:" << std::endl;
    for (const auto& address: addresses) {
        std::clog << std::format("\t0x{:04X}: {}", address.second, address.first) << std::endl;
    }
    std::clog << "[LINKER] References:" << std::endl;
    for (const auto& ref: references) {
        std::clog << std::format("\t0x{:04X}: {}", ref.first, ref.second) << std::endl;
    }
}

void assemble(
    std::ifstream& asm_file,
    std::ofstream& code_file,
    std::ofstream& addr_file,
    std::ofstream& ref_file)
{
    std::size_t length = fsize(asm_file);

    std::string input(length, 0);
    std::vector<std::uint8_t> output;

    std::unordered_map<std::string, std::uint16_t> addresses;
    std::unordered_map<std::uint16_t, std::string> references;

    asm_file.read(input.data(), length);

    assemble(input, output, addresses, references);

    code_file.write(reinterpret_cast<const char*>(output.data()), output.size());
    write_addresses(addresses, addr_file);
    write_references(references, ref_file);
}

void read_addresses(
    std::ifstream& addr_file,
    std::unordered_map<std::string, std::uint16_t>& addresses)
{
    while (addr_file.peek() != std::char_traits<char>::eof()) {
        std::string name;
        std::uint16_t addr;
        std::getline(addr_file, name, '\0');
        addr_file.read(reinterpret_cast<char*>(&addr), sizeof(addr));
        addresses[name] = addr;
    }
}

void read_references(
    std::ifstream& ref_file,
    std::unordered_map<std::uint16_t, std::string>& references)
{
    while (ref_file.peek() != std::char_traits<char>::eof()) {
        std::string name;
        std::uint16_t addr;
        ref_file.read(reinterpret_cast<char*>(&addr), sizeof(addr));
        std::getline(ref_file, name, '\0');
        references[addr] = name;
    }
}

void link(
    std::vector<std::uint8_t>& bytecode,
    std::unordered_map<std::string, std::uint16_t>& addresses,
    std::unordered_map<std::uint16_t, std::string>& references)
{
    link_info(addresses, references);
    for (const auto& [addr, name] : references) {

        auto location = addresses.find(name);
        if (location == addresses.end()) {
            throw std::logic_error(std::format("[LINKER] 0x{:04X}: Undefined reference to {}", addr, name));
        }

        auto location_addr = location->second;

        // Replace placeholder locations
        bytecode.at(addr + 2) = (location_addr & 0xFF);
        bytecode.at(addr + 3) = ((location_addr >> 8) & 0xFF);
    }
}

void link(
    std::ifstream& code_file,
    std::ifstream& addr_file,
    std::ifstream& ref_file,
    std::ofstream& bin_file)
{
    std::size_t length = fsize(code_file);

    std::vector<std::uint8_t> bytecode(length);
    std::unordered_map<std::string, std::uint16_t> addresses;
    std::unordered_map<std::uint16_t, std::string> references;

    code_file.read(reinterpret_cast<char*>(bytecode.data()), length);
    read_addresses(addr_file, addresses);
    read_references(ref_file, references);

    link(bytecode, addresses, references);

    bin_file.write(reinterpret_cast<const char*>(bytecode.data()), bytecode.size());
}

void link(
    const std::vector<std::tuple<
        std::vector<std::uint8_t>,
        std::unordered_map<std::string, std::uint16_t>,
        std::unordered_map<std::uint16_t, std::string>>>& objects,
    std::vector<std::uint8_t>& output_bytecode,
    std::unordered_map<std::string, std::uint16_t>& output_addr,
    std::unordered_map<std::uint16_t, std::string>& output_ref)
{
    std::size_t addr_count = 0;
    std::vector<std::uint16_t> relocation_offsets(objects.size() + 1);

    relocation_offsets.push_back(0);

    for (const auto& [bytecode, addrs, refs] : objects) {
        // Concatenate addresses with relocation
        output_addr.insert(addrs.begin(), addrs.end());
        for (const auto& [name, addr] : addrs) {
            output_addr.at(name) += relocation_offsets.back();
        }

        // Concatenate references with relocation
        for (const auto& [addr, name]: refs) {
            output_ref[addr + relocation_offsets.back()] = name;
        }

        // Concatenate code
        output_bytecode.insert(output_bytecode.end(), bytecode.begin(), bytecode.end());

        // Setup next relocation offset
        addr_count += addrs.size();
        relocation_offsets.push_back(relocation_offsets.back() + bytecode.size());
    }

    // Duplicate addresses
    if (output_addr.size() != addr_count) {
        throw std::invalid_argument("[LINKER] Duplicate location names");
    }

    // Link all
    link(output_bytecode, output_addr, output_ref);
}

void link(
    std::vector<std::tuple<std::ifstream, std::ifstream, std::ifstream>>& object_files,
    std::ofstream& output_addr_file,
    std::ofstream& output_ref_file,
    std::ofstream& output_bin_file)
{
    std::vector<std::tuple<
        std::vector<std::uint8_t>,
        std::unordered_map<std::string, std::uint16_t>,
        std::unordered_map<std::uint16_t, std::string>>> objects;

    std::vector<std::uint8_t> output_bytecode;
    std::unordered_map<std::string, std::uint16_t> output_addr;
    std::unordered_map<std::uint16_t, std::string> output_ref;

    std::vector<std::size_t> lengths(object_files.size());

    for (auto& [code_file, addr_file, ref_file] : object_files) {
        lengths.push_back(fsize(code_file));
        objects.push_back({std::vector<std::uint8_t>(lengths.back()), {}, {}});

        code_file.read(reinterpret_cast<char*>(std::get<0>(objects.back()).data()), lengths.back());
        read_addresses(addr_file, std::get<1>(objects.back()));
        read_references(ref_file, std::get<2>(objects.back()));
    }

    link(objects, output_bytecode, output_addr, output_ref);

    output_bin_file.write(reinterpret_cast<const char*>(output_bytecode.data()), output_bytecode.size());
    write_addresses(output_addr, output_addr_file);
    write_references(output_ref, output_ref_file);
}

}