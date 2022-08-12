#ifndef COSMOCLK_HPP
#define COSMOCLK_HPP

#include "cosmocore.hpp"
#include "cosmobus.hpp"

#include <ctime>

namespace cosmocore
{
    class cosmoclk
    {
        private: std::shared_ptr<cosmobus>& m_bus;

        public: cosmoclk() = delete;
        public: cosmoclk(std::shared_ptr<cosmobus>& bus);
        public: ~cosmoclk();

        public: u16i get_year(u16i dummy);
        public: u16i get_month(u16i dummy);
        public: u16i get_day(u16i dummy);
        public: u16i get_hour(u16i dummy);
        public: u16i get_min(u16i dummy);
        public: u16i get_seconds(u16i dummy);
    };
}

#endif /* COSMOCLK_HPP */