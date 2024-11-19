#ifndef PNT_COMMON_TIMESTAMP_HPP_
#define PNT_COMMON_TIMESTAMP_HPP_

#pragma push_macro("likely")
#undef likely
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma pop_macro("likely")
#include <ctime>
#include <chrono>
#include <string>
#include <iostream>
#include <ostream>
#include <sstream> 

    // Standardize timestamp

namespace pnt { namespace common {

namespace time { 
    namespace sc = std::chrono;
    using Clock = sc::high_resolution_clock;
    using Time = sc::time_point<Clock>;
    using Duration = Clock::duration;
    using Date = boost::gregorian::date;
    constexpr Time _invalid = Time::max();

    inline long total_seconds(Duration dur) {
        return sc::duration_cast<sc::seconds>(dur).count();
    }
    inline long long total_millis(Duration dur) {
        return sc::duration_cast<sc::milliseconds>(dur).count();
    }
    inline long long total_usec(Duration dur) { 
        return sc::duration_cast<sc::microseconds>(dur).count();
    }
    inline long long total_nanos(Duration dur) { 
        return sc::duration_cast<sc::nanoseconds>(dur).count();
    }
    inline Duration hours(long count) { return sc::hours(count); }
    inline Duration minutes(long count) { return sc::minutes(count); }
    inline Duration seconds(long long count) { return sc::seconds(count); }
    inline Duration milliseconds(long long count) { return sc::milliseconds(count); }
    inline Duration microseconds(long long count) { return sc::microseconds(count); }
    inline Duration nanoseconds(long long count) { return sc::nanoseconds(count); }
    //inline Time now() { return sc::system_clock::now(); }
}

struct Timestamp {
    using Date = time::Date;
    using Duration = time::Duration;
    using ptime = boost::posix_time::ptime;

    // Member variables
    time::Time _time;

    // Construction

    Timestamp() noexcept : _time(std::chrono::system_clock::now()) {}
    Timestamp(const time::Time& p) noexcept : _time(p) {}
    Timestamp(const ptime& pt) noexcept : _time(from_ptime(pt)) {}
    Timestamp(const Timestamp& stamp) noexcept : _time(stamp.getTime()) {}
    Timestamp(const Duration duration) noexcept : _time(duration) {}
    Timestamp(long long int microsFromEpoch) noexcept : _time(time::microseconds(microsFromEpoch)) {}
    Timestamp(int years, int months, int days, Duration duration) 
        : _time(from_ptime(ptime(Date(years, months, days))) + duration) {}
    Timestamp(int seconds, int micros) noexcept : _time( time::seconds(seconds) + time::microseconds(micros) ) {}
    Timestamp(Date date) noexcept: _time(from_ptime(ptime(date))) {}
    Timestamp(Date date, Duration duration) noexcept : _time(from_ptime(ptime(date)) + duration) {}
    Timestamp(const Date& date, const std::string& duration) noexcept
        : _time(from_ptime(ptime(date, boost::posix_time::duration_from_string(duration)))) {}
    Timestamp(const std::string& date, const std::string& duration) noexcept : 
            _time(from_ptime(ptime(boost::gregorian::from_undelimited_string(date), 
                  boost::posix_time::duration_from_string(duration)))) {}

    // Static methods

    static Timestamp now() noexcept {
        return Timestamp();
    }
    static Timestamp nowSecond() noexcept {
        return Timestamp(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()));
    }
    static Timestamp local() { 
        return Timestamp().utc_to_local(); 
    }
    static Date today() { return boost::gregorian::day_clock::universal_day(); }
    static Date localToday() { return boost::gregorian::day_clock::local_day(); }
    static Timestamp invalid() { return Timestamp(time::_invalid); }

    // From integer representation of HHMMSS (hours-minutes-seconds) NOTE: THIS IS SLOW
    // Returns a local timestamp (133000 -> 2017-xx-xx 13:30:00), must call to_utc() if
    // you want it in utc terms for comparisons
    static Timestamp fromLocalHHMMSS(uint64_t hhmmss, bool expectedInFuture)
    {
        namespace bpt = boost::posix_time;
        int hours = hhmmss / 10000;
        int mins = (hhmmss / 100) % 100;
        int secs = hhmmss % 100;
        bpt::ptime pt(localToday(), bpt::time_duration(hours, mins, secs));
        auto localts = bpt::microsec_clock::local_time();
        if(expectedInFuture && pt < localts)
            pt = pt + boost::gregorian::days(1);
        return from_ptime(pt);
    }
    static Timestamp fromLocalHHMMSS(uint64_t hhmmss)
    {
        return fromLocalHHMMSS(hhmmss, false);
    }

    static Timestamp from_iso(std::string iso_datetime){
        auto pt = boost::posix_time::from_iso_string(iso_datetime);
        return from_ptime(pt);
    }

    static Timestamp fromNanos(int64_t nanos)
    {
        return Timestamp(std::chrono::nanoseconds(nanos));
    }
    static Timestamp fromNanos(uint64_t nanos)
    {
        return Timestamp(std::chrono::nanoseconds(nanos));
    }

    static time::Time from_ptime(const boost::posix_time::ptime& from) 
    {
        auto time_since_epoch = from - boost::posix_time::from_time_t(0); 
        time::Time t = std::chrono::system_clock::from_time_t(time_since_epoch.total_seconds()); 
        long nsec = time_since_epoch.fractional_seconds() * (1000000000/time_since_epoch.ticks_per_second()); 
        return t + time::nanoseconds(nsec); 
    }

    static std::string today_string() {
        auto ts = Timestamp(Timestamp::localToday());
        return ts.to_string();
    }


    ////////////////////////////////
    // Interface

    const time::Time& getTime() const {
        return _time;
    }

    const time::Date getDate() const {
        return to_ptime().date();
    }

    Duration sinceEpoch() const {
        return (_time.time_since_epoch());
    }

    uint64_t totalMics() const {  
        return std::chrono::duration_cast< std::chrono::microseconds >(_time.time_since_epoch()).count();
    }

    uint64_t totalNanos() const {
        return std::chrono::duration_cast< std::chrono::nanoseconds >(_time.time_since_epoch()).count();
    }

    // Number of seconds since epoch
    uint64_t totalSeconds() const noexcept {
        return std::chrono::duration_cast< std::chrono::seconds >(_time.time_since_epoch()).count();
    }


    // Number of days since epoch
    uint64_t totalDays() const noexcept {
        using days = std::chrono::duration<int64_t, std::ratio<86400>>;
        return std::chrono::duration_cast< days >(_time.time_since_epoch()).count();
    }

    ////////////////////////////////
    // struct tm sorta fields

    // Number of seconds elapsed today
    uint64_t seconds_today() const noexcept {
        return totalSeconds() % (24LL * 60LL * 60LL);
    }

    // Number of hours today
    uint hours() const noexcept {
        return seconds_today() / 3600;
    }

    // Number of minutes this hour
    uint minutes() const noexcept {
        return (seconds_today() % 3600) / 60;
    }

    // Number of seconds this minute
    uint seconds() const noexcept {
        return seconds_today() % 60;
    }

    // Number of milliseconds since last second
    uint microseconds() const noexcept {
        return totalMics() % 1000000; // is this right?
    }

    // Number of nanoseconds since last second
    uint64_t nanoseconds() const noexcept {
        return totalNanos() % 1000000000; // is this right?
    }


    ////////////////////////////////
    // Utility methods

    // https://stackoverflow.com/questions/4910373/interoperability-between-boostdate-time-and-stdchrono
    const boost::posix_time::ptime to_ptime() const 
    {
        namespace bpt = boost::posix_time;
        auto d = totalNanos();
        auto sec = d / 1000000000;
        auto nsec = d%1000000000; 
        return bpt::from_time_t(0) + 
               bpt::seconds(static_cast<long>(sec)) +
        #ifdef BOOST_DATE_TIME_HAS_NANOSECONDS 
               bpt::nanoseconds(nsec); 
        #else 
               bpt::microseconds((nsec+500)/1000); 
        #endif 
    }
    // crazy slow
    Timestamp utc_to_local() const {
        typedef boost::date_time::local_adjustor< boost::posix_time::ptime, -6, boost::posix_time::us_dst > us_central;
        auto pt = us_central::utc_to_local( to_ptime() );
        return from_ptime(pt);
    }

    // also crazy slow
    Timestamp to_utc() const {
        typedef boost::date_time::local_adjustor< boost::posix_time::ptime, -6, boost::posix_time::us_dst > us_central;
        auto pt = us_central::local_to_utc( to_ptime() );
        return from_ptime(pt);
    }

    // Is this a valid time
    bool isDateTime() const {
        return !isNotDateTime();
    }
    bool isNotDateTime() const {
        return _time == time::_invalid;
    }

    // slow
    std::string to_string() const {
        return (isDateTime() ? boost::posix_time::to_iso_string(to_ptime()) : std::string("NaT"));
    }
    
};

///////////////////////////////////////////////////////////
// Timestamp operators


// Equals operator
inline bool operator==(const Timestamp& lhs, const Timestamp& rhs) {
    return lhs.getTime() == rhs.getTime();
}

// Not equals operator
inline bool operator!=(const Timestamp& lhs, const Timestamp& rhs) {
    return !operator==(lhs, rhs);
}

// Less than operator
inline bool operator< (const Timestamp& lhs, const Timestamp& rhs) {
    return lhs.getTime() < rhs.getTime();
}

// Greater than operator
inline bool operator> (const Timestamp& lhs, const Timestamp& rhs) {
    return  operator<(rhs, lhs);
}

// Less than or equals operator
inline bool operator<=(const Timestamp& lhs, const Timestamp& rhs) {
    return !operator>(lhs, rhs);
}

// Greater than or equals operator
inline bool operator>=(const Timestamp& lhs, const Timestamp& rhs) {
    return !operator<(lhs, rhs);
}

inline Timestamp::Duration operator-(const Timestamp& lhs, const Timestamp& rhs) {
    return lhs.getTime() - rhs.getTime();
}

inline std::ostream & operator<<(std::ostream &out, const Timestamp &timestamp) {
    return out << timestamp.to_string();
}

inline Timestamp operator-(const Timestamp& lhs, const Timestamp::Duration& rhs) {
    return lhs.getTime() - rhs;
}

inline Timestamp operator+(const Timestamp& lhs, const Timestamp::Duration& rhs) {
    return lhs.getTime() + rhs;
}

} /* namespace common */ } /* namspace pnt */

#endif /* PNT_COMMON_TIMESTAMP_HPP_ */
