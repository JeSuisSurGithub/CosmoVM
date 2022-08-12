#include <cosmocore/cosmoscr.hpp>

using namespace cosmocore;

cosmoscr::cosmoscr(std::shared_ptr<cosmobus>& bus, const std::string& window_title)
:
m_mode(VIDEO_MODES::TEXT),
m_bus(bus)
{
    SDL_CreateWindowAndRenderer(
        WINDOW_W,
        WINDOW_H,
        0,
        &m_window,
        &m_renderer);
    SDL_SetWindowTitle(m_window, window_title.c_str());
    m_color = {0xDF, 0xDF, 0xDF, 0};

    if((m_font = TTF_OpenFont(FONT_PATH.c_str(), 8)) == NULL)
        throw std::invalid_argument("[COSMOSCR] Couldn't find " + FONT_PATH);
    m_video_mem_buf = reinterpret_cast<const char*>(m_bus->get_memory()->get_buf().data()) + VIDEO_START_ADDR;

    // Arbitrary port
    m_bus->bind_port(0x44, std::bind(&cosmoscr::change_mode, this, std::placeholders::_1));
}

cosmoscr::~cosmoscr()
{
    TTF_CloseFont(m_font);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void cosmoscr::run()
{
    switch (m_mode)
    {
        case VIDEO_MODES::TEXT:
            render_text_mode();
            break;
        case VIDEO_MODES::GRAPHIC:
            render_graphic_mode();
            break;
        default:
            break;
    }
}

bool cosmoscr::window_is_open()
{
    return !m_quit;
}

u16i cosmoscr::change_mode(u16i mode)
{
    if (mode == VIDEO_MODES::TEXT)
        m_mode = static_cast<VIDEO_MODES>(mode);
    else if (mode == VIDEO_MODES::GRAPHIC)
        m_mode = static_cast<VIDEO_MODES>(mode);
    else throw std::invalid_argument("[COSMOSCR] Unknown mode " + std::to_string(mode));
    // Return dummy
    return PORT_DUMMY_VALUE;
}

void cosmoscr::render_text_mode()
{
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Event event;

    if (*m_video_mem_buf != 0)
    {
        surface =
            TTF_RenderUTF8_Solid_Wrapped(
                m_font,
                m_video_mem_buf,
                m_color,
                WINDOW_W);
        texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        rect = {.x = 0, .y = 0, .w = surface->w, .h = static_cast<int>(surface->h * 2.125F)};
    }

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            m_quit = 1;
    }
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);
    if (*m_video_mem_buf != 0)
        SDL_RenderCopy(m_renderer, texture, NULL, &rect);
    SDL_RenderPresent(m_renderer);

    if (*m_video_mem_buf != 0)
    {
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void cosmoscr::render_graphic_mode()
{
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Event event;
    surface =
        SDL_CreateRGBSurfaceFrom(
            (void*)m_video_mem_buf,
            GFX_MODE_W,
            GFX_MODE_H,
            GFX_BIT_DEPTH,
            GFX_MODE_W,
            0b11100000,
            0b00011100,
            0b00000011,
            0b0);
    texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    rect = {.x = 0, .y = 0, .w = surface->w * (WINDOW_W / GFX_MODE_W), .h = surface->h * (WINDOW_H / GFX_MODE_H)};

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            m_quit = 1;
    }
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
    SDL_RenderPresent(m_renderer);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}