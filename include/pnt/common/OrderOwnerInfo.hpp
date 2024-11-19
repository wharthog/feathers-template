#ifndef PNT_COMMON_ORDER_OWNER_INFO_HPP
#define PNT_COMMON_ORDER_OWNER_INFO_HPP

#include <stdint.h>
#include <string>

namespace pnt { namespace common {

struct OrderOwnerInfo {
    typedef uint32_t Id;
    static constexpr Id NONE = 0;
    Id id;
    std::string name;
    OrderOwnerInfo()
        : id{NONE}
        , name{""}
    {}
    OrderOwnerInfo(Id id, const std::string &name)
        : id{id}
        , name{name}
    {}
};

} /* namespace common */ } /* namespace pnt */

#endif // PNT_COMMON_ORDER_OWNER_INFO_HPP
