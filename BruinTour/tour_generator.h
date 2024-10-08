#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "base_classes.h"


class TourGenerator : public TourGeneratorBase 
{ 
public: 
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router); 
    virtual ~TourGenerator();
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const; 
private:
    const GeoDatabaseBase *myGeodb;
    const RouterBase *myRouter;


    std::string dir(double angle) const
    {
        if ((angle >= 0 && angle < 22.5) || (angle >= 337.5))
            return "east";
        else if (angle >= 22.5 && angle < 67.5)
            return "northeast";
        else if (angle >= 67.5 && angle < 112.5)
            return "north";
        else if (angle >= 112.5 && angle < 157.5)
            return "northwest";
        else if (angle >= 157.5 && angle < 202.5)
            return "west";
        else if (angle >= 202.5 && angle < 247.5)
            return "southwest";
        else if (angle >= 247.5 && angle < 292.5)
            return "south";
        else 
            return "southeast";
    }

};
