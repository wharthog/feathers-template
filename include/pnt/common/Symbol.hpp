/*
 * Symbol.hpp
 *
 *  Created on: Feb 20, 2020
 *      Author: chad
 */

#ifndef SRC_PNT_COMMON_SYMBOL_HPP_
#define SRC_PNT_COMMON_SYMBOL_HPP_

#include <pnt/common/Types.hpp>
#include <string>

namespace pnt { namespace common {

namespace sneezy {
    class Product;
}

struct Symbol {
    std::string value_;
    SecurityId id_ = common::invalid::SECURITYID;
    sneezy::Product* product_ = nullptr;

    Symbol() {}
    Symbol(const Symbol &other) : value_(other.value_), id_(other.id_), product_(other.product_) {}
    Symbol(const char* value) : value_(value) {}
    Symbol(const std::string &value) : value_(value) {}

    std::string substr(std::size_t start, std::size_t length) const
    {
        return value_.substr(start, length);
    }
    std::size_t size() const
    {
        return value_.size();
    }

    const char* c_str() const { return value_.c_str(); }
    operator std::string() const { return value_; }

    friend std::string operator+(const Symbol &a, const Symbol &b) {
        return a.value_ + b.value_;
    }
    friend bool operator==(const Symbol &a, const Symbol &b) {
        return a.value_ == b.value_;
    }
    friend bool operator!=(const Symbol &a, const Symbol &b) {
        return a.value_ != b.value_;
    }
    friend bool operator<(const Symbol &a, const Symbol &b) {
        return a.value_ < b.value_;
    }
    friend bool operator>(const Symbol &a, const Symbol &b) {
        return a.value_ > b.value_;
    }
    friend bool operator>=(const Symbol &a, const Symbol &b) {
        return a.value_ >= b.value_;
    }
    friend bool operator<=(const Symbol &a, const Symbol &b) {
        return a.value_ <= b.value_;
    }
    std::string toString() const {
        return value_ + (id_ >= 0 ? (":" + std::to_string(id_)) : "");
    }
};

inline std::ostream & operator<<(std::ostream &out, const Symbol &symbol) {
    return out << symbol.toString();
}

} }

namespace std
{
    template <>
    struct hash<pnt::common::Symbol>
    {
        size_t operator()( const pnt::common::Symbol& k ) const
        {
            return hash<string>()(k.value_);
            // if(k.product_)
            //     return hash<long>()((long)k.product_);
            // if(k.id_ != -1)
            //     return hash<int>()(k.id_);
        }
    };
}

#endif /* SRC_PNT_COMMON_SYMBOL_HPP_ */
