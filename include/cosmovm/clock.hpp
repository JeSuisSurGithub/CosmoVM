#ifndef CLOCK_HPP
#define CLOCK_HPP

#include "common.hpp"
#include "bus.hpp"

namespace cosmovm
{
    class clock
    {
        private:
            std::shared_ptr<bus>& m_bus;

        public:
            clock() = delete;
            clock(std::shared_ptr<bus>& bus);
            ~clock();

            u16 get_year(u16 dummy);
            u16 get_month(u16 dummy);
            u16 get_day(u16 dummy);
            u16 get_hour(u16 dummy);
            u16 get_min(u16 dummy);
            u16 get_seconds(u16 dummy);
    };
}

#endif /* CLOCK_HPP */