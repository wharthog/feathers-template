#ifndef PNT_COMMON_QUOTE_HPP
#define PNT_COMMON_QUOTE_HPP
#pragma once

#include <pnt/common/Types.hpp>

#include <boost/bind.hpp>
#include <boost/container/flat_map.hpp>

#include <iomanip>
#include <ostream>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iso646.h>

// Standard Quote

namespace pnt { namespace common {

struct Quote {

  ///////////////////////////////////////////////////////////
  // Memeber Variables

    Qty qty = invalid::QTY;
    Price price = 0.;

    constexpr Quote() noexcept {}
    Quote(Qty qty, Price price) : qty{qty}, price{price} {}

  ///////////////////////////////////////////////////////////
  // Methods

    // Is quote valid?
    bool valid() const noexcept {
        return qty != invalid::QTY;
    }

    // Is quote invertible?
    bool invertible() const noexcept {
        return valid() and price != 0.;
    }

  /////////////////////////

    // Clear quote
    void clear() {
        qty = invalid::QTY;
        price = 0.;
    }

  ///////////////////////////////////////////////////////////
  // Quote Comparators

    // Compare quotes by price ascending
    static bool CompareByPriceDescending(const Quote& lhs, const Quote& rhs) { return lhs.price > rhs.price; };
    static bool ComparePriceLesser(const Quote& lhs, const Quote& rhs) { return lhs.price < rhs.price; };
    static bool CompareQtyLesser(const Quote& lhs, const Quote& rhs) { return lhs.qty < rhs.qty; };

    std::string to_string() const {
        return valid() ? std::to_string(qty) + '@' + priceString() : "No Value";
    }
private:
    std::string priceString() const {
        std::ostringstream out;
        // note that we are *not* using std::fixed here
        out << std::setprecision(8) << price;
        return out.str();
    }
};

struct QuoteWithCount : Quote
{
    Qty order_count;
    QuoteWithCount() : Quote(), order_count(-1) {};
    QuoteWithCount(Quote quote, Qty count)
        : Quote(quote)
        , order_count(count)
    {}
    QuoteWithCount(Qty qty, Price price, Qty count)
        : Quote(qty, price)
        , order_count(count)
    {}
    void clear() {
        Quote::clear();
        order_count = 0;
    }
};


///////////////////////////////////////////////////////////
// Quote Operators - pass by value might be faster

// Equals operator
inline bool operator==(const Quote& lhs, const Quote& rhs) {
    return (lhs.qty == rhs.qty && priceEqual(lhs.price, rhs.price));
}

// Not equals operator
inline bool operator!=(const Quote& lhs, const Quote& rhs) {
	return !operator==(lhs, rhs);
};

// Stream operator
inline std::ostream & operator<<(std::ostream &out, const Quote &quote) {
    return out << quote.to_string();
}

///////////////////////////////////////////////////////////

// Quote Pair (this can probably be remvoed once we remove direct container dependancy )

struct QuotePair {
    const std::pair< const Price, Quote > &pair;

    QuotePair( const std::pair< const Price, Quote > &pair ) : pair( pair ) {}
    friend std::ostream & operator<<( std::ostream &out, const QuotePair &pair ) {
        out << pair.pair.second;
        return out;
    }
};

///////////////////////////////////////////////////////////
// Quote Containers


// Quote Map
// This fugly, but we are in 03 so it goes
template<typename Ordering>
struct QuoteBy {
    typedef std::map< Price, Quote, Ordering > map;
    typedef boost::container::flat_map< Price, Qty, Ordering > vector;
};

typedef std::greater< Price > BidOrder;
typedef std::less< Price > AskOrder;

// Quote Vector
typedef std::vector< Quote > QuoteVector;

// stream operator for quote vectors
inline std::ostream & operator<<(std::ostream &out, const QuoteVector &quotes) {
    std::copy(quotes.begin(), quotes.end(), std::ostream_iterator<Quote>(out, "\n"));

    return out;
}

// Transform a quote map to a quote vector
template<typename Ordering>
QuoteVector vectorize(typename QuoteBy< Ordering >::map map) {
    QuoteVector vector;
    std::transform(map.begin(), map.end(), std::back_inserter(vector), boost::bind(&QuoteBy< Ordering >::map::value_type::second, _1));
    sort(vector.begin(), vector.end(), Quote::CompareByPriceDescending);

    return vector;
}


///////////////////////////////////////////////////////////
// Conditional Quote Operators

typedef std::optional< Quote > OptionalQuote;

inline std::ostream & operator<<(std::ostream &out, const OptionalQuote &quote) {
    if (quote) {
        out << *quote;
    } else {
        out << "No value";
    }

    return out;
}


} /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_QUOTE_HPP
