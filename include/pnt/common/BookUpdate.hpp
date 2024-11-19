#ifndef PNT_COMMON_BOOK_UPDATE_HPP
#define PNT_COMMON_BOOK_UPDATE_HPP

#include <pnt/common/Quote.hpp>
#include <pnt/common/TradeFields.hpp>
#include <pnt/common/Timestamp.hpp>
#include <pnt/common/Types.hpp>
#include <pnt/common/Symbol.hpp>
#ifdef PERF_STATS
#include <pnt/common/exchange/MetaData.hpp>
#endif

namespace pnt { namespace common {

struct BookUpdate {
    enum Enum {
        Bid  = 0,
        Ask  = 1,
        Last = 2
    };

    struct Kind {
        Kind( Enum kind ) : kind( kind ) {}
        Kind( Side::Enum side ) : kind( static_cast< Enum >( side ) ) {}
        Kind() {} // default construction is ok
        friend std::ostream & operator<<( std::ostream &out, const Kind &kind ) {
            switch( kind.kind ) {
                case Bid:
                    out << "Bid";
                    break;
                case Ask:
                    out << "Ask";
                    break;
                case Last:
                    out << "Last";
                    break;
            }
            return out;
        }
        operator Enum() const {
            return kind;
        }
        bool operator==( Side::Enum &side ) {
            return static_cast< Enum >( side ) == kind;
        }

        Enum kind;
    };

    Kind kind;
    Quote quote;
    std::optional<int> matchEventIndicator;
    std::optional<TradeFields> tradeFields;
    std::optional<CMEQuoteFields> cmeQuoteFields;

    BookUpdate( Kind kind, const Quote &quote ) : kind( kind ), quote( quote ) {}
    BookUpdate() {} // default construction is ok.
    static BookUpdate bid( const Quote &quote ) {
        return BookUpdate( Bid, quote );
    }
    static BookUpdate ask( const Quote &quote ) {
        return BookUpdate( Ask, quote );
    }
    static BookUpdate last( const Quote &quote ) {
        return BookUpdate( Last, quote );
    }
    static BookUpdate remove( Side::Enum side, Price price ) {
        return BookUpdate( side, Quote( 0, price ) );
    }
    bool remove() const { return quote.qty == 0; }
    friend std::ostream & operator<<( std::ostream &out, const BookUpdate &update ) {
        out << update.kind << ' ' << update.quote;
        if(update.cmeQuoteFields){
            out << ' ' << update.cmeQuoteFields->seqNum
                << ' ' << update.cmeQuoteFields->depth
                << ' ' << update.cmeQuoteFields->numOrders
                << ' ' << update.cmeQuoteFields->directQty;
        }
        return out;
    }
};


///////////////////////////////////////////////////////////
// Book updates

typedef std::vector<  BookUpdate > BookUpdateVector;

struct BookUpdates {
    Symbol symbol;
    BookUpdateVector updateVec;
    Timestamp stamp;
    Timestamp transact;
    Timestamp sending_time;
#ifdef PERF_STATS
    exchange::MetaData  metaData;
#endif
//    BookUpdates(const BookUpdates &other) : symbol(other.symbol), updateVec(other.updateVec), stamp(other.stamp) {}
    BookUpdates(const Symbol &symbol) : symbol(symbol)
    {}
    BookUpdates(const Symbol &symbol, const BookUpdateVector &updates)
        : symbol(symbol), updateVec(std::move(updates))
    {}
    BookUpdates(const Symbol &symbol, const BookUpdateVector &updates, const Timestamp &timestamp)
        : symbol(symbol), updateVec(std::move(updates)), stamp(std::move(timestamp))
    {}
//    BookUpdates& pnt::common::BookUpdates::operator=(pnt::common::BookUpdates&& other){
//        return BookUpdates(other.symbol);
//    }
};

///////////////////////////////////////////////////////////
// Book Updates Vector

// stream operator for quote vectors
inline std::ostream & operator<<(std::ostream &out, const BookUpdateVector &quotes) {
    std::copy(quotes.begin(), quotes.end(), std::ostream_iterator<BookUpdate>(out, "\n"));

    return out;
}


} /* namespace common */ } /* namespace pnt */

#endif//PNT_COMMON_BOOK_UPDATE_HPP



