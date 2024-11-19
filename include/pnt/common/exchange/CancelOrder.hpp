#ifndef PNT_COMMON_EXCHANGE_CANCEL_ORDER_HPP
#define PNT_COMMON_EXCHANGE_CANCEL_ORDER_HPP
#pragma once

#include <pnt/common/exchange/Types.hpp>
#include <pnt/common/Timestamp.hpp>
#include <pnt/common/Types.hpp>

#ifdef PERF_STATS
#include <pnt/common/exchange/MetaData.hpp>
#endif

namespace pnt { namespace common { namespace exchange {

struct CancelOrder {

  /////////////////////////////////////////////////////////
  // Member Variables

    OrderTxnId txnId;
    OrderTxnId orderId;
    OrderTxnId origOrderId;
    OrderTxnId triggerId;
    TriggerType triggerType;
    uint64_t flags;
    Timestamp stamp;
    uint64_t partyDetailsListId = UINT64_MAX;

#ifdef PERF_STATS
    exchange::MetaData metaData;
#endif

   /////////////////////////////////////////////////////////
   // Construction

    // Standard constructor
    CancelOrder( OrderTxnId txnId, OrderTxnId orderId, OrderTxnId origOrderId ) noexcept
        : txnId( txnId )
        , orderId( orderId )
        , origOrderId{ origOrderId }
        , triggerId{ invalid::ID }
        , triggerType{ TriggerType::UNDEFINED }
        , flags(0)
    {}

    // Legacy constructor
    CancelOrder( OrderTxnId txnId, OrderTxnId orderId ) noexcept
        : CancelOrder( txnId, orderId, invalid::ID)
    {}

    // Default constructor
    CancelOrder() noexcept
        : txnId{ invalid::ID }
        , orderId{ invalid::ID }
        , origOrderId{ invalid::ID }
        , triggerId{ invalid::ID }
        , triggerType{ TriggerType::UNDEFINED }
        , flags(0)
    {}

    std::string toString() const {
        std::stringstream out;
        out << "CANCELORDER,"
                   << "txnId=" << txnId
                   << ",orderId=" << orderId
                   << ",origOrderId=" << origOrderId
                   << ",triggerId=" << triggerId
                   << ",triggerType=" << triggerType
                   << ",flags=" << flags
                   << ",partyDetailsListId=" << partyDetailsListId;
        return out.str();
    }

};

inline std::ostream & operator<<( std::ostream &out, const CancelOrder &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif // PNT_COMMON_EXCHANGE_CANCEL_ORDER_HPP
