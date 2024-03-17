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

#define SDL_MAIN_HANDLED

#include <cstdlib>

#include <chrono>
#include <iostream>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <tuple>

#include <cosmovm/bus.hpp>
#include <cosmovm/clock.hpp>
#include <cosmovm/cpu.hpp>
#include <cosmovm/disk.hpp>
#include <cosmovm/display.hpp>
#include <cosmovm/keyboard.hpp>
#include <cosmovm/memory.hpp>

#include "assembler.hpp"

constexpr std::size_t TARGET_CPU_FREQ = 1000000; // 1MHz
constexpr std::size_t TARGET_RENDER_FREQ = 60; // 60Hz

void build(const std::string& outputpath, const std::string& inputpath)
{
    {
        std::cout << std::format("[BUILDER] Assembling {}...", inputpath) << std::endl;
        std::ifstream asm_file {inputpath, std::ios::in};
        std::ofstream code_file{std::format("{}.code" , outputpath), std::ios::binary | std::ios::out};
        std::ofstream addr_file{std::format("{}.addr", outputpath), std::ios::binary | std::ios::out};
        std::ofstream ref_file {std::format("{}.ref" , outputpath), std::ios::binary | std::ios::out};

        if (!asm_file.is_open()) {
            throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}", inputpath));
        }

        cosmoasm::assemble(asm_file, code_file, addr_file, ref_file);
    }
    {

        std::cout << std::format("[BUILDER] Linking {}...", inputpath) << std::endl;
        std::ifstream code_file{std::format("{}.code" , outputpath), std::ios::binary | std::ios::in};
        std::ifstream addr_file{std::format("{}.addr", outputpath), std::ios::binary | std::ios::in};
        std::ifstream ref_file {std::format("{}.ref" , outputpath), std::ios::binary | std::ios::in};
        std::ofstream bin_file {std::format("{}.bin" , outputpath), std::ios::binary | std::ios::out};

        if (!code_file.is_open()) {
            throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}.code", outputpath));
        }
        if (!addr_file.is_open()) {
            throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}.addr", outputpath));
        }
        if (!ref_file.is_open()) {
            throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}.ref", outputpath));
        }

        cosmoasm::link(code_file, addr_file, ref_file, bin_file);
    }
}

void build(const std::string& outputpath, const std::vector<std::string>& inputpaths)
{
    {
        std::cout << "[BUILDER] Assembling..." << std::endl;
        for (const auto& inputpath : inputpaths) {
            std::cout << std::format("\t{}", inputpath) << std::endl;

            std::string outputpath_prefix = std::filesystem::path(inputpath).filename().string();
            std::ifstream asm_file {inputpath, std::ios::in};
            std::ofstream code_file{std::format("{}.code" , outputpath_prefix), std::ios::binary | std::ios::out};
            std::ofstream addr_file{std::format("{}.addr", outputpath_prefix) , std::ios::binary | std::ios::out};
            std::ofstream ref_file {std::format("{}.ref" , outputpath_prefix) , std::ios::binary | std::ios::out};

            if (!asm_file.is_open()) {
                throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}", inputpath));
            }

            cosmoasm::assemble(asm_file, code_file, addr_file, ref_file);
        }
    }
    {
        std::cout << "[BUILDER] Linking:" << std::endl;
        for (const auto& inputpath : inputpaths) {
            std::cout << std::format("\t{}", inputpath) << std::endl;
        }

        std::vector<std::tuple<std::ifstream, std::ifstream, std::ifstream>> object_files;

        for (const auto& inputpath : inputpaths) {
            std::string outputpath_prefix = std::filesystem::path(inputpath).filename().string();

            object_files.push_back(std::make_tuple(
                std::ifstream{std::format("{}.code" , outputpath_prefix), std::ios::binary | std::ios::in},
                std::ifstream{std::format("{}.addr", outputpath_prefix) , std::ios::binary | std::ios::in},
                std::ifstream{std::format("{}.ref" , outputpath_prefix) , std::ios::binary | std::ios::in}));

            if (!std::get<0>(object_files.back()).is_open()) {
            throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}.code", outputpath_prefix));
            }
            if (!std::get<1>(object_files.back()).is_open()) {
                throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}.addr", outputpath_prefix));
            }
            if (!std::get<2>(object_files.back()).is_open()) {
                throw std::invalid_argument(std::format("[BUILDER] Couldn't open {}.ref", outputpath_prefix));
            }
        }

        std::ofstream addr_file{std::format("{}.addr", outputpath), std::ios::binary | std::ios::out};
        std::ofstream ref_file {std::format("{}.ref" , outputpath), std::ios::binary | std::ios::out};
        std::ofstream bin_file {std::format("{}.bin" , outputpath), std::ios::binary | std::ios::out};

        cosmoasm::link(object_files, addr_file, ref_file, bin_file);
    }
}

void run(const std::string& disk_path)
{
    std::cout << std::format("[EMULATOR] Booting from {}...", disk_path) << std::endl;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();

    // Overwrite disk

    // Prepare disk
    std::vector<std::uint8_t> boot(cosmovm::SECTOR_SIZE, 0);
    {
        std::ifstream disk_file{disk_path, std::ios::binary | std::ios::in};
        if (!disk_file.is_open()) {
            throw std::invalid_argument(std::format("[EMULATOR] Couldn't open {}", disk_path));
        }
        if (cosmoasm::fsize(disk_file) < cosmovm::SECTOR_SIZE) {
            throw std::invalid_argument(std::format("[EMULATOR] Invalid boot disk {}", disk_path));
        }
        disk_file.read(reinterpret_cast<char*>(boot.data()), cosmovm::SECTOR_SIZE);
    }

    // Initialize emulator components
    std::shared_ptr<cosmovm::memory> cmem = std::make_shared<cosmovm::memory>(0, boot, cosmovm::SECTOR_SIZE);
    std::shared_ptr<cosmovm::bus> cbus = std::make_shared<cosmovm::bus>(cmem);
    std::unique_ptr<cosmovm::clock> cclk = std::make_unique<cosmovm::clock>(cbus);
    std::unique_ptr<cosmovm::cpu> ccpu = std::make_unique<cosmovm::cpu>(cbus);
    std::unique_ptr<cosmovm::disk> cdsk = std::make_unique<cosmovm::disk>(cbus, disk_path);
    std::unique_ptr<cosmovm::display> cscr = std::make_unique<cosmovm::display>(cbus, "CosmoVM");
    std::unique_ptr<cosmovm::keyboard> ckb = std::make_unique<cosmovm::keyboard>(cbus);

    // Run
    std::size_t cycles_to_execute = TARGET_CPU_FREQ / TARGET_RENDER_FREQ;
    double sleep_time = 0;

    while (cscr->window_is_open() && !ccpu->shutdown_flag_set())
    {
        // RUNNING
        // Execute instructions
        auto start_cpu_time = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < std::max(cycles_to_execute, static_cast<std::size_t>(1)); i++) ccpu->run();
        auto end_cpu_time = std::chrono::high_resolution_clock::now();

        // Render
        auto start_render_time = std::chrono::high_resolution_clock::now();
        cscr->run();
        auto end_render_time = std::chrono::high_resolution_clock::now();

        // TIMING
        auto cpu_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_cpu_time - start_cpu_time).count();
        auto render_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_render_time - start_render_time).count();

        if ((1.F / TARGET_RENDER_FREQ) >= (cpu_time + render_time))
            sleep_time = (1.F / TARGET_RENDER_FREQ) - (cpu_time + render_time);
        else sleep_time = 0;

        // Seconds -> Milliseconds
        SDL_Delay(sleep_time * 1000);
    }

    std::cout << "[EMULATOR] Shutting down..." << std::endl;

    if (ccpu->shutdown_flag_set() && ccpu->exception_flag_set()) {
        std::cout << std::format("[EMULATOR] Exception flag set... dumping memory into {}...", cosmovm::DUMP_PATH) << std::endl;
        cmem->dump();
    }

    // Destroy SDL objects first then clean
    cscr.reset();

    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv, argv + argc);

    try {
        if (argc == 1) {
            std::cout <<
                "CosmoVM an emulator and assembler for an imaginary cpu\n"
                "Licensed under GPL-3.0, (see https://www.gnu.org/licenses/)"
                << std::endl;
            std::cout << std::format("\tUsage: {} [DISK_PATH]", args.at(0)) << std::endl;
            std::cout << std::format("\tUsage: {} [OUTPUT_PREFIX] [INPUT_ASM]", args.at(0)) << std::endl;
            std::cout << std::format("\tUsage: {} [OUTPUT_PREFIX] [INPUT_ASM1] [INPUT_ASM2]...", args.at(0)) << std::endl;
        } else if(argc == 2) {
            run(args.at(1));
        } else if (argc == 3) {
            build(args.at(1), args.at(2));
        } else {
            build(args.at(1), std::vector<std::string>(argv + 2, argv + argc));
        }
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::exit(EXIT_SUCCESS);
}