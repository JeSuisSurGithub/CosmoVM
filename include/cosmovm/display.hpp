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

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "common.hpp"
#include "bus.hpp"

namespace cosmovm
{
    constexpr std::string FONT_PATH = "vgafont.ttf";
    constexpr u16 VIDEO_START_ADDR = 0xB500;
    constexpr u16 VIDEO_MEM_SIZE   = 0x4B00;
    constexpr u16 TEXT_MODE_W = 80;
    constexpr u16 TEXT_MODE_H = 25;
    constexpr u16 GFX_MODE_W = 160;
    constexpr u16 GFX_MODE_H = 120;
    constexpr u16 WINDOW_W = 640;
    constexpr u16 WINDOW_H = 480;
    constexpr u16 GFX_BIT_DEPTH = 8;

    typedef enum VIDEO_MODES
    {
        TEXT = 0,       // 80x25 MONOCHROME
        GRAPHIC = 1,    // 320x240 8-bit color
    }VIDEO_MODES;

    class display
    {
        private:
            SDL_Window* m_window;
            SDL_Renderer* m_renderer;
            SDL_Color m_color;
            TTF_Font* m_font;

            std::shared_ptr<bus>& m_bus;
            std::array<u8, MEM_SIZE>::const_iterator m_video_mem_buf;
            bool m_quit;
            VIDEO_MODES m_mode;

        public:
            display(std::shared_ptr<bus>& bus, const std::string& window_title);
            display(const display&) = delete;
            display() = delete;
            ~display();

            void run();
            bool window_is_open();
            u16 change_mode(u16 mode);

        private:
            void render_text_mode();
            void render_graphic_mode();
    };
}

#endif /* DISPLAY_HPP */