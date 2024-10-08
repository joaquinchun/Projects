#include "geodb.h"


GeoDatabase::GeoDatabase()
{

}
bool GeoDatabase::load(const std::string& map_data_file)
{
	std::ifstream mapData(map_data_file);
	std::string name, poi, streetname;
	std::string lat1, lat2, long1, long2, numStrPoi, pointStr;
	int numPoi;

	std::vector<GeoPoint>* vecGeoPtr1;
	std::vector<GeoPoint>* vecGeoPtr2 = nullptr;

	if (!mapData.is_open())
		return false;
	while (mapData.peek() != EOF)
	{
		mapData >> std::ws;
		std::getline(mapData, streetname);
		mapData >> lat1;
		mapData >> long1;
		mapData >> lat2;
		mapData >> long2;
		mapData.ignore(256, '\n');
		mapData >> numStrPoi;
		numPoi = stoi(numStrPoi);
		GeoPoint p1(lat1, long1);
		GeoPoint p2(lat2, long2);

		pointStr = p1.to_string() + " " + p2.to_string();
		myStreetName.insert(pointStr, streetname);

		vecGeoPtr1 = myConnectedPoints.find(p1.to_string());
		if (vecGeoPtr1 == nullptr)
		{
			std::vector<GeoPoint> tempVec;
			tempVec.push_back(p2);
			myConnectedPoints.insert(p1.to_string(), tempVec);
		}
		else
			vecGeoPtr1->push_back(p2);

		vecGeoPtr2 = myConnectedPoints.find(p2.to_string());
		if (vecGeoPtr2 == nullptr)
		{
			std::vector<GeoPoint> tempVec;
			tempVec.push_back(p1);
			myConnectedPoints.insert(p2.to_string(), tempVec);
		}
		else
		{
			vecGeoPtr2->push_back(p1);
		}
		mapData.ignore(256, '\n');
		while (numPoi > 0)
		{
			std::getline(mapData, name);
			poi = name.substr(0, name.find('|'));
			size_t nextPart = name.find('|') + 1;
			lat1 = name.substr(nextPart, name.find(' ', nextPart) - nextPart);
			long1 = name.substr(name.find(' ', nextPart) + 1);
			GeoPoint p3(lat1, long1);
			myPOI.insert(poi, p3);
			GeoPoint midPoint = midpoint(p1, p2);

			vecGeoPtr1 = myConnectedPoints.find(p1.to_string()); // Might not need
			vecGeoPtr2 = myConnectedPoints.find(p2.to_string()); // Same
			bool add;

			add = true;
			for (std::vector<GeoPoint>::iterator it = vecGeoPtr1->begin(); it != vecGeoPtr1->end(); ++it)
				if (midPoint.to_string() == it->to_string())
					add = false;
			if (add)
				vecGeoPtr1->push_back(midPoint);

			add = true;
			for (std::vector<GeoPoint>::iterator it = vecGeoPtr2->begin(); it != vecGeoPtr2->end(); ++it)
				if (midPoint.to_string() == it->to_string())
					add = false;
			if (add)
				vecGeoPtr2->push_back(midPoint);

			std::vector<GeoPoint>* vecGeoMidPtr = myConnectedPoints.find(midPoint.to_string());
			if (vecGeoMidPtr == nullptr)
			{
				std::vector<GeoPoint> vecGeoMid;
				vecGeoMid.push_back(p1);
				vecGeoMid.push_back(p2);
				vecGeoMid.push_back(p3);
				myConnectedPoints.insert(midPoint.to_string(), vecGeoMid);
			}
			else
				vecGeoMidPtr->push_back(p3);

			std::vector<GeoPoint> vecGeoPoi;
			vecGeoPoi.push_back(midPoint);
			myConnectedPoints.insert(p3.to_string(), vecGeoPoi);
			myStreetName.insert(p1.to_string() + " " + midPoint.to_string(), streetname);
			myStreetName.insert(p2.to_string() + " " + midPoint.to_string(), streetname);
			myStreetName.insert(midPoint.to_string() + " " + p3.to_string(), "a path");
			--numPoi;
		}

	}
	return true;
}
bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
	const GeoPoint* temp = myPOI.find(poi);
	if (temp == nullptr)
		return false;
	else
	{
		point = *temp;
		return true;
	}
}
std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
	const std::vector<GeoPoint>* temp = myConnectedPoints.find(pt.to_string());
	std::vector<GeoPoint> returnVec;
	if (temp == nullptr)
		return returnVec;
	else
	{
		returnVec = *temp;
		return returnVec;
	}
}
std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	const std::string *temp = myStreetName.find(pt1.to_string() + " " + pt2.to_string());
	if (temp == nullptr)
	{
		temp = myStreetName.find(pt2.to_string() + " " + pt1.to_string());
		if (temp == nullptr)
		{
			return "";
		}
		else
			return *temp;
	}
	else
		return *temp;
}