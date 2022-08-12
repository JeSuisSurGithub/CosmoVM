#ifndef COSMOASM_HPP
#define COSMOASM_HPP

#include <cstdint>

#include <algorithm>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace cosmopc
{
    typedef struct _instruction
    {
        std::uint32_t opcode;
        std::uint8_t arg_count;
    }instruction;

    typedef struct _location
    {
        std::string name;
        std::uint16_t pos;
        std::size_t line;
    }location;

    const char prefix_comment = ';';
    const char prefix_def_loc = '.';
    const char prefix_imm = '$';
    const char prefix_loc = '&';
    const char prefix_reg = '%';
    const std::string prefix_wb = "WB";

    const std::unordered_map<std::string, std::uint8_t> registers =
    {
        {"AZ", 0x01},
        {"BZ", 0x02},
        {"CZ", 0x03},
        {"DZ", 0x04},
        {"EZ", 0x05},
        {"FZ", 0x06},
        {"GZ", 0x07},
        {"HZ", 0x08},
        {"IZ", 0x09},
        {"JZ", 0x0A},
        {"WZ", 0x0B},
        {"XZ", 0x0C},
        {"YZ", 0x0D},
        {"ZZ", 0x0E}
    };

    const std::unordered_map<std::string, instruction> instructions =
    {
        {"WCYL",    {0x00, 0}},
        {"ADD",     {0x01, 2}},
        {"ADDI",    {0x02, 2}},
        {"AND",     {0x03, 2}},
        {"ANDI",    {0x04, 2}},
        {"CALL",    {0x05, 1}},
        {"CLE",     {0x06, 0}},
        //{"CLI",     {0x07, 0}},
        {"CLXP",    {0x08, 0}},
        {"CMP",     {0x09, 2}},
        {"CMPI",    {0x0A, 2}},
        {"DEC",     {0x0B, 1}},
        {"DIV",     {0x0C, 2}},
        {"DIVI",    {0x0D, 2}},
        {"IN",      {0x0E, 2}},
        //{"INB",     {0x0F, 2}},

        {"INC",     {0x10, 1}},
        {"JE",      {0x11, 1}},
        {"JER",     {0x12, 1}},
        {"JG",      {0x13, 1}},
        {"JGE",     {0x14, 1}},
        {"JLE",     {0x15, 1}},
        {"JMP",     {0x16, 1}},
        {"JNE",     {0x17, 1}},
        {"JNER",    {0x18, 1}},
        {"JNXP",    {0x19, 1}},
        {"JXP",     {0x1A, 1}},
        {"LOP",     {0x1B, 1}},
        {"LOPE",    {0x1C, 1}},
        {"LOPNE",   {0x1D, 1}},
        {"MOV",     {0x1E, 2}},
        {"MOVRM",   {0x1F, 2}},

        {"MOVRI",   {0x20, 2}},
        {"MOVMR",   {0x21, 2}},
        {"MOVMM",   {0x22, 2}},
        {"MOVMI",   {0x23, 2}},
        {"MOVBMR",  {0x24, 2}},
        {"MOVBMM",  {0x25, 2}},
        {"MOVBMI",  {0x26, 2}},
        {"MUL",     {0x27, 2}},
        {"MULI",    {0x28, 2}},
        {"NEG",     {0x29, 1}},
        {"NOT",     {0x2A, 1}},
        {"OR",      {0x2B, 2}},
        {"ORI",     {0x2C, 2}},
        {"OUT",     {0x2D, 2}},
        //{"OUTB",    {0x2E, 2}},
        {"POP",     {0x2F, 1}},

        {"POPF",    {0x30, 0}},
        {"PUSH",    {0x31, 1}},
        {"PUSHF",   {0x32, 0}},
        {"RET",     {0x33, 0}},
        {"SHL",     {0x34, 2}},
        {"SHR",     {0x35, 2}},
        {"STE",     {0x36, 0}},
        //{"STI",     {0x37, 0}},
        {"STXP",    {0x38, 0}},
        {"STRS",    {0x39, 0}},
        {"SUB",     {0x3A, 2}},
        {"SUBI",    {0x3B, 2}},
        {"XOR",     {0x3C, 2}},
        {"XORI",    {0x3D, 2}},
        {"MOVBRM",  {0x3E, 2}},
        {"SHLI",    {0x3F, 2}},
        {"SHRI",    {0x40, 2}},

        {"STSD",    {0x41, 0}}
    };

    std::vector<std::string> _tokenize(const std::string& str, std::string delimiter);

    std::uint16_t _convert_int_literal(const std::string& str);

    void _replace_substring(std::string& str, const std::string& old, const std::string& new_str);

    void assemble(std::ifstream& file_in, std::ofstream& file_out);
}

#endif /* COSMOASM_HPP */