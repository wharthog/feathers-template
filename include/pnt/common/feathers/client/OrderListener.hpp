#ifndef PNT_FEATHERS_ORDER_LISTENER_HPP
#define PNT_FEATHERS_ORDER_LISTENER_HPP

#include <pnt/common/exchange/Ack.hpp>
#include <pnt/common/exchange/Reject.hpp>
#include <pnt/common/exchange/CxlAck.hpp>
#include <pnt/common/exchange/CxlReject.hpp>
#include <pnt/common/exchange/Fill.hpp>
#include <pnt/common/exchange/Replaced.hpp>
#include <pnt/common/exchange/ReplaceReject.hpp>
#include <pnt/common/exchange/Done.hpp>

namespace pnt { namespace common { namespace feathers { namespace client {

class OrderListener {
public:
    virtual ~OrderListener() {}
    virtual void onMessage( const pnt::common::exchange::Ack & ) = 0;
    virtual void onMessage( const pnt::common::exchange::Reject & ) = 0;
    virtual void onMessage( const pnt::common::exchange::CxlAck & ) = 0;
    virtual void onMessage( const pnt::common::exchange::CxlReject & ) = 0;
    virtual void onMessage( const pnt::common::exchange::Fill & ) = 0;
    virtual void onMessage( const pnt::common::exchange::Replaced &  ) = 0;
    virtual void onMessage( const pnt::common::exchange::ReplaceReject & ) = 0;
    virtual void onMessage( const pnt::common::exchange::Done & ) = 0;

};

} } } }

#endif//PNT_FEATHERS_ORDER_LISTENER_HPP
