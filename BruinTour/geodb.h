#pragma once

#include "base_classes.h"
#include "hashmap.h"
#include "geotools.h"
#include <vector>
#include <unordered_map>
class GeoDatabase : public GeoDatabaseBase
{
public:
	GeoDatabase();
	virtual bool load(const std::string& map_data_file);
	virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
	virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
	virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:

	HashMap<GeoPoint> myPOI;
	HashMap<std::vector<GeoPoint>> myConnectedPoints;
	HashMap<std::string> myStreetName;

	//HashMap<std::vector<GeoPoint>> myStreets;
	//HashMap<std::vector<std::string>> myPtoNames;
	//HashMap<std::string> myMidPoints;
};

