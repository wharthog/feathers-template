#pragma once

#include <pnt/common/Quote.hpp>
#include <pnt/common/Types.hpp>
#include <bitset>

namespace pnt { namespace common { namespace nbbo {


constexpr auto EXCHANGE_COUNT_START_LINE = __LINE__;
enum struct EXCHANGE : std::uint32_t {
    UNDEFINED = 0,
    NYSE      = 1,
    ARCA      = 2,
    NASDAQ    = 3,
    BZX       = 4,
    EDGX      = 5,
    INVALID   = UINT32_MAX
};
constexpr auto EXCHANGE_COUNT = __LINE__ - EXCHANGE_COUNT_START_LINE - 5; // don't count "UNDEFINED" or "INVALID"

// it *has* to be optimized so that it's not faster to use std::bitset<64>, right? gbench seems to say so.
typedef std::bitset<EXCHANGE_COUNT> ExchangeMask;
struct ExchangeMaskPair {
    ExchangeMask bid;
    ExchangeMask ask;
    ExchangeMaskPair()
        : bid{}
        , ask{}
    {}
    ExchangeMaskPair(ExchangeMask bid, ExchangeMask ask)
        : bid{bid}
        , ask{ask}
    {}
    ExchangeMaskPair(const std::string& pair_str)
        : ExchangeMaskPair{}
    {
        auto mask_split = pair_str.find("|");
        bid = ExchangeMask(std::atoll(pair_str.substr(0, mask_split).c_str()));
        ask = ExchangeMask(std::atoll(pair_str.substr(mask_split + 1).c_str()));
    }
    std::string to_string(){ return std::to_string(bid.to_ullong()) + "|" + std::to_string(ask.to_ullong()); }
};

inline std::string to_string(EXCHANGE exchange) {
    switch (exchange) {
    case EXCHANGE::UNDEFINED:
        return "UNDEFINED";
    case EXCHANGE::NYSE:
        return "NYSE";
    case EXCHANGE::ARCA:
        return "ARCA";
    case EXCHANGE::NASDAQ:
        return "NSDQ";
    case EXCHANGE::BZX:
        return "BZX";
    case EXCHANGE::EDGX:
        return "EDGX";
    // case EXCHANGE::INVALID:
    default:
        return "INVALID";
    }
}

inline std::ostream & operator<<(std::ostream &out, EXCHANGE exchange) {
    return out << to_string(exchange);
    // switch (exchange) {
    // case EXCHANGE::UNDEFINED:
    //     return out << "UNDEFINED";
    // case EXCHANGE::NYSE:
    //     return out << "NYSE";
    // case EXCHANGE::ARCA:
    //     return out << "ARCA";
    // case EXCHANGE::NASDAQ:
    //     return out << "NSDQ";
    // case EXCHANGE::BZX:
    //     return out << "BZX";
    // case EXCHANGE::EDGX:
    //     return out << "EDGX";
    // // case EXCHANGE::INVALID:
    // default:
    //     return out << "INVALID";
    // }
}

constexpr common::Price bid_min = DBL_MIN;
constexpr common::Price ask_max = DBL_MAX;

inline Quote bid_init(){
    return {0, bid_min};
}

inline Quote ask_init(){
    return {0, ask_max};
}

inline std::string normalize(const EXCHANGE& exchange, const std::string& symbol){
    switch (exchange) {
    case EXCHANGE::NYSE:
    case EXCHANGE::ARCA:
        {
            std::string suffix_separator = " ";
            auto pos = symbol.find(suffix_separator);
            if(pos == std::string::npos){
                return symbol;
            }
            std::string result = symbol;
            std::string to = ".PR";
            std::string from = " PR";
            if(symbol.find(from.c_str(), pos, from.length()) != std::string::npos){
                return result.replace(pos, from.length(), to);
            }
            else {
                to = ".";
                return result.replace(pos, suffix_separator.length(), to);
            }
        }
    case EXCHANGE::NASDAQ:
    case EXCHANGE::BZX:
    case EXCHANGE::EDGX:
        {
            std::string to = ".PR";
            std::string from = "-";
            auto pos = symbol.find(from);
            if(pos == std::string::npos)
                return symbol;
            std::string result = symbol;
            return result.replace(pos, from.length(), to);
        }
    // case EXCHANGE::UNDEFINED:
    // case EXCHANGE::INVALID:
    default:
        return symbol;
    }
}


} /* namespace nbbo */ } /* namespace common */ } /*namespace pnt */
