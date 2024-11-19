#ifndef PNT_COMMON_FEATHERS_CLIENT_ADMIN_LISTENER_HPP
#define PNT_COMMON_FEATHERS_CLIENT_ADMIN_LISTENER_HPP

#include <string>
#include <pnt/common/Timestamp.hpp>

namespace pnt { namespace common { namespace feathers { namespace client {

struct AdminListener {
    virtual ~AdminListener() {}
    virtual void onConnected(bool canPlace) = 0;
    virtual void onDisconnected() = 0;
    virtual void onPosPnl(const std::string &symbol, int position, double pnl, double openPnl, double closedPnl) = 0;
    virtual void onPortfolioPosPnl(int position, double pnl, double openPnl, double closedPnl) = 0;
    virtual void onAlert(const std::string &alert, const common::Timestamp &timestamp) = 0;
};

} /* namespace client */ } /* namespace feathers */ } /* namespace common */ } /* namespace pnt */

#endif //PNT_COMMON_FEATHERS_CLIENT_ADMIN_LISTENER_HPP
