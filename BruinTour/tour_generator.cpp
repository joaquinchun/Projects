#include "tour_generator.h"
#include "geotools.h"

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
	: myGeodb(&geodb), myRouter(&router)
{

}
TourGenerator::~TourGenerator()
{

}
std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
	std::vector<TourCommand> myTour, empty;
	if (stops.size() == 0)
		return empty;



	std::string poiStr1, poiStr2, talkingPoints1, talkingPoints2, streetNameCurrent,streetNamePrev;
	streetNamePrev = "";
	GeoPoint geoPtr, poiPT1, poiPT2;
	std::vector<GeoPoint> theRoute;
	TourCommand command;
	int commandSize = -1;
	for (int i = 0; i < stops.size(); ++i)
	{
		stops.get_poi_data(i, poiStr1, talkingPoints1);
		if (!myGeodb->get_poi_location(poiStr1, geoPtr))
			return empty;
	}

	for (int stop = 0; stop < stops.size() - 1; ++stop)
	{
		stops.get_poi_data(stop, poiStr1, talkingPoints1);
		stops.get_poi_data(stop + 1, poiStr2, talkingPoints2);
		myGeodb->get_poi_location(poiStr1, poiPT1);
		myGeodb->get_poi_location(poiStr2, poiPT2);
		theRoute = myRouter->route(poiPT1, poiPT2);
		if (theRoute.empty())
			return empty;

		myTour.push_back(command);
		++commandSize;
		myTour[commandSize].init_commentary(poiStr1, talkingPoints1);
		GeoPoint prevPoint;

		for (int index = 0; index < theRoute.size() - 1; ++index)
		{
			streetNameCurrent = myGeodb->get_street_name(theRoute[index], theRoute[index + 1]);
			if (streetNameCurrent != streetNamePrev && streetNamePrev != "")
			{
				// Is 0 considerd only 0 or between 0 and .999999
				double ang = angle_of_turn(prevPoint, theRoute[index], theRoute[index + 1]);
				std::string turn = "";
				if (ang >= 1 && ang < 180)
					turn = "left";
				else if (ang >= 180 && ang < 359)
					turn = "right";
				else {};
				if (turn != "")
				{
					myTour.push_back(command);
					++commandSize;
					myTour[commandSize].init_turn(turn, streetNameCurrent);
				}
			}

			prevPoint = theRoute[index];
			streetNamePrev = streetNameCurrent;

			myTour.push_back(command);
			++commandSize;
			myTour[commandSize].init_proceed((dir(angle_of_line(theRoute[index], theRoute[index + 1]))),
				streetNameCurrent, distance_earth_miles(theRoute[index], theRoute[index + 1]), theRoute[index], theRoute[index + 1]);
		}
	}

	if (theRoute.size() > 1)
	{
		stops.get_poi_data(stops.size() - 1, poiStr1, talkingPoints1);
		//std::cerr << poiStr1 << "|" << talkingPoints1 << std::endl;
		myTour.push_back(command);
		++commandSize;
		myTour[commandSize].init_commentary(poiStr2, talkingPoints2);
	}
	return myTour;
}