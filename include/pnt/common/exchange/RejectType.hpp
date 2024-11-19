#ifndef PNT_COMMON_EXCHANGE_REJECT_TYPE_HPP
#define PNT_COMMON_EXCHANGE_REJECT_TYPE_HPP
#pragma once

#include <ostream>
#include <iso646.h>

// Exchange reject type

namespace pnt { namespace common { namespace exchange {

enum struct RejectType {
    None = 0,
    PNT_Risk = 1,
    PNT_Gateway = 2,
    Session = 3,
    Business = 4,
    Exchange = 5
};

// Reject type as string
inline const char * text(const RejectType type) {
    switch (type) {
    case RejectType::None:
        return "None";
    case RejectType::PNT_Risk:
        return "PNT_Risk";
    case RejectType::PNT_Gateway:
        return "PNT_Gateway";
    case RejectType::Session:
        return "Session";
    case RejectType::Business:
        return "Business";
    case RejectType::Exchange:
        return "Exchange";
    default:
        return "INVALIDENUMVALUE(RejectType)";
    }
}

enum struct RiskRejectCode {
    Rate = 0,
    MaxExposure = 1,
    MaxPosition = 2,
    ChainGone = 3,
    INVALID = -1
};

typedef int RejectCode;
static constexpr RejectCode RejectCode_INVALID = -1;
static constexpr RejectCode RejectCode_RATE = (int)RiskRejectCode::Rate;

// Reject type as string
inline const char * text(const RiskRejectCode code) {
    switch (code) {
    case RiskRejectCode::Rate:
        return "Rate";
    case RiskRejectCode::MaxExposure:
        return "MaxExposure";
    case RiskRejectCode::MaxPosition:
        return "MaxPosition";
    case RiskRejectCode::ChainGone:
        return "ChainGone";
    case RiskRejectCode::INVALID:
        return "INVALID";
    default:
        return "INVALIDENUMVALUE(RiskRejectCode)";
    }
}

// Stream operator
inline std::ostream & operator<<(std::ostream &out, const RejectType &result) {
    return out << text(result);
}

inline std::ostream & operator<<(std::ostream &out, const RiskRejectCode &code) {
    return out << text(code);
}

///////////////////////////////////////////////////////////
// Fluent boolean methods

// Is reject a due to risk?
namespace is { inline bool risk(RejectType type) noexcept {
    return type == RejectType::PNT_Risk;
}}

// Is reject a due to gateway?
namespace is { inline bool gateway(RejectType type) noexcept {
    return type == RejectType::PNT_Gateway;
}}

///////////////////////////

// Is reject an internal reject?
namespace is { inline bool internal(RejectType reject) noexcept {
    return risk(reject) or gateway(reject);
}}


} /* namespace exchange */ } /* namespace common */ } /* namespace pnt */

#endif // PNT_COMMON_EXCHANGE_REJECT_TYPE_HPP
