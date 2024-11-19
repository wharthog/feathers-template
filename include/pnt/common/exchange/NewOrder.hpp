#ifndef PNT_COMMON_EXCHANGE_NEW_ORDER_HPP
#define PNT_COMMON_EXCHANGE_NEW_ORDER_HPP

#include <pnt/common/exchange/Types.hpp>
#include <pnt/common/OrderOwnerInfo.hpp>
#include <pnt/common/Timestamp.hpp>
#include <pnt/common/Types.hpp>
#include <pnt/common/Symbol.hpp>
#include <string>
#include <sstream>
#ifdef PERF_STATS
#include <pnt/common/exchange/MetaData.hpp>
#endif

namespace pnt { namespace common { namespace exchange {

struct NewOrder {
    static constexpr OrderTxnId TRIGGER_INVALID = invalid::ID;
  ///////////////////////////////////////////////////////
  // Member Variables

    OrderTxnId orderId = invalid::ID;
    Side::Enum side = Side::Bid;//invalid::PRICE == min double, so this is probably the safest default
    Symbol symbol;
    Qty qty = invalid::QTY;
    Price price = invalid::PRICE;
    bool shortMarker = false;
    bool hypermarket = false;
    TIF::Enum timeInForce = TIF::Day;
    std::optional<std::string> exDestination;
    std::optional<Price> stopPrice;
    std::optional<Qty> displayQty;
    std::optional<TraderId> traderId;
    std::optional<Timestamp> expirationTime;
    OrderOwnerInfo::Id ownerId = OrderOwnerInfo::NONE;
    TriggerType triggerType = TriggerType::UNDEFINED;
    OrderTxnId triggerId = TRIGGER_INVALID;
    Timestamp stamp;
    OrderTxnId batchId = TRIGGER_INVALID;
    ExchangeOrderId exchangeId = invalid::ID;  // Exchange order id, only set on existing orders
    OrderPurpose purpose = OrderPurpose::Unknown;
    bool external = false; // hw generated
    uint64_t flags = 0;
    uint64_t partyDetailsListId = UINT64_MAX;
    bool mid = false;

#ifdef PERF_STATS
    exchange::MetaData metaData;
#endif

  ///////////////////////////////////////////////////////
  // Construction

    NewOrder() noexcept {}

    NewOrder( OrderTxnId orderId, Side::Enum side, const Symbol &symbol, Qty qty, Price price ) noexcept
        : orderId( orderId )
        , side( side )
        , symbol( symbol )
        , qty( qty )
        , price( price )
    {}

    NewOrder(OrderTxnId orderId, TIF::Enum tif, Side::Enum side, const Symbol &symbol, Qty qty, Price price) noexcept
        : orderId( orderId )
        , side( side )
        , symbol( symbol )
        , qty( qty )
        , price( price )
        , timeInForce( tif )
    {}

    std::string to_string_short() const
    {
        std::stringstream ss;
        ss << "id: " << orderId
           << " sym: " << symbol
           << " side: " << common::Side(side)
           << " qty: " << qty
           << " px: " << price
        ;
        return ss.str();
    }

    std::string toString() const {
        std::stringstream out;
        out << "NEWORDER,orderId=" << orderId
                   << ",side=" << common::Side(side)
                   << ",symbol=" << symbol
                   << ",qty=" << qty
                   << ",price=" << price
                   << ",shortMarker=" << std::boolalpha << shortMarker << std::noboolalpha
                   << ",hypermarket=" << std::boolalpha << hypermarket << std::noboolalpha
                   << ",timeInForce=" << timeInForce
                   << ",expTime=" << (expirationTime ? expirationTime.value().to_string() : "NaT")
                   << ",exDestination=" << (exDestination ? exDestination.value() : "UNDEFINED")
                   << ",stopPrice=" << (stopPrice ? stopPrice.value() : 0)
                   << ",displayQty=" << (displayQty ? displayQty.value() : -1)
                   << ",triggerId=" << triggerId
                   << ",triggerType=" << triggerType
                   << ",ownerId=" << ownerId
                   << ",exchangeId=" << exchangeId
                   << ",purpose=" << purpose
                   << ",external=" << std::boolalpha << external << std::noboolalpha
                   << ",flags=" << flags
                   << ",partyDetailsListId=" << partyDetailsListId
                   ;
        return out.str();
    }

};

inline std::ostream & operator<<( std::ostream &out, const NewOrder &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_EXCHANGE_NEW_ORDER_HPP
