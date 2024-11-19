#ifndef PNT_COMMON_TYPES_HPP
#define PNT_COMMON_TYPES_HPP
#pragma once

// #include <pnt/common/Log.hpp> // (20230630 - jwu) keeping this out of here to make cpp feathers client lib simpler. Log.hpp pulls in a lot of stuff...
#include <pnt/common/Timestamp.hpp>
#include <boost/noncopyable.hpp>
#include <ostream>
#include <fstream>
#include <limits>
#include <memory>
#include <stdint.h>
#include <vector>
#include <cmath>
#include <optional>
#include <map>

// GCC branch prediction macros
#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

namespace pnt { namespace common {

///////////////////////////////////////////////
// Prices

typedef double Price;

typedef std::vector<Price> PriceVector;

///////////////////////////////////////////////
// Bid/Ask Quantity

typedef int32_t Qty;
typedef uint32_t Level;
typedef double  QtyRatio;
typedef double  Ratio;
typedef int32_t OrderTxnId;
typedef uint64_t ExchangeOrderId;
typedef uint64_t ExchangePriority;
typedef uint32_t Endpoint;
typedef int32_t SecurityId;
//typedef std::string Symbol;
typedef std::string TraderId;
typedef uint32_t FillId;

namespace invalid {
    static constexpr Qty QTY = -1;
    static constexpr OrderTxnId ID = -1;
    static constexpr int32_t LINEPOS = -1;
    static constexpr Price PRICE = std::numeric_limits<Price>::lowest();
    static constexpr ExchangeOrderId EXCHANGEID = -1;
    static constexpr ExchangePriority EXPRIO = 0;
    static constexpr Endpoint ENDPOINT = -1;
    static constexpr SecurityId SECURITYID = -1;
    static constexpr FillId FILLID = 0;
}

static constexpr Qty Qty_INVALID = invalid::QTY;
static constexpr OrderTxnId OrderTxnId_INVALID = invalid::ID;
static constexpr int32_t LinePosition_INVALID = invalid::LINEPOS;
static constexpr Price Price_INVALID = invalid::PRICE;
static constexpr ExchangeOrderId ExchangeOrderId_INVALID = invalid::EXCHANGEID;
static constexpr ExchangePriority ExchangePriority_INVALID = invalid::EXPRIO;
static constexpr Endpoint Endpoint_INVALID = invalid::ENDPOINT;
static constexpr SecurityId SecurityId_INVALID = invalid::SECURITYID;
static constexpr FillId FillId_INVALID = invalid::FILLID;

static constexpr OrderTxnId OrderTxnId_PREOPEN_BID = -100;
static constexpr OrderTxnId OrderTxnId_PREOPEN_OFFER = -200;

struct Side {
    enum Enum {
        Bid = 0,
        Ask = 1
    };
private:
    Enum side;
public:
    Side( const Side &side) : side((Enum)side) {}
    Side( Enum side) : side(side) {}
    static Side::Enum flip(Side::Enum side) {
        if ( side == Bid ) {
            return Ask;
        } else {
            return Bid;
        }
    }
    operator Enum() const {
        return side;
    }

    std::string to_string() const {
        switch ( side ) {
            case Bid:
                return "Bid";
            case Ask:
                return "Ask";
            default:
                return "Unknown";
        }

        return "";
    }

    // deprecated
    std::string toString() const {
        return to_string();
    }

};

static constexpr Side::Enum SIDES[] = {Side::Bid, Side::Ask};

///////////////////////////////////////////////////////////
// Operators

// stream operator
inline std::ostream & operator<<(std::ostream &out, const Side &side) {
    return out << side.toString();
}

// stream operator
inline std::ostream & operator<<(std::ostream &out, const Side::Enum &side) {
    return out << Side(side).toString();
}

///////////////////////////////////////////////////////////
// Types

struct CMEQuoteFields
{
    uint32_t seqNum    ;
    int32_t depth      ;
    int32_t numOrders  ;
    int32_t directQty  ;

    CMEQuoteFields( )
    : seqNum( 0 ),
      depth( 0 ),
      numOrders( 0 ),
      directQty( 0 )
    {}

    CMEQuoteFields(uint32_t seqNum, int32_t depth, int32_t numOrders)
        : seqNum(seqNum),
        depth(depth),
        numOrders(numOrders),
        directQty(0)
    {}
};

// Useful for any value pairs where it makes sense to reference them by name 'bid' and 'ask'
template <class T>
struct BidAsk
{
    typedef T type;
    T bid;
    T ask;

    BidAsk() {}
    BidAsk(const T& bid, const T& ask) : bid{bid}, ask{ask} {}
    T& get(Side::Enum side) { return side == Side::Bid ? bid : ask; }
    const T& get(Side::Enum side) const { return side == Side::Bid ? bid : ask; }
    T& operator[](Side::Enum side) { return get(side); }
    const T& operator[](Side::Enum side) const { return get(side); }
    bool operator ==(const BidAsk<T>& other) const { return other.bid == bid && other.ask == ask; }
};

template <class T>
std::ostream& operator<<(std::ostream& out, const BidAsk<T>& bidask)
{
    out << bidask.bid << " | " << bidask.ask;
    return out;
}

// Represents the position of an order in a fifo queue
struct LinePosition {
    int32_t ordersBefore;
    int32_t ordersAfter;
    Qty qtyBefore;
    Qty qtyAfter;
    ExchangePriority exch_priority;

    LinePosition()
    : ordersBefore(invalid::LINEPOS)
    , ordersAfter(invalid::LINEPOS)
    , qtyBefore(invalid::QTY)
    , qtyAfter(invalid::QTY)
    , exch_priority(invalid::EXPRIO)
    {}
    LinePosition(int32_t position, int32_t orders_after, Qty qty_before, Qty qty_after, ExchangePriority exprio=invalid::EXPRIO)
    : ordersBefore(position)
    , ordersAfter(orders_after)
    , qtyBefore(qty_before)
    , qtyAfter(qty_after)
    , exch_priority(exprio)
    {}
    bool valid() const {
        return (
           ordersBefore != invalid::LINEPOS &&
           ordersAfter != invalid::LINEPOS &&
           qtyBefore != invalid::QTY &&
           qtyAfter != invalid::QTY
        );
    }

    friend std::ostream& operator<<(std::ostream& out, const LinePosition& linepos)
    {
        out
            << "ordersBefore=" << linepos.ordersBefore
            << ",ordersAfter=" << linepos.ordersAfter
            << ",qtyBefore=" << linepos.qtyBefore
            << ",qtyAfter=" << linepos.qtyAfter;
        return out;
    }
};

enum struct OrderPurpose {
    Resting = 0,
    Hedge = 1,
    Scratch = 2,
    Cover = 3,
    Manual = 4,
    Spread = 5,
    Puke = 6,
    Profit = 7,
    Warmup = 8,
    Scalp = 9,
    Unknown = -1
};
inline std::ostream& operator<<(std::ostream& out, const OrderPurpose& purpose)
{
    switch(purpose) {
        case OrderPurpose::Resting: out << "RESTING"; break;
        case OrderPurpose::Hedge:   out << "HEDGE"; break;
        case OrderPurpose::Scratch: out << "SCRATCH"; break;
        case OrderPurpose::Cover:   out << "COVER"; break;
        case OrderPurpose::Manual:  out << "MANUAL"; break;
        case OrderPurpose::Spread:  out << "SPREAD"; break;
        case OrderPurpose::Puke:    out << "PUKE"; break;
        case OrderPurpose::Profit:  out << "PROFIT"; break;
        case OrderPurpose::Warmup:  out << "WARMUP"; break;
        case OrderPurpose::Scalp:   out << "SCALP"; break;
        case OrderPurpose::Unknown: out << "UNKNOWN"; break;
    }
    return out;
}


template <typename T> inline int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

inline int sideSign(Side::Enum side){
    return side == Side::Bid ? 1 : -1;
}

inline Side pos_side(Qty pos){
    return pos > 0 ? Side::Bid : Side::Ask;
}

// is y closer to zero than x?
inline bool toward_zero(int x, int y) 
{ 
    return signum(x) == -1 * signum(y) || std::abs(y) < std::abs(x);
}

// // /////////////////////////////////////////////////////////
// Needed due to our use of floats without perfect precision

inline bool doubleEqual(double d1, double d2)
{
    return std::fabs(d1-d2) < 1e-9;
}

inline bool priceEqual(Price p1, Price p2)
{
    return std::fabs(p1-p2) < 1e-9;
}

inline bool priceLess(const Price& p1, const Price& p2)
{
    return p1 < (p2 - 1e-9);
}
inline bool priceGreater(const Price& p1, const Price& p2)
{
    return p1 > (p2 + 1e-9);
}
inline bool priceLE(const Price& p1, const Price& p2)
{
    return !priceGreater(p1, p2);
}
inline bool priceGE(const Price& p1, const Price& p2)
{
    return !priceLess(p1, p2);
}

struct PriceLessComp
{
    bool operator() (const Price& p1, const Price& p2) const
    {
        return priceLess(p1, p2);
    }
    typedef Price first_argument_type;
    typedef Price second_argument_type;
    typedef bool result_type;
};
struct PriceGreaterComp
{
    bool operator() (const Price& p1, const Price& p2) const
    {
        return priceGreater(p1, p2);
    }
    typedef Price first_argument_type;
    typedef Price second_argument_type;
    typedef bool result_type;
};

inline bool quantityGreater(const Qty& q1, const Qty& q2)
{
    return q1 > q2;
}

struct QuantityGreaterComp
{
    bool operator() (const Qty& q1, const Qty& q2) const
    {
        return quantityGreater(q1, q2);
    }
    typedef Qty first_argument_type;
    typedef Qty second_argument_type;
    typedef bool result_type;
};


// is p1 closer to the market than p2?:
inline bool isCloser(common::Side::Enum side, const Price p1, const Price p2)
{
    if(side == common::Side::Bid)
        return priceGreater(p1, p2);
    return priceLess(p1, p2);
}
// is p1 further from the market than p2?:
inline bool isAway(common::Side::Enum side, const Price p1, const Price p2)
{
    if(side == common::Side::Bid)
        return priceLess(p1, p2);
    return priceGreater(p1, p2);
}
inline Price bestPrice(common::Side::Enum side, const Price p1, const Price p2){
    return isCloser(side, p1, p2) ? p1 : p2;
}
inline Price worstPrice(common::Side::Enum side, const Price p1, const Price p2){
    return isCloser(side, p1, p2) ? p2 : p1;
}

inline Price roundBid(Price price, Price increment)
{
    return std::floor((price + 1e-10) / increment) * increment;
}

inline Price roundAsk(Price price, Price increment)
{
    return std::ceil((price - 1e-10) / increment) * increment;
}

inline Price roundPrice(Price price, Price increment)
{
    return std::round((price - 1e-10) / increment) * increment;
}

inline Price roundPrice(common::Side::Enum side, Price price, Price increment)
{
    if(side == common::Side::Bid)
        return roundBid(price, increment);
    else
        return roundAsk(price, increment);
}

inline Price roundToIncrement(Price value, const Price increment, common::Side::Enum side, const Price roundingThreshold = 0.95)
{
	Price remainder = fmod(value, increment);
	if (remainder != 0)
	{
		value -=  remainder;
		if (remainder < 0)
		{
			remainder = fabs(remainder);
			if ((side == common::Side::Ask && remainder > increment * roundingThreshold) ||
				(side == common::Side::Bid && remainder > increment * (1.0-roundingThreshold)))
			{
				value -= increment;
			}
		}
		else if ((side == common::Side::Ask && remainder > increment * (1-roundingThreshold)) ||
				 (side == common::Side::Bid && remainder > increment * roundingThreshold))
		{
			value += increment;
		}
	}
	return value;
}

inline Price roundUpToIncrement(Price const value, Price const increment)
{
    return std::ceil(value / increment) * increment;
}

inline Price roundDownToIncrement(Price const value, Price const increment)
{
    return std::floor(value / increment) * increment;
}

struct StrategyLogic
{
    uint32_t lastBookEventId_;
    uint64_t lastTradeEventId_;
    int32_t relatedFillOrderId_;

    StrategyLogic() :
        lastBookEventId_(0),
        lastTradeEventId_(0),
        relatedFillOrderId_(0)
    {}

    void reset()
    {
        lastBookEventId_ = 0;
        lastTradeEventId_ = 0;
        relatedFillOrderId_ = 0;
    }
};
typedef std::shared_ptr<StrategyLogic> StrategyLogicPtr;

enum struct MatchingAlgorithm
{
    FIFO = 'F', // First In, First Out (FIFO)
    CONFIGURABLE = 'K', // Configurable
    PRORATA = 'C', // Pro-Rata
    ALLOCATION = 'A', // Allocation
    FIFO_LMM = 'T', // FIFO with LMM
    THRESH_PRORATA = 'O', // Threshold Pro-Rata
    FIFO_TOP_LMM = 'S', // FIFO with TOP and LMM
    THRESH_PRORATA_LMM = 'Q', // Threshold Pro-Rata with LMM
    EURODOLLAR_OPTIONS = 'Y', // Eurodollar options,
    INVALID = '\0'
};

inline std::string to_string(const MatchingAlgorithm& algo){
    switch(algo) {
        case MatchingAlgorithm::FIFO: return "FIFO";
        case MatchingAlgorithm::CONFIGURABLE: return "CONFIGURABLE";
        case MatchingAlgorithm::PRORATA: return "PRORATA";
        case MatchingAlgorithm::ALLOCATION: return "ALLOCATION";
        case MatchingAlgorithm::FIFO_LMM: return "FIFO_LMM";
        case MatchingAlgorithm::THRESH_PRORATA: return "THRESH_PRORATA";
        case MatchingAlgorithm::FIFO_TOP_LMM: return "FIFO_TOP_LMM";
        case MatchingAlgorithm::THRESH_PRORATA_LMM:  return "THRESH_PRORATA_LMM";
        case MatchingAlgorithm::EURODOLLAR_OPTIONS: return "EURODOLLAR_OPTIONS";
        case MatchingAlgorithm::INVALID: default: return "INVALID";
    }
}

inline std::ostream& operator<<(std::ostream& out, const MatchingAlgorithm& algo)
{
    switch(algo) {
        case MatchingAlgorithm::FIFO: out << "FIFO"; break;
        case MatchingAlgorithm::CONFIGURABLE:   out << "CONFIGURABLE"; break;
        case MatchingAlgorithm::PRORATA: out << "PRORATA"; break;
        case MatchingAlgorithm::ALLOCATION:   out << "ALLOCATION"; break;
        case MatchingAlgorithm::FIFO_LMM:  out << "FIFO_LMM"; break;
        case MatchingAlgorithm::THRESH_PRORATA:  out << "THRESH_PRORATA"; break;
        case MatchingAlgorithm::FIFO_TOP_LMM:    out << "FIFO_TOP_LMM"; break;
        case MatchingAlgorithm::THRESH_PRORATA_LMM:  out << "THRESH_PRORATA_LMM"; break;
        case MatchingAlgorithm::EURODOLLAR_OPTIONS: out << "EURODOLLAR_OPTIONS"; break;
        case MatchingAlgorithm::INVALID: default: out << "INVALID"; break;
    }
    return out;
}

static const double nan = std::numeric_limits<double>::quiet_NaN();

} /* namespace common */ } /* namespace pnt */


#endif//PNT_COMMON_TYPES_HPP
