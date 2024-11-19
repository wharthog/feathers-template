#ifndef PNT_COMMON_EXCHANGE_TYPES_HPP
#define PNT_COMMON_EXCHANGE_TYPES_HPP

#include <string>
#include <sstream>

namespace pnt { namespace common { namespace exchange {

namespace TIF {
enum Enum{
    Day = 0,
    DayExt = 1,
    GTC = 2,
    Open = 3,
    IOC = 4,
    FOK = 5,
    Close = 6,
    GTD = 7, 
    Unknown = 99,
};
inline std::ostream & operator<<( std::ostream &out, const Enum &tif ) {
    switch ( tif ) {
        case Day:
            out << "Day";
            break;
        case DayExt:
            out << "DayExt";
            break;
        case GTC:
            out << "GTC";
            break;
        case Open:
            out << "Open";
            break;
        case IOC:
            out << "IOC";
            break;
        case FOK:
            out << "FOK";
            break;
        case Close:
            out << "Close";
            break;
        case GTD:
            out << "GTD";
            break;
        case Unknown:
        default:
            out << "Unknown";
    }
    return out;
}
}

enum struct TriggerType { MD = 0, FILL = 1, UNDEFINED = 2, TIMER = 3, USER_INTERVENTION = 4, ORDER_ACTION = 5, MD_PRED_FILL = 6 };
inline std::ostream & operator<<( std::ostream &out, const TriggerType &triggerType ) {
     switch ( triggerType ) {
        case TriggerType::MD:
            out << "MD";
            break;
        case TriggerType::FILL:
            out << "FILL";
            break;
        case TriggerType::TIMER:
            out << "TIMER";
            break;
        case TriggerType::USER_INTERVENTION:
            out << "USER_INTERVENTION";
            break;
        case TriggerType::ORDER_ACTION:
            out << "ORDER_ACTION";
            break;
        case TriggerType::MD_PRED_FILL:
            out << "MD_PRED_FILL";
            break;
        default:
            out << "UNDEFINED";
            break;
     }
     return out;
}


} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_EXCHANGE_TYPES_HPP
