#ifndef PNT_COMMON_EXCHANGE_REPLACE_ORDER_HPP
#define PNT_COMMON_EXCHANGE_REPLACE_ORDER_HPP

#include <pnt/common/Symbol.hpp>
#include <pnt/common/exchange/Types.hpp>
#include <pnt/common/OrderOwnerInfo.hpp>
#include <pnt/common/Timestamp.hpp>
#include <pnt/common/Types.hpp>
#include <boost/optional.hpp>

#ifdef PERF_STATS
#include <pnt/common/exchange/MetaData.hpp>
#endif

namespace pnt { namespace common { namespace exchange {

struct ReplaceOrder {
    // (20160423 - jwu) Note that there's no Side in a ReplaceOrder request. RiskControl, for example, depends on this.
    static constexpr int32_t TRIGGER_INVALID = invalid::ID;

  /////////////////////////////////////////////////////////
  // Member variables

    OrderTxnId txnId; // new order id
    OrderTxnId orderId; // previous order id
    OrderTxnId origOrderId; // orig order id
    Qty qty;
    Price price;
    Symbol symbol;
    std::optional<Price> stopPrice;
    std::optional<Qty> displayQty;
    std::optional<TraderId> traderId;
    OrderOwnerInfo::Id ownerId;
    std::optional<TIF::Enum> timeInForce;
    OrderTxnId triggerId;
    exchange::TriggerType triggerType;
    Timestamp stamp;
    OrderTxnId batchId;
    OrderPurpose purpose;
    bool ifm;
    
    // flag used together with ifm. If only ifm is true, use order qty + cumQty as replaced qty.
    // If both are true, use order qty as replace qty, so the order will not be filled more than its
    // original qty
    bool noQtyIncrease;

    uint64_t flags = 0;
    uint64_t partyDetailsListId;

#ifdef PERF_STATS
    exchange::MetaData metaData;
#endif

  /////////////////////////////////////////////////////////
  // Construction

    // Standard limit order constructor
    ReplaceOrder( OrderTxnId txnId, OrderTxnId orderId, OrderTxnId origOrderId, Qty qty, Price price, const Symbol &symbol ) noexcept
        : txnId( txnId )
        , orderId( orderId )
        , origOrderId{ origOrderId }
        , qty( qty )
        , price( price )
        , symbol( symbol )
        , ownerId( OrderOwnerInfo::NONE )
        , triggerId{ invalid::ID }
        , triggerType{ exchange::TriggerType::UNDEFINED }
        , batchId{ invalid::ID }
        , purpose{ OrderPurpose::Unknown }
        , ifm{ false }
        , noQtyIncrease{ false }
        , partyDetailsListId{ UINT64_MAX }
    {}

    // Standard iceberg order constructor
    ReplaceOrder( OrderTxnId txnId, OrderTxnId orderId, OrderTxnId origOrderId, Qty qty, Price price, const Symbol &symbol, Qty displayQty ) noexcept
        : txnId( txnId )
        , orderId( orderId )
        , origOrderId{ origOrderId }
        , qty( qty )
        , price( price )
        , symbol( symbol )
        , displayQty(displayQty)
        , ownerId( OrderOwnerInfo::NONE )
        , triggerId{ invalid::ID }
        , triggerType{ exchange::TriggerType::UNDEFINED }
        , batchId{ invalid::ID }
        , purpose{ OrderPurpose::Unknown }
        , ifm{ false }
        , noQtyIncrease{ false }
        , partyDetailsListId{ UINT64_MAX }
    {}

    // SLOW: Legacy limit order constructor, without origOrderId this is now slower
    ReplaceOrder( OrderTxnId txnId, OrderTxnId orderId, Qty qty, Price price, const Symbol &symbol ) noexcept
        : ReplaceOrder( txnId, orderId, invalid::ID, qty, price, symbol )
    {}

    // SLOW: Legacy iceberg order constructor, without origOrderId this is now slower
    ReplaceOrder( OrderTxnId txnId, OrderTxnId orderId, Qty qty, Price price, const Symbol &symbol, Qty displayQty ) noexcept
        : ReplaceOrder( txnId, orderId, invalid::ID, qty, price, symbol, displayQty )
    {}

    // Default constructor
    ReplaceOrder() noexcept
        : txnId{ invalid::ID }
        , orderId{ invalid::ID }
        , origOrderId{ invalid::ID }
        , qty{ invalid::QTY }
        , price{ 0. }
        , ownerId{ OrderOwnerInfo::NONE }
        , triggerId{ invalid::ID }
        , triggerType{ exchange::TriggerType::UNDEFINED }
        , batchId{ invalid::ID }
        , purpose{ OrderPurpose::Unknown }
        , ifm{ false }
        , noQtyIncrease{ false }
        , partyDetailsListId{ UINT64_MAX }
    {}

    std::string toString() const {
        std::stringstream out;
        out << "REPLACE,txnId=" << txnId
                   << ",orderId=" << orderId
                   << ",origOrderId=" << origOrderId
                   << ",qty=" << qty
                   << ",price=" << price
                   << ",stopPrice=" << (stopPrice ? stopPrice.value() : 0)
                   << ",displayQty=" << (displayQty ? displayQty.value() : -1)
                   << ",ownerId=" << ownerId
                   << ",triggerId=" << triggerId
                   << ",triggerType=" << triggerType
                   << ",purpose=" << purpose
                   << ",ifm=" << std::boolalpha << ifm << std::noboolalpha
                   << ",flags=" << flags
                   << ",partyDetailsListId=" << partyDetailsListId;
                   if(traderId)
                      out << ",traderId=" << traderId.value();
        if(timeInForce)
            out << ",timeInForce=" << timeInForce.value();
        return out.str();
    }

};

inline std::ostream & operator<<( std::ostream &out, const ReplaceOrder &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif // PNT_COMMON_EXCHANGE_REPLACE_ORDER_HPP
