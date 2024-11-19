#ifndef PNT_COMMON_EXCHANGE_CXL_REJECT_HPP
#define PNT_COMMON_EXCHANGE_CXL_REJECT_HPP

#include <pnt/common/Types.hpp>
#include <pnt/common/exchange/RejectType.hpp>

#include <ostream>
#include <sstream>

namespace pnt { namespace common { namespace exchange {

struct CxlReject {

  ///////////////////////////////////////////////////////
  // Member variables

    // (20210323 - jwu) why don't we include txnId here if we include it in CxlAck?
    // OrderTxnId txnId;
    OrderTxnId orderId;
    OrderTxnId origOrderId;
    std::string reason;
    RejectType type;
    RejectCode reason_code;
    Timestamp transactTime;

  ///////////////////////////////////////////////////////
  // Construction

    CxlReject(
        OrderTxnId orderId,
        OrderTxnId origOrderId,
        const std::string &reason,
        RejectType type = RejectType::None,
        RejectCode code = RejectCode_INVALID
    ) noexcept
      : orderId{ orderId }
      , origOrderId{ origOrderId }
      , reason{ reason }
      , type{ type }
      , reason_code{ code }
    {}

    CxlReject(
        OrderTxnId orderId,
        const std::string &reason,
        RejectType type = RejectType::None,
        RejectCode code = RejectCode_INVALID
    ) noexcept
      : CxlReject(orderId , invalid::ID, reason, type, code)
    {}

    CxlReject() noexcept
      : CxlReject(0 , invalid::ID, "", RejectType::None, RejectCode_INVALID)
    {}

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
        out << "CXLREJECT,isInternal=" << isInternal()
            << ",orderId=" << orderId
            << ",origOrderId=" << origOrderId
            << ",type=" << type
            << ",reason=" << reason;
        return out.str();
    }
};

inline std::ostream & operator<<( std::ostream &out, const CxlReject &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_EXCHANGE_CXL_REJECT_HPP
