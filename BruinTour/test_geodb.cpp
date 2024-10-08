//#include "hashmap.h"
//#include "geodb.h"
//#include <iostream>
//#include <string>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//	GeoDatabase test;
//	test.load("C:/Users/Joaquin Chun/OneDrive/Documents/CS32/Project 4 files/minimap.txt");
//	GeoPoint p1("34.0736122", "-118.4927669");
//	std::vector<GeoPoint> vec;
//	vec = test.get_connected_points(p1);
//	assert(vec[0].to_string() == "34.0732851,-118.4931016");
//	assert(vec[1].to_string() == "34.0731714,-118.4921918");
//	assert(vec[2].to_string() == "34.0739890,-118.4931789");
//
//
//	GeoPoint p2("34.0601422", "-118.4468929");
//	vec = test.get_connected_points(p2);
//	assert(vec[0].to_string() == "34.0600768,-118.4467216");
//
//	GeoPoint p3("34.0600768", "-118.4467216");
//	vec = test.get_connected_points(p3);
//	assert(vec[0].to_string() == "34.0602175,-118.4464952");
//	assert(vec[1].to_string() == "34.0599361,-118.4469479");
//	assert(vec[2].to_string() == "34.0601422,-118.4468929");
//	assert(vec[3].to_string() == "34.0591552,-118.4463759");
//
//	cerr << "Passed get connected points" << endl;
//
//	GeoPoint p4("34.0732851", "-118.4931016");
//	GeoPoint p5("34.0736122", "-118.4927669");
//	assert(test.get_street_name(p4, p5) == "Glenmere Way");
//
//	GeoPoint p6("34.0601422", "-118.4468929"); 
//	GeoPoint p7("34.0600768", "-118.4467216");
//	assert(test.get_street_name(p6, p7) == "a path");
//
//	GeoPoint p8("34.0602175", "-118.4464952"); 
//	GeoPoint p9("34.0600768", "-118.4467216");
//	assert(test.get_street_name(p8, p9) == "Kinross Avenue");
//
//	cerr << "Passed get street name" << endl;
//
//	GeoPoint p10;
//	assert(test.get_poi_location("Fatburger", p10));
//	assert(p10.to_string() == "34.0601422,-118.4468929");
//	cerr << "Passed get poi location" << endl;
//}


