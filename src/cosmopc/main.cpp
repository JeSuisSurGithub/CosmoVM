#include "cosmopc_version.hpp"
#include "cosmoasm.hpp"

#define SDL_MAIN_HANDLED

#include <cosmocore/cosmobus.hpp>
#include <cosmocore/cosmoclk.hpp>
#include <cosmocore/cosmocpu.hpp>
#include <cosmocore/cosmodsk.hpp>
#include <cosmocore/cosmokb.hpp>
#include <cosmocore/cosmomem.hpp>
#include <cosmocore/cosmoscr.hpp>

#include <cstdlib>

#include <chrono>
#include <iostream>
#include <memory>

constexpr std::size_t target_cpu_frequency = 1000000; // 1MHz
constexpr std::size_t target_render_frequency = 60; // 60Hz

std::size_t get_file_handle_size(std::ifstream& file)
{
    std::size_t original_pos = file.tellg();

    file.seekg(0, std::ios::end);
    std::size_t file_size = file.tellg();

    file.seekg(original_pos);
    return file_size;
}

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        // Assemble
        std::cout << "Assembling " << argv[1] << " into " << argv[2] << "..." << std::endl;
        std::ifstream file_in{argv[1], std::ios::in};
        std::ofstream file_out{argv[2], std::ios::out | std::ios::binary};

        if (!file_in.is_open())
        {
            std::cout << "Couldn't access " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
        if (!file_out.is_open())
        {
            std::cout << "Couldn't access " << argv[2] << std::endl;
            return EXIT_FAILURE;
        }
        cosmopc::assemble(file_in, file_out);
        file_in.close();
        file_out.close();
        std::cout << "Assembled " << argv[1] << " into " << argv[2] << "..." << std::endl;
        return EXIT_SUCCESS;
    }
    else if(argc == 2)
    {
        std::cout << "Booting from " << argv[1] << "..." << std::endl;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
        TTF_Init();

        // Read binary
        std::ifstream boot_file{argv[1], std::ios::in | std::ios::binary};
        std::size_t file_size = get_file_handle_size(boot_file);
        std::vector<std::uint8_t> buf(file_size);
        boot_file.read(reinterpret_cast<char*>(buf.data()), file_size);

        // Initialize emulator components
        std::shared_ptr<cosmocore::cosmomem> cmem = std::make_shared<cosmocore::cosmomem>(0, buf, file_size);
        std::shared_ptr<cosmocore::cosmobus> cbus = std::make_shared<cosmocore::cosmobus>(cmem);
        std::unique_ptr<cosmocore::cosmocpu> ccpu = std::make_unique<cosmocore::cosmocpu>(cbus);
        std::unique_ptr<cosmocore::cosmodsk> cdsk = std::make_unique<cosmocore::cosmodsk>(cbus);
        std::unique_ptr<cosmocore::cosmoclk> cclk = std::make_unique<cosmocore::cosmoclk>(cbus);
        std::unique_ptr<cosmocore::cosmoscr> cscr = std::make_unique<cosmocore::cosmoscr>(cbus, "COSMOPC");
        std::unique_ptr<cosmocore::cosmokb> ckb = std::make_unique<cosmocore::cosmokb>(cbus);

        // Run
        std::size_t cycles_to_execute = target_cpu_frequency / target_render_frequency;
        double sleep_time{0};
        while (cscr->window_is_open())
        {
            // RUNNING
            // Execute instructions
            auto start_cpu_time = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < cycles_to_execute + 1; i++) ccpu->run();
            auto end_cpu_time = std::chrono::high_resolution_clock::now();

            // Render
            auto start_render_time = std::chrono::high_resolution_clock::now();
            cscr->run();
            auto end_render_time = std::chrono::high_resolution_clock::now();

            // TIMING
            auto cpu_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_cpu_time - start_cpu_time).count();
            auto render_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_render_time - start_render_time).count();

            if ((1.F / target_render_frequency) >= (cpu_time + render_time))
                sleep_time = (1.F / target_render_frequency) - (cpu_time + render_time);
            else sleep_time = 0;

            // Seconds -> Milliseconds
            SDL_Delay(sleep_time * 1000);
        }

        // Destroy SDL objects first then clean
        cscr.reset();
        TTF_Quit();
        SDL_Quit();
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "Usage: " << argv[0] << " [ASM_PATH] [OUTPUT_PATH]\n";
        std::cout << "Usage: " << argv[0] << " [BINARY_PATH]\n";
    }
}