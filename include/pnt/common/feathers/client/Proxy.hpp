#ifndef PNT_COMMON_FEATHERS_CLIENT_PROXY_HPP
#define PNT_COMMON_FEATHERS_CLIENT_PROXY_HPP

#include <pnt/common/Types.hpp>
#include <pnt/common/exchange/NewOrder.hpp>
#include <pnt/common/exchange/CancelOrder.hpp>
#include <pnt/common/exchange/ReplaceOrder.hpp>
#include <pnt/common/feathers/client/AdminListener.hpp>
#include <pnt/common/feathers/client/QuoteListener.hpp>
#include <pnt/common/feathers/client/OrderListener.hpp>

#include <boost/noncopyable.hpp>
#include <memory>

namespace pnt { namespace common { namespace feathers { namespace client {

struct ProxyImpl;

class Proxy : public boost::noncopyable {
public:
    Proxy( const std::string &host,
           const std::string &port,
           std::shared_ptr< AdminListener > adminListener,
           std::shared_ptr< QuoteListener > quoteListener,
           std::shared_ptr< OrderListener > orderListener );

    std::string version();

    void connect(bool requestOrderEntry);
    void disconnect();

    void waitForSignal();
    void stop();

    int place( common::exchange::NewOrder order );
    void cancel( common::exchange::CancelOrder cancel);
    int modify( common::exchange::ReplaceOrder modify );

    // void getPosPnl( std::vector<std::string> symbols );
    void sendSettlement( std::string symbol, common::Price price );

    // double getDisplayFactor( std::string symbol );
    // int getMarketDepth( std::string symbol );
    // int getPosLimit( std::string symbol );

    void addFunction( std::function<void()> fn );

private:
    std::shared_ptr< ProxyImpl > impl;
};

} /* namespace client */ } /* namespace feathers */ } /* namespace common */ } /* namespace pnt */

#endif //PNT_COMMON_FEATHERS_CLIENT_PROXY_HPP
