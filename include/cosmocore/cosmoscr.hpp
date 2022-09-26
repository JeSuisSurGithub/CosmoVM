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

#ifndef COSMOSCR_HPP
#define COSMOSCR_HPP

#include "cosmocore.hpp"
#include "cosmobus.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <cstring>

#include <memory>
#include <stdexcept>

namespace cosmocore
{
    const std::string FONT_PATH = "vgafont.ttf";
    constexpr u16i VIDEO_START_ADDR = 0xC17F;
    constexpr std::size_t TEXT_MODE_W = 80;
    constexpr std::size_t TEXT_MODE_H = 25;
    constexpr std::size_t GFX_MODE_W = 320;
    constexpr std::size_t GFX_MODE_H = 240;
    constexpr std::size_t WINDOW_W = 640;
    constexpr std::size_t WINDOW_H = 480;
    constexpr std::size_t GFX_BIT_DEPTH = 8;

    typedef enum VIDEO_MODES
    {
        TEXT = 0,       // 80x25 MONOCHROME
        GRAPHIC = 1,    // 320x240 8-bit color
    }VIDEO_MODES;

    class cosmoscr
    {
        private: SDL_Window* m_window;
        private: SDL_Renderer* m_renderer;
        private: SDL_Color m_color;
        private: TTF_Font* m_font;

        private: const char* m_video_mem_buf;
        private: bool m_quit;
        private: VIDEO_MODES m_mode;
        private: std::shared_ptr<cosmobus>& m_bus;

        public: cosmoscr(std::shared_ptr<cosmobus>& bus, const std::string& window_title);
        public: ~cosmoscr();

        public: void run();
        public: bool window_is_open();
        public: u16i change_mode(u16i mode);

        private: void render_text_mode();
        private: void render_graphic_mode();
    };
}

#endif /* COSMOSCR_HPP */