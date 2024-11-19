#include <pnt/common/feathers/client/AdminListener.hpp>
#include <pnt/common/feathers/client/QuoteListener.hpp>
#include <pnt/common/feathers/client/OrderListener.hpp>
#include <pnt/common/feathers/client/Proxy.hpp>
#include <pnt/common/exchange/NewOrder.hpp>
#include <pnt/common/exchange/CancelOrder.hpp>

#include <memory>

// #include <functional>
#include <iostream>
#include <sstream>

inline std::string thread_id(){
    std::stringstream ss;
    ss << "[" << boost::this_thread::get_id() << "] ";
    std::string result = ss.str();
    return result;
}

inline void log_line(const std::string& function){
    std::cout << thread_id() << function << std::endl;
}

inline void log_line(const std::string& function, const std::string& line){
    std::cout << thread_id() << function << ": " << line << std::endl;
}

template<typename T>
inline void log_obj(const std::string& function, const T& obj){
    std::cout << thread_id() << function << ": " << obj << std::endl;
}


struct Tester
    : pnt::common::feathers::client::AdminListener
    , pnt::common::feathers::client::QuoteListener
    , pnt::common::feathers::client::OrderListener
{
    Tester()
        : proxy{}
        , canPlace{false}
        , placed{false}
    {}

    // {{{ AdminListener
    void onConnected(bool canPlace) override {
        log_line("AdminListener::onConnected()",
            "canPlace=" + std::to_string(canPlace));
        if(!placed){ this->canPlace = canPlace; }
    }

    void onDisconnected() override {
        log_line("AdminListener::onDisconnected()");
    }

    void onPosPnl(const std::string &symbol, int position, double pnl, double openPnl, double closedPnl) override {
        // log_line("AdminListener::onPosPnl()",
        //     "symbol=" + symbol +
        //     ", position=" + std::to_string(position) +
        //     ", pnl=" + std::to_string(pnl) +
        //     ", openPnl=" + std::to_string(openPnl) +
        //     ", closedPnl=" + std::to_string(closedPnl));
    }

    void onPortfolioPosPnl(int position, double pnl, double openPnl, double closedPnl) override {
        // log_line("AdminListener::onPortfolioPosPnl()",
        //     "position=" + std::to_string(position) +
        //     ", pnl=" + std::to_string(pnl) +
        //     ", openPnl=" + std::to_string(openPnl) +
        //     ", closedPnl=" + std::to_string(closedPnl));
    }

    void onAlert(const std::string &alert, const pnt::common::Timestamp &timestamp) override {
        log_line("AdminListener::onAlert()",
            "alert=" + alert +
            ", timestamp=" + timestamp.to_string());
    }
    // }}} AdminListener

    // {{{ QuoteListener
    void onMessage(const pnt::common::BookUpdates& updates) override {
        // log_line("QuoteListener::onMessage(BookUpdates)",
        //     "symbol=" + updates.symbol +
        //     ", timestamp=" + updates.stamp.to_string());
        // for (size_t i = 0; i < updates.updateVec.size(); ++i){
        //     std::cout << "\t" << updates.updateVec[i] << std::endl;
        // }
        if(!placed && canPlace){
            this->canPlace = false;
            this->placed = true;
            pnt::common::OrderTxnId orderId = 666;
            pnt::common::Side::Enum side = pnt::common::Side::Bid;
            pnt::common::Symbol symbol = updates.symbol;
            pnt::common::Qty qty = 1;
            // pnt::common::Price price = 4470; // updates.updateVec[0].quote.price;
            pnt::common::Price price = updates.updateVec[0].quote.price;
            pnt::common::exchange::NewOrder newOrder(orderId, side, symbol, qty, price);
            log_obj("PLACE", newOrder);
            this->proxy->place( newOrder );
            log_line(__FUNCTION__, "PLACE");
        }
    }
    // }}} QuoteListener

    // {{{ OrderListener
    void onMessage(const pnt::common::exchange::Ack& ack) override {
        log_obj("OrderListener::onMessage(Ack)", ack);
        pnt::common::OrderTxnId txnId = ack.orderId + 66600000;
        pnt::common::OrderTxnId orderId = ack.orderId;
        pnt::common::exchange::CancelOrder cancelOrder(txnId , orderId);
        log_obj("CANCEL", cancelOrder);
        proxy->cancel(cancelOrder);
    }

    void onMessage(const pnt::common::exchange::Reject& reject) override {
        log_obj("OrderListener::onMessage(Reject)", reject);
    }

    void onMessage(const pnt::common::exchange::CxlAck& cxlAck) override {
        log_obj("OrderListener::onMessage(CxlAck)", cxlAck);
    }

    void onMessage(const pnt::common::exchange::CxlReject& cxlReject) override {
        log_obj("OrderListener::onMessage(CxlReject)", cxlReject);
    }

    void onMessage(const pnt::common::exchange::Fill& fill) override {
        log_obj("OrderListener::onMessage(Fill)", fill);
    }

    void onMessage(const pnt::common::exchange::Replaced& replaced) override {
        log_obj("OrderListener::onMessage(Replaced)", replaced);
    }

    void onMessage(const pnt::common::exchange::ReplaceReject& replaceReject) override {
        log_obj("OrderListener::onMessage(ReplaceReject)", replaceReject);
    }

    void onMessage(const pnt::common::exchange::Done& done) override {
        log_obj("OrderListener::onMessage(Done)", done);
    }
    // }}} OrderListener

    void initProxy(std::shared_ptr<pnt::common::feathers::client::Proxy> proxy){
        this->proxy = proxy;
    }

    std::shared_ptr<pnt::common::feathers::client::Proxy> proxy;
    bool canPlace;
    bool placed;
};

int main(int argc, char *argv[]) {
    std::string host = "";
    std::string port = "";
    // std::string host = "10.3.14.7";
    // std::string port = "9660";
    log_line(__FUNCTION__, "make tester...");
    auto tester = std::make_shared<Tester>();
    log_line(__FUNCTION__, "make proxy...");
    auto proxy = std::make_shared<pnt::common::feathers::client::Proxy>(host, port, tester, tester, tester);
    log_line(__FUNCTION__, "init...");
    tester->initProxy( proxy );
    log_line(__FUNCTION__, "\tversion=" + tester->proxy->version());
    log_line(__FUNCTION__, "connect...");
    // tester->proxy->connect(true);
    tester->proxy->connect(false);
    log_line(__FUNCTION__, "wait...(control-c to interrupt)");
    tester->proxy->waitForSignal();
    log_line(__FUNCTION__, "disconnect...");
    tester->proxy->disconnect();
    log_line(__FUNCTION__, "stop...");
    tester->proxy->stop();
    return 0;
}
