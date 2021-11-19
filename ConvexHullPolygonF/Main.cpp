#include <iostream>
#include <list>
#include <vector>

//TODO:
//if there are multiple points in a row all part of the polygon, only the outer ones are important
//check for all points in cornorPoints if there are any between two other

struct Point
{
	//point = 2 values on cartesian system
	float X, Y;

	//Constructor
	Point()
	{
		X = 0.0f;
		Y = 0.0f;
	}
	Point(float x, float y)
	{
		X = x;
		Y = y;
	}

	//Compare with other point whether equal
	bool isEqual(Point p0)
	{
		if (X == p0.X && Y == p0.Y) return true;
		else return false;
	}
};

std::list<Point> CreatePointList()
{
	std::list<Point> pointList;//pointlist
	//points
	Point p0(1, 3.5);
	Point p1(5, 5.5);
	Point p2(2.5, 1);
	Point p3(4, 2.5);
	Point p4(4.5, 3);
	Point p5(4.5, 5.5);
	Point p6(5, 4);
	Point p7(7, 3.5);
	Point p8(6.5, 1.5);
	Point p9(2.5, 5.5);

	pointList.push_back(p0);
	pointList.push_back(p1);
	pointList.push_back(p2);
	pointList.push_back(p3);
	pointList.push_back(p4);
	pointList.push_back(p5);
	pointList.push_back(p6);
	pointList.push_back(p7);
	pointList.push_back(p8);
	pointList.push_back(p9);

	return pointList;
}

float* getArea(std::list<Point>* pointList)
{
	//startValues: must lay inside the final area -> use first points coordinates
	float minX = (*pointList->begin()).X;
	float maxX = minX;
	float minY = (*pointList->begin()).Y;
	float maxY = minY;

	for (Point p : *pointList)
	{
		//test for every Point in the list, if theres a new extremum
		if (minX > p.X) minX = p.X;
		if (maxX < p.X) maxX = p.X;
		if (minY > p.Y) minY = p.Y;
		if (maxY < p.Y) maxY = p.Y;
	}

	//create Array whose adress will be returned
	static float Range[4];
	Range[0] = minX;
	Range[1] = maxX;
	Range[2] = minY;
	Range[3] = maxY;
	return Range;

}

int GetOrientation(Point* p0, Point* p1, Point* p2)
{
	enum Orientation : int
	{
		Collinear = 0, Clockwise, Counterclockwise
	};

	float value = ((*p1).Y - (*p0).Y) * ((*p2).X - (*p1).X) - ((*p1).X - (*p0).X) * ((*p2).Y - (*p1).Y);
	if (value == 0) return Collinear;
	else if (value > 0) return Clockwise;
	else return Counterclockwise;
}

enum AreaPos : int
{
	MinimumX = 0, MaximumX, MinimumY, MaximumY
};

std::ostream& operator<<(std::ostream& stream, const Point& point)
{
	stream << "(" << point.X << "/" << point.Y << ")";
	return stream;
}

int main()
{
	//get pointlist
	std::list<Point> pointList = CreatePointList();
	float* extremum;
	extremum = getArea(&pointList);

	//Rectangle: 4 corner Points:
	Point A(extremum[AreaPos::MinimumX], extremum[AreaPos::MinimumY]);
	Point B(extremum[AreaPos::MaximumX], extremum[AreaPos::MinimumY]);
	Point C(extremum[AreaPos::MaximumX], extremum[AreaPos::MaximumY]);
	Point D(extremum[AreaPos::MinimumX], extremum[AreaPos::MaximumY]);

	//list with all corner Points (unknown number)
	std::list<Point> cornerPoints;
	Point middle(extremum[AreaPos::MinimumX] / 2 + extremum[AreaPos::MaximumX] / 2, extremum[AreaPos::MinimumY] / 2 + extremum[AreaPos::MaximumY] / 2);
	Point testPnt = middle;
	//find start Point
	//Point cornerPnt(0, 0);
	//cornerPnt = middle;
	for (Point p : pointList)
	{
		if (p.X == extremum[AreaPos::MinimumX])//this point will certainly be part of the polygon, but maybe in a line surrounded by other corner points
		{
			if (testPnt.X == extremum[AreaPos::MinimumX] && p.Y >= testPnt.Y) continue;//if there are several points with minimumX and already one selected: if selected has lower y dont change
			testPnt = p;
		}
	}
	cornerPoints.push_back(testPnt);//startpoint to build polygon
	testPnt = middle;

	//test for every Point whether it is at the right hand side of the line cornerpoint middle -> if yes chose as reference and go on to find the rightest one
	while (1)
	{
		for (Point p : pointList)
		{
			if (GetOrientation(cornerPoints.end(), &testPnt, &p) == 1)//point p is further at the right side
			{
				testPnt = p;
			}
		}
		if ((*cornerPoints.begin()).isEqual(testPnt)) break;
		cornerPoints.push_back(testPnt);
		//cornerPnt = testPnt;
		testPnt = middle;
	}

	//Print all polygon points
	std::cout << "Rectangle points: ";
	std::cout << A << B << C << D;
	std::cout << std::endl;

	std::cout << "Polygon points: ";
	for (Point p : cornerPoints)
	{
		std::cout << p << "  ";
	}
	std::cout << std::endl;

	std::cin.get();
}