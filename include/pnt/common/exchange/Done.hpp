#ifndef PNT_COMMON_EXCHANGE_DONE_HPP
#define PNT_COMMON_EXCHANGE_DONE_HPP

#include <pnt/common/Types.hpp>
#include <ostream>

namespace pnt { namespace common { namespace exchange {

/**
 * There will be no more updates for this order, feel free to perform any cleanup
 */
struct Done {
    OrderTxnId orderId;
    OrderTxnId origOrderId;
    Done( OrderTxnId orderId, OrderTxnId origOrderId )
        : orderId{ orderId }
        , origOrderId{ origOrderId }
    {}
    Done( OrderTxnId orderId )
        : Done(orderId, invalid::ID)
    {}

    std::string toString() const {
        std::stringstream out;
        out << "DONE,orderId=" << orderId
            << ",origOrderId=" << origOrderId;
        return out.str();
    }
};

inline std::ostream & operator<<( std::ostream &out, const Done &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_EXCHANGE_DONE_HPP
