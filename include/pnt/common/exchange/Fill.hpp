#ifndef PNT_COMMON_EXCHANGE_FILL_HPP
#define PNT_COMMON_EXCHANGE_FILL_HPP
#pragma once

#include <pnt/common/Types.hpp>
#include <pnt/common/Timestamp.hpp>

#include <ostream>
#include <array>

    // Order Fill

namespace pnt { namespace common { namespace exchange {

namespace LiquidityFlag {
enum Enum {
    Add = 1,
    Remove = 2,
    Routed = 3,
    Self = 4,
    Unknown = 999
};}

struct Fill {
    enum struct FillReason {
        OTHER = 0,
        PRO_RATA = 1,
        TOP = 2,
        FIFO = 3,
        LEVELING = 4,
        OPENING = 5,
        IMPLIED_OPENING = 6,
        FIFO_PERCENT = 7,
    };
    
  /////////////////////////////////////////////
  // Members

    OrderTxnId orderId;
    OrderTxnId origOrderId;
    Qty qty;
    Price price;
    Qty openQty;
    Qty cumQty = 0;
    Side::Enum side;
    Timestamp filledAt;
    FillId fillId;
    LiquidityFlag::Enum liquidityFlag;
    Timestamp sendTime;
    Timestamp transactTime;
    bool isShort = false;
    std::array<Qty, 8> fillReasons{};

  /////////////////////////////////////////////
  // Constructor

    Fill(OrderTxnId id, OrderTxnId origOrderId, Qty qty, Price price, Qty open, Side::Enum side, uint fillId, Timestamp filledAt)
        : orderId{id}
        , origOrderId{origOrderId}
        , qty{qty}
        , price{price}
        , openQty{open}
        , side{side}
        , filledAt{filledAt}
        , fillId{fillId}
        , liquidityFlag{LiquidityFlag::Unknown}
        , sendTime{Timestamp::invalid()}
        , transactTime{Timestamp::invalid()}
    {}

    Fill(OrderTxnId id, Qty qty, Price price, Qty open, Side::Enum side, uint fillId, Timestamp filledAt)
        : Fill(id, invalid::ID, qty, price, open, side, fillId, filledAt)
    {}

    Fill( OrderTxnId id, Qty qty, Price price, Qty open, Side::Enum side, Timestamp filledAt )
        : Fill(id, invalid::ID, qty, price, open, side, 0, filledAt)
    {}

    Fill()
        : Fill(invalid::ID, invalid::ID, invalid::QTY, 0, invalid::QTY, Side::Ask, 0, Timestamp(Timestamp::invalid()))
    {}

    std::string toString() const {
        std::stringstream out;
        out << "FILL"
               << ",orderId=" << orderId
               << ",origOrderId=" << origOrderId
               << ",side=" << Side(side)
               << ",qty=" << qty
               << ",price=" << std::setprecision(7) << price
               << ",openQty=" << openQty
               << ",filledAt=" << filledAt
               << ",sendTime=" << sendTime
               << ",transactTime=" << transactTime
               << ",liqFlag=" << liquidityFlag
               << ",fillId=" << fillId
               << ",cumQty=" << cumQty;
        return out.str();
    }

};

// Stream operator
inline std::ostream & operator<<(std::ostream &out, const Fill &message) {
    return out << message.toString();
}

} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif //PNT_COMMON_EXCHANGE_FILL_HPP
