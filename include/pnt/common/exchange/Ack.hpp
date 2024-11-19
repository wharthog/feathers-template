#ifndef PNT_COMMON_EXCHANGE_ACK_HPP
#define PNT_COMMON_EXCHANGE_ACK_HPP

#include <pnt/common/Types.hpp>
#include <ostream>

    // Order ack

namespace pnt { namespace common { namespace exchange {

struct Ack {

  ///////////////////////////////////////////////////////
  // Member Variables

    OrderTxnId orderId;            // Pnt order id
    ExchangeOrderId exchange;           // Exchange order id
    bool isShort = false;
    Timestamp transactTime;

  ///////////////////////////////////////////////////////
  // Construction

    // Default constructor
    Ack() noexcept
     : Ack{invalid::ID, invalid::EXCHANGEID} {}

    // Legacy constructor
    Ack(OrderTxnId order) noexcept
     : Ack{order, invalid::EXCHANGEID} {}

    // Standard constructor
    Ack(OrderTxnId order, ExchangeOrderId exchange) noexcept
     : orderId{order}, exchange{exchange} {}

    std::string toString() const {
        std::stringstream out;
        out << "ACK,orderId=" << orderId
            << ",exchId=" << exchange
            << ",transactTS=" << transactTime;
        return out.str();
    }
};

inline std::ostream &operator<<(std::ostream &out, const Ack &message) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif // PNT_COMMON_EXCHANGE_ACK_HPP
