#ifndef PNT_COMMON_TRADEFIELDS_HPP
#define PNT_COMMON_TRADEFIELDS_HPP
#pragma once

#include <pnt/common/nbbo/Defines.hpp>

namespace pnt { namespace common {

struct TradeCondition {
    enum Enum {
        None         = 0,
        OpeningTrade = 1,
        ExCalculated = 2,
        ClosingTrade = 3,
    };
    TradeCondition(Enum val) : val((Enum)val) {}
    Enum val;
    operator Enum() const {
         return val;
    }
};

struct AggressorSide {
    enum Enum {
        None = 0,
        Buy  = 1,
        Sell = 2
    };
    AggressorSide(Enum val) : val((Enum)val) {}
    Enum val;
    operator Enum() const {
         return val;
    }
};

struct TickDirection {
    enum Enum {
        None = 0,
        Up   = 1,
        Down = 2
    };
    TickDirection(Enum val) : val((Enum)val) {}
    Enum val;
    operator Enum() const {
         return val;
    }
};

struct TradeFields
{
    TradeCondition tradeCondition ;
    AggressorSide  aggressorSide  ;
    TickDirection  tickDirection  ;

    TradeFields( )
    : tradeCondition( TradeCondition::None ),
      aggressorSide( AggressorSide::None ),
      tickDirection( TickDirection::None )
    {}
};

} /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_TRADEFIELDS_HPP
