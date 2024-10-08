//#include "hashmap.h"
//#include "geodb.h"
//#include "router.h"
//#include <iostream>
//#include <string>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//	GeoDatabaseBase* test = new GeoDatabase();
//	test->load("C:/Users/Joaquin Chun/OneDrive/Documents/CS32/Project 4 files/minimap.txt");
//	Router path(*test);
//	GeoPoint p1("34.0709602", "-118.4425828");
//	GeoPoint p2("34.0714199", "-118.4437548");
//	std::vector<GeoPoint> myPath = path.route(p1, p2);
//	for (auto pt : myPath)
//		cerr << pt.to_string() << endl;
//
//	GeoPoint p3("34.0709693", "-118.4436074");
//	myPath = test->get_connected_points(p3);
//	cerr << endl;
//	for (auto pt : myPath)
//		cerr << pt.to_string() << endl;
//
//}