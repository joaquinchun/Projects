#include "router.h"
#include <algorithm>

Router::Router(const GeoDatabaseBase& geo_db)
	: Geodb(&geo_db)
{

}
Router::~Router()
{

}
std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	//Cases
	std::vector<GeoPoint> retVec;
	if (Geodb->get_connected_points(pt1).size() == 0 || Geodb->get_connected_points(pt2).size() == 0)
		return retVec;
	else if (pt1.to_string() == pt2.to_string())
	{
		retVec.push_back(pt1);
		return retVec;
	}

	//Main
	Search* myStart = new Search(pt1, nullptr);
	Search* myEnd = new Search(pt2, nullptr);
	myEnd->H = myStart->G = 0;
	myEnd->G = myStart->H = myStart->disFromEnd(myEnd);

	std::vector<Search*> myOpenList;
	std::vector<Search*> myClosedList;
	std::vector<GeoPoint> myFinalPath;

	myOpenList.push_back(myStart);
	Search* currentCell = myStart;

	// Loops until end or OpenList is empty 
	while (currentCell->myName != myEnd->myName)
	{
		if (myOpenList.empty())
			return retVec;

		myOpenList.erase(std::remove(myOpenList.begin(), myOpenList.end(), currentCell), myOpenList.end());
		myClosedList.push_back(currentCell);
		// For every connected GeoPoint it checks if:
		// In closed then do nothing
		// If in open and F value is less replace the value in open
		// If neither add it to open
		std::vector<GeoPoint> connectedPoints = Geodb->get_connected_points(currentCell->myPoint);
		bool alreadyExist;
		for (auto newpt : connectedPoints)
		{
			alreadyExist = false;
			for (auto closept : myClosedList)
			{
				if (closept->myName == newpt.to_string())
					alreadyExist = true;
				if (alreadyExist)
					break;
			}

			if (!alreadyExist)
			{
				alreadyExist = false;
				Search* newCell = new Search(newpt, currentCell, myStart, myEnd);
				for (auto openpt : myOpenList)
				{
					if (newCell->myName == openpt->myName && newCell->getF() < openpt->getF())
					{
						openpt->H = newCell->H;
						openpt->G = newCell->G;
						openpt->myParent = currentCell;
						alreadyExist = true;
					}
					if (alreadyExist)
						break;
				}

				if (!alreadyExist)
					myOpenList.push_back(newCell);
				else
					delete newCell;
			}
		}

		int vecIndex = nextSearch(myOpenList);
		if (vecIndex > -1)
			currentCell = myOpenList[nextSearch(myOpenList)];
	}

	// Getting Path
	Search* getPath = currentCell;
	while (getPath != nullptr)
	{
		myFinalPath.push_back(getPath->myPoint);
		getPath = getPath->myParent;
	}
	std::reverse(myFinalPath.begin(), myFinalPath.end());

	// Deallocating Memeory
	for (auto del : myOpenList)
		delete del;
	myOpenList.clear();
	for (auto del : myClosedList)
		delete del;
	myClosedList.clear();

	delete myEnd;

	return myFinalPath;
}