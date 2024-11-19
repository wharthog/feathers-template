#ifndef PNT_COMMON_EXCHANGE_REPLACED_HPP
#define PNT_COMMON_EXCHANGE_REPLACED_HPP
#include <pnt/common/Types.hpp>

namespace pnt { namespace common { namespace exchange {

struct Replaced {
    OrderTxnId newOrderId;
    OrderTxnId oldOrderId;
    OrderTxnId origOrderId;
    Qty qty;
    Price price;
    ExchangeOrderId exchangeId;
    std::optional<Qty> displayQty;
    bool isShort = false;
    Timestamp transactTime;

    Replaced(OrderTxnId newOrderId, OrderTxnId oldOrderId, OrderTxnId origOrderId, Qty qty, Price price, ExchangeOrderId exchangeId, std::optional<Qty> displayQty) noexcept
        : newOrderId{newOrderId}
        , oldOrderId{oldOrderId}
        , origOrderId{origOrderId}
        , qty{qty}
        , price{price}
        , exchangeId{exchangeId}
        , displayQty{displayQty}
    {}

    Replaced(OrderTxnId newOrderId, OrderTxnId oldOrderId, Qty qty, Price price, ExchangeOrderId exchangeId, std::optional<Qty> displayQty) noexcept
        : Replaced(newOrderId, oldOrderId, invalid::ID, qty, price, exchangeId, displayQty)
    {}

    Replaced(OrderTxnId newOrderId, OrderTxnId oldOrderId, OrderTxnId origOrderId, Qty qty, Price price, ExchangeOrderId exchangeId) noexcept
        : Replaced(newOrderId, oldOrderId, origOrderId, qty, price, exchangeId, std::optional<Qty>())
    {}

    Replaced( OrderTxnId newOrderId, OrderTxnId oldOrderId, Qty qty, Qty displayQty, Price price ) noexcept
        : Replaced(newOrderId, oldOrderId, invalid::ID, qty, price, invalid::ID, displayQty)
    {}

    Replaced( OrderTxnId newOrderId, OrderTxnId oldOrderId, Qty qty, Price price ) noexcept
        : Replaced(newOrderId, oldOrderId, invalid::ID, qty, price, invalid::ID)
    {}

    std::string toString() const {
        std::stringstream out;
        out << "REPLACED,newOrderId=" << newOrderId
                   << ",oldOrderId=" << oldOrderId
                   << ",origOrderId=" << origOrderId
                   << ",exchId=" << exchangeId
                   << ",qty=" << qty
                   << ",price=" << price;
        return out.str();
    }

};

inline std::ostream & operator<<( std::ostream &out, const Replaced &message ) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_EXCHANGE_REPLACED_HPP
