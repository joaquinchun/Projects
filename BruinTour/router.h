#pragma once
#include "base_classes.h"
#include <cmath>

class Router : public RouterBase
{ 
public:   
	Router(const GeoDatabaseBase& geo_db);
	virtual ~Router();
	virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
	struct Search
	{
		GeoPoint myPoint;
		Search* myParent;
		double G, H;
		std::string myName;
		Search* start;
		Search* end;

		Search(const GeoPoint& p, Search* parent,Search *start_ = nullptr,Search *end_ = nullptr)
			: myPoint(p), myParent(parent), myName(myPoint.to_string()), start(start_), end(end_)
		{
			if (start == nullptr || end == nullptr)
				G = H = 0;
			else
			{
				H = disFromEnd(end);
				G = disFromStart(start);
			}
		}

		bool operator<(const Search* other)
		{
			return (this->getF() < other->getF());
		}

		double getF() const
		{
			return G + H;
		}

		double disFromEnd(Search* end)
		{
			double myLat, myLong, endLat, endLong;
			size_t comma;
			comma = myPoint.to_string().find(',');
			myLat = stod(myPoint.to_string().substr(0, comma));
			myLong = stod(myPoint.to_string().substr(comma + 1));

			comma = end->myPoint.to_string().find(',');
			endLat = stod(end->myPoint.to_string().substr(0, comma));
			endLong = stod(end->myPoint.to_string().substr(comma + 1));

			return sqrt(pow(endLat - myLat, 2) + pow(endLong - myLong, 2));
		}
		double disFromStart(Search* start)
		{
			double myLat, myLong, startLat, startLong;
			size_t comma;
			comma = myPoint.to_string().find(',');
			myLat = stod(myPoint.to_string().substr(0, comma));
			myLong = stod(myPoint.to_string().substr(comma + 1));

			comma = start->myPoint.to_string().find(',');
			startLat = stod(start->myPoint.to_string().substr(0, comma));
			startLong = stod(start->myPoint.to_string().substr(comma + 1));

			return sqrt(pow(myLat-startLat, 2) + pow(myLong-startLong, 2));
		}
	};


	int nextSearch(std::vector<Search*> vecOpen) const
	{
#ifdef _WIN32
#include <cfloat>
		double FVal = DBL_MAX;
#else
#include <limits>
		double FVal = std::numeric_limits<double>::max();
#endif
		int vecIndex = -1;
		for (int i = 0; i < vecOpen.size(); ++i)
			if (vecOpen[i]->getF() < FVal)
			{
				FVal = vecOpen[i]->getF();
				vecIndex = i;
			}
		return vecIndex;
	}

	const GeoDatabaseBase* Geodb;
};

