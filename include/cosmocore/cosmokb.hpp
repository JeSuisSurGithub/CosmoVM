#ifndef COSMOKB_HPP
#define COSMOKB_HPP

#include "cosmocore.hpp"
#include "cosmobus.hpp"

#include <unordered_map>

namespace cosmocore
{
    // Lazy implementation using directly SDL Scancodes
    class cosmokb
    {
        private: u16i m_key_selector;
        private: const u8i* m_sdl_kb_state;
        private: std::shared_ptr<cosmobus>& m_bus;

        public: cosmokb() = delete;
        public: cosmokb(std::shared_ptr<cosmobus>& bus);
        public: ~cosmokb();

        public: u16i set_key_selector(u16i key_selector);
        public: u16i get_requested_key(u16i dummy);
    };
}

#endif /* COSMOKB_HPP */