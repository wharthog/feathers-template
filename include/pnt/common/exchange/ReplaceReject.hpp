#ifndef PNT_COMMON_EXCHANGE_REPLACE_REJECT_HPP
#define PNT_COMMON_EXCHANGE_REPLACE_REJECT_HPP

#include <pnt/common/Types.hpp>
#include <pnt/common/exchange/RejectType.hpp>
#include <ostream>
#include <sstream>

namespace pnt { namespace common { namespace exchange {

/**
 * Replace has failed the newOrderId was never placed and the oldOrderId state is unknown (by this message)
 */
struct ReplaceReject {

  ///////////////////////////////////////////////////////
  // Member variables

    OrderTxnId newOrderId;
    OrderTxnId oldOrderId;
    OrderTxnId origOrderId;
    std::string reason;
    RejectType type;
    RejectCode reason_code;
    Timestamp transactTime;
    Timestamp internalRateBreachEnd;
    SecurityId triggeringSecurityId;
  ///////////////////////////////////////////////////////
  // Construction

    ReplaceReject(
        OrderTxnId newOrderId,
        OrderTxnId oldOrderId,
        OrderTxnId origOrderId,
        const std::string &reason,
        RejectType type = RejectType::None,
        RejectCode code = RejectCode_INVALID,
        Timestamp internalRateBreachEnd = Timestamp(),
        SecurityId triggeringSecurityId = SecurityId_INVALID
    ) noexcept
        : newOrderId{newOrderId}
        , oldOrderId{oldOrderId}
        , origOrderId{origOrderId}
        , reason{reason}
        , type{type}
        , reason_code{code}
        , internalRateBreachEnd{internalRateBreachEnd}
        , triggeringSecurityId{triggeringSecurityId}
    {}

    ReplaceReject(
        OrderTxnId newOrderId,
        OrderTxnId oldOrderId,
        const std::string &reason,
        RejectType type = RejectType::None,
        RejectCode code = RejectCode_INVALID,
        Timestamp internalRateBreachEnd = Timestamp(),
        SecurityId triggeringSecurityId = SecurityId_INVALID
    ) noexcept
        : ReplaceReject(newOrderId, oldOrderId, invalid::ID, reason, type, code, internalRateBreachEnd, triggeringSecurityId)
    {}

    ReplaceReject() noexcept
        : ReplaceReject(0, 0, invalid::ID, "", RejectType::None, RejectCode_INVALID)
    {};

  ///////////////////////////////////////////////////////
  // Traits

    // Is this an internal reject?
    bool  isInternal() const noexcept {
        return is::internal(type);
    }

  ///////////////////////////////////////////////////////
  // Implementation

    std::string toString() const {
        std::stringstream out;
        out << "REPLACEREJECT,isInternal=" << isInternal()
            << ",newOrderId=" << newOrderId
            << ",oldOrderId=" << oldOrderId
            << ",origOrderId=" << origOrderId
            << ",type=" << type
            << ",reason=" << reason
            << ",code=" << reason_code
            << ",triggeringSecurityId=" << triggeringSecurityId;
        return out.str();
    }

};

inline std::ostream & operator<<(std::ostream &out, const ReplaceReject &message) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_EXCHANGE_REPLACE_REJECT_HPP
