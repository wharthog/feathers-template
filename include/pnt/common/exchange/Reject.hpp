#ifndef PNT_COMMON_EXCHANGE_ORDER_REJECT_HPP
#define PNT_COMMON_EXCHANGE_ORDER_REJECT_HPP

#include <pnt/common/Types.hpp>
#include <pnt/common/exchange/RejectType.hpp>

#include <ostream>
#include <sstream>

// Exchange order reject

namespace pnt { namespace common { namespace exchange {

struct Reject {

  ///////////////////////////////////////////////////////
  // Member variables

    OrderTxnId orderId;
    std::string reason;
    RejectType type;
    RejectCode reason_code;
    Timestamp internalRateBreachEnd;
    SecurityId triggeringSecurityId;
  ///////////////////////////////////////////////////////
  // Construction

    // Default constructor
    Reject() noexcept
     : orderId{0},
       type{RejectType::None},
       reason_code(RejectCode_INVALID),
       internalRateBreachEnd{Timestamp()},
       triggeringSecurityId{SecurityId_INVALID}
    {};

    // Standard constructor
    Reject(
        OrderTxnId orderId,
        std::string reason,
        RejectType type = RejectType::None,
        RejectCode code = RejectCode_INVALID,
        Timestamp internalRateBreachEnd = Timestamp(),
        SecurityId triggeringSecurityId = SecurityId_INVALID
    ) noexcept
        : orderId( orderId )
        , reason( reason )
        , type( type )
        , reason_code(code)
        , internalRateBreachEnd{internalRateBreachEnd}
        , triggeringSecurityId{triggeringSecurityId}
    {}

  ///////////////////////////////////////////////////////
  // Traits

    // Is this an internal reject?
    bool isInternal() const noexcept {
        return is::internal(type);
    }

  ///////////////////////////////////////////////////////
  // Implementation

    std::string toString() const {
        std::stringstream out;
        out << "REJECT,isInternal=" << isInternal()
            << ",orderId=" << orderId
            << ",type=" << type
            << ",reason=" << reason
            << ",code=" << reason_code
            << ",triggeringSecurityId=" << triggeringSecurityId;
        return out.str();
    }
};

inline std::ostream & operator<<( std::ostream &out, const Reject &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif // PNT_COMMON_EXCHANGE_ORDER_REJECT_HPP
