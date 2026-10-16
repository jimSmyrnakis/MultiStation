#include "Globals.hpp"

namespace MultiStation::IO {

    Access operator|(Access a, Access b)
    {
        return static_cast<Access>(
            static_cast<uint16_t>(a) |
            static_cast<uint16_t>(b)
            );
    }

    bool HasAccess(Access value, Access flag)
    {
        return
            (static_cast<uint16_t>(value) &
                static_cast<uint16_t>(flag)) != 0;
    }

}
