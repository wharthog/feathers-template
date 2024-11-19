#ifndef PNT_COMMON_DIRECTION_HPP
#define PNT_COMMON_DIRECTION_HPP

#include <ostream>

namespace pnt { namespace common {

struct Direction {
    enum Enum {
        None = 1,
        Down = 2,
        Up = 3
    };
private:
    Enum direction;
public:
    Direction( const Direction &direction) : direction((Enum)direction) {}
    Direction( Enum direction) : direction(direction) {}
    operator Enum() const {
         return direction;
    }
    friend std::ostream & operator<<( std::ostream &out, const Direction &direction ) {
        switch ( direction ) {
            case None:
                out << "None";
                break;
            case Down:
                out << "Down";
                break;
            case Up:
                out << "Up";
                break;
        }
        return out;
    }
    friend std::ostream & operator<<( std::ostream &out, const Direction::Enum &direction ) {
        switch ( direction ) {
            case None:
                out << "None";
                break;
            case Down:
                out << "Down";
                break;
            case Up:
                out << "Up";
                break;
        }
        return out;
    }
    std::string toString(){
        switch ( direction ) {
            case None:
                return "None";
            case Down:
                return "Down";
            case Up:
                return "Up";
        }
    }
};

} /* namespace common */ } /* namespace pnt */

#endif //PNT_COMMON_DIRECTION_HPP
