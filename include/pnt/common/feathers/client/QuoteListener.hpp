#ifndef PNT_COMMON_FEATHERS_CLIENT_QUOTE_LISTENER_HPP
#define PNT_COMMON_FEATHERS_CLIENT_QUOTE_LISTENER_HPP

#include <pnt/common/BookUpdate.hpp>
#include <vector>
#include <string>

namespace pnt { namespace common { namespace feathers { namespace client {

struct QuoteListener {
    virtual ~QuoteListener() {}
    virtual void onMessage( const pnt::common::BookUpdates& updates ) = 0;
};

} /* namespace client */ } /* namespace feathers */ } /* namespace common */ } /* namespace pnt */

#endif //PNT_COMMON_FEATHERS_CLIENT_QUOTE_LISTENER_HPP