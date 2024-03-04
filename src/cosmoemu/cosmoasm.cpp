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

#include "cosmoasm.hpp"

std::vector<std::string> cosmoemu::_tokenize(const std::string& _str, std::string delimiter)
{
    std::string str{_str};
    std::size_t pos{0};
    std::vector<std::string> tokens{};
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str.substr(0, pos));
    return tokens;
}

std::uint16_t cosmoemu::_convert_int_literal(const std::string& _str)
{
    std::string str{_str};
    std::uint8_t integer_base{10};
    if (str.find("0X", 0) == 0)
    {
        str = str.substr(2, str.length());
        integer_base = 16;
    }
    else if (str.find("0B", 0) == 0)
    {
        str = str.substr(2, str.length());
        integer_base = 2;
    }
    return std::stoul(str, nullptr, integer_base);
}

void cosmoemu::_replace_substring(std::string& str, const std::string& old, const std::string& new_str)
{
    std::size_t index{0};
    while ((index = str.find(old)) != std::string::npos)
        str.replace(index, old.length(), new_str);
}

void cosmoemu::assemble(std::ifstream &file_in, std::ofstream &file_out)
{
    std::size_t line_count{0};
    std::uint16_t binary_offset{0};
    std::unordered_map<std::string, std::uint16_t>locations{};
    std::vector<location> to_be_checked_locations{};

    while (!file_in.eof())
    {
        ++line_count;
        // Prepare line
        std::string original_line{};
        std::getline(file_in, original_line, '\n');
        std::string current_line{original_line};
        std::transform(current_line.begin(), current_line.end(), current_line.begin(), ::toupper);

        current_line = current_line.substr(0, current_line.find(prefix_comment, 0));
        if (current_line.length() == 0)
            continue;
		current_line.erase(
			current_line.begin(),
			std::find_if(current_line.begin(), current_line.end(), [](char const &c) { return (c != ' '); }));

        // Tokenize
        std::vector<std::string> tokens{_tokenize(current_line, " ")};

        // Assemble
        std::string instruction = tokens.at(0);

        // Define location
        if (instruction.at(0) == prefix_def_loc)
        {
            std::string loc_name{instruction.substr(1, instruction.length())};
            if (std::find_if(
                loc_name.begin(),
                loc_name.end(),
                [](char const &c) { return (!isalnum(c)) && (c != '_'); }) != loc_name.end())
            {
                throw std::invalid_argument("[COSMOASM] Location tag isn't valid " + std::to_string(line_count));
            }

            if (locations.find(loc_name) != locations.end())
                throw std::invalid_argument("[COSMOASM] Repeated name" + std::to_string(line_count));

            locations[instruction.substr(1, instruction.length())] = binary_offset;
        }

        // Write bytes
        else if (instruction == prefix_wb)
        {
            std::string data_type{tokens.at(1)};
            if (data_type == "INT8")
            {
                std::string integer{tokens.at(2)};
                std::uint8_t data_int{static_cast<std::uint8_t>(_convert_int_literal(integer) & 0xFF)};
                file_out.write(reinterpret_cast<char*>(&data_int), 1);
                binary_offset += 1;
            }
            else if (data_type == "INT16")
            {
                std::string integer{tokens.at(2)};
                std::uint16_t data_int{_convert_int_literal(integer)};
                file_out.write(reinterpret_cast<char*>(&data_int), 2);
                binary_offset += 2;
            }
            else if (data_type == "ARRAY8")
            {
                // Detokenize
                std::string array{};
                for (std::size_t count = 2; count < tokens.size(); count++)
                    array.append(tokens[count]);
                std::vector<std::string> items{_tokenize(array, ",")};
                for (const auto& item : items)
                {
                    std::uint8_t data_int{static_cast<std::uint8_t>(_convert_int_literal(item) & 0xFF)};
                    file_out.write(reinterpret_cast<char*>(&data_int), 1);
                    binary_offset += 1;
                }
            }
            else if (data_type == "ARRAY16")
            {
                // Detokenize
                std::string array{};
                for (std::size_t count = 2; count < tokens.size(); count++)
                    array.append(tokens[count]);
                std::vector<std::string> items{_tokenize(array, ",")};
                for (const auto& item : items)
                {
                    std::uint16_t data_int{_convert_int_literal(item)};
                    file_out.write(reinterpret_cast<char*>(&data_int), 2);
                    binary_offset += 2;
                }
            }
            else if (data_type == "STR")
            {
                std::string data_str{original_line};

                data_str = data_str.substr(data_str.find("\"", 0) + 1, data_str.length());
                data_str = data_str.substr(0, data_str.rfind("\""));
                bool is_null_terminated = (data_str.find("\\0") != std::string::npos);
                _replace_substring(data_str, "\\n", "\n");
                _replace_substring(data_str, "\\0", "\0");
                file_out.write(data_str.c_str(), data_str.size() + is_null_terminated);
                binary_offset += data_str.size() + 1;
            }
            else
            {
                throw std::invalid_argument("[COSMOASM] Unknown data type " + data_type);
            }
        }
        else if (instructions.find(instruction) != instructions.end())
        {
            std::uint32_t opcode{instructions.at(instruction).opcode};
            std::uint8_t arg_count{instructions.at(instruction).arg_count};
            if (arg_count)
            {
                std::size_t count = 0;
                for (; count < arg_count; count++)
                {
                    std::string argument = tokens.at(count + 1);
                    if (argument.at(0) == prefix_imm)
                    {
                        std::uint32_t args_bin = _convert_int_literal(argument.substr(1, argument.length() - 1));
                        opcode = (args_bin << 16) | opcode;
                    }
                    else if (argument.at(0) == prefix_loc)
                    {
                        std::string loc_name{argument.substr(1, argument.length() - 1)};
                        if (locations.find(loc_name) == locations.end())
                        {
                            to_be_checked_locations.push_back(
								{loc_name, static_cast<std::uint16_t>(file_out.tellp()) + 1 + count, line_count});
                        }
                        else
                        {
                            std::uint32_t args_bin = locations.at(loc_name);
							opcode = (args_bin << (8 + (8 * count))) | opcode;
                        }
                    }
                    else if (argument.at(0) == prefix_reg)
                    {
                        std::uint32_t args_bin = registers.at(argument.substr(1, argument.length() - 1));
                        opcode = (args_bin << (8 * (count + 1))) | opcode;
                    }
                    else
                    {
                        throw std::invalid_argument("[COSMOASM] Unknown operand " + argument);
                    }
                }
            }
            file_out.write(reinterpret_cast<const char*>(&opcode), 4);
            binary_offset += 4;
        }
        else
        {
            throw std::invalid_argument("[COSMOASM] Unknown instruction " + instruction + " " + std::to_string(line_count));
        }
    }
    for (const auto& location : to_be_checked_locations)
    {
        if (locations.find(location.name) == locations.end())
            throw std::invalid_argument("[COSMOASM] Unknown location tag " + std::to_string(location.line));
        else
        {
            file_out.seekp(location.pos);
            file_out.write(reinterpret_cast<const char*>(&locations.at(location.name)), 2);
        }
    }
}