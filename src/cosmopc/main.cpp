#include "cosmopc_version.hpp"
#include "cosmoasm.hpp"

#include <cosmocore/cosmoclk.hpp>
#include <cosmocore/cosmocpu.hpp>
#include <cosmocore/cosmobus.hpp>
#include <cosmocore/cosmokb.hpp>
#include <cosmocore/cosmomem.hpp>
#include <cosmocore/cosmoscr.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>

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
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << "[ASM_PATH] [OUTPUT_PATH]\n";
        return EXIT_FAILURE;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();

    // Assemble
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

    // Read binary
    std::ifstream boot_file{argv[2], std::ios::in | std::ios::binary};
    std::size_t file_size = get_file_handle_size(boot_file);
    std::vector<std::uint8_t> buf(file_size);
    boot_file.read(reinterpret_cast<char*>(buf.data()), file_size);

    // Initialize emulator components
    std::shared_ptr<cosmocore::cosmomem> cmem = std::make_shared<cosmocore::cosmomem>(0, buf, file_size);
    std::shared_ptr<cosmocore::cosmobus> cbus = std::make_shared<cosmocore::cosmobus>(cmem);
    std::shared_ptr<cosmocore::cosmocpu> ccpu = std::make_shared<cosmocore::cosmocpu>(cbus);
    std::shared_ptr<cosmocore::cosmoclk> cclk = std::make_shared<cosmocore::cosmoclk>(cbus);
    std::shared_ptr<cosmocore::cosmoscr> cscr = std::make_shared<cosmocore::cosmoscr>(cbus, "COSMOPC");
    std::shared_ptr<cosmocore::cosmokb> ckb = std::make_shared<cosmocore::cosmokb>(cbus);

    // Run
    while (cscr->window_is_open())
    {
        // Execute one instruction
        ccpu->run();

        // Update
        cscr->run();

        // 60Hz cpu
        SDL_Delay(16);
    }

    // Destroy SDL objects first then clean
    cscr.reset();
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}