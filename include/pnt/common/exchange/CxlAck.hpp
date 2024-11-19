#ifndef PNT_COMMON_EXCHANGE_CXL_ACK_HPP
#define PNT_COMMON_EXCHANGE_CXL_ACK_HPP

#include <pnt/common/Types.hpp>
#include <ostream>

namespace pnt { namespace common { namespace exchange {

enum ExecRestatementReason
{
    UnknownReason =                        0,
    Exchange =                             8,
	CancelledNotBest =                     9,
    CancelOnDisconnect =                   100,
    CancelOldestDueToSelfMatchPrevention = 103,
    CancelFromCMECreditControlsViolation = 104,
    CancelFromFirmsoft =                   105,
    CancelFromRiskManagement =             106,
    CancelNewestDueToSelfMatchPrevention = 107,
	CancelDueToVolatilityQuoteOptionLessThanMin = 108,
    MassActionCxl =                        999
};

struct CxlAck {
    OrderTxnId txnId;
    OrderTxnId orderId;
    OrderTxnId origOrderId;
    Timestamp transactTime;
    ExecRestatementReason cxlReason;

    CxlAck( OrderTxnId txnId, OrderTxnId orderId, OrderTxnId origOrderId, ExecRestatementReason reason = ExecRestatementReason::UnknownReason ) noexcept
        : txnId{ txnId }
        , orderId{ orderId }
        , origOrderId{ origOrderId }
        , cxlReason{ reason }
    {}
    CxlAck( OrderTxnId txnId, OrderTxnId orderId ) noexcept
        : CxlAck(txnId, orderId, invalid::ID)
    {}

    std::string toString() const {
        std::stringstream out;
        out << "CXLACK,txnId=" << txnId
            << ",orderId=" << orderId
            << ",origOrderId=" << origOrderId;
        return out.str();
    }
};

inline std::ostream & operator<<( std::ostream &out, const CxlAck &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_EXCHANGE_CXL_ACK_HPP
