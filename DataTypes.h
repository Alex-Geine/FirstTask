#pragma once
#include "time.h"
#include "iostream"
#include "vector"
#include "cmath"
#include "math.h"
#define PI 3.14

using namespace std;

struct Triangle;

//struct points
struct tPoint {
private:
	//coords
	double x = 0;
	double y = 0;

	//flags
	bool isExtra = false;
	bool isBorder = false;

	//potencial
	double p = 0;

	//vector with all triangles, wich includes this point
	vector<Triangle*> familyTriangles;

	//vector with all neighbours
	vector<tPoint*> neighbour;
public:
	//default constructor
	tPoint() {};

	//constructor
	tPoint(double x, double y, bool isExtra, bool isBorder) : x(x), y(y), isExtra(isExtra), isBorder(isBorder) {};

	//get x
	double X() { return x; };

	//get y
	double Y() { return y; };

	//get p
	double P() { return p; };

	//set X
	void SetX(double x) { this->x = x; };

	//set Y
	void SetY(double y) { this->y = y; };

	//set p
	void SetP(double p) { this->p = p; };

	//isExtra
	bool isExtraP() { return isExtra; };

	//isBorder
	bool isBorderP() { return isBorder; };

	//operator ==
	bool operator ==(tPoint& p) {
		if (this->x == p.x && this->y == p.y && this->isExtra == p.isExtra && this->isBorder == p.isBorder)
			return true;
		else
			return false;
	}

	//add triangle in family
	void AddTriangle(Triangle*);

	//check triangle for including this point
	bool CheckTriangleForIncluding(Triangle*);
	
	//find neigh
	void FindNeigh();

	//find Cpoyes neigh
	bool FindCopyNeigh(tPoint p);

	//getTriagnles family
	vector<Triangle*> GetFamilyTriangles() { return familyTriangles; };

	//get Neigh
	vector<tPoint*> GetNeightbours() { return neighbour; };
};

//структура эллипса
struct Ellipce {
private:
	pair<double, double> c0;	//point of center
	double a = 0;				//semiaxes
	double b = 0;				//semiaxes

	//border value
	double borderValue = 0;

	//for drawer
	double x0 = 0;
	double y0 = 0;

public:
	//constructor
	Ellipce(pair<double, double> c0, double a, double b, double bv) : c0(c0), a(a), b(b), borderValue(bv) {
		x0 = c0.first - a;
		y0 = c0.second - b;
	};

	//default constructor
	Ellipce() :c0({ 0,0 }) {};

	//check point
	bool CheckPoint(tPoint p);

	//getters
	double GetCx() { return c0.first; };
	double GetCy() { return c0.second; };
	double GetX0() { return x0; };
	double GetY0() { return y0; };
	double GetHeight() { return 2 * b; };
	double GetWidth() { return 2 * a; };
	vector<tPoint> GetTPoints();
};

//структура треугольника
struct Triangle {
private:
	tPoint p1, p2, p3;	//points of triangle
	double Ro = 0;						//circumscribed circle radius
	double Rv = 0;						//inscribed circle radius
	double triangleParametr = 0;		//parametr for rating triangles
	tPoint center;		//circumcenter

	//Find parametrs
	void FindParametrs();
public:
	//constructor
	Triangle(tPoint p1, tPoint p2, tPoint p3);

	//check point
	bool CheckPoint(tPoint p);

	//Getters
	tPoint GetP1() { return p1; };
	tPoint GetP2() { return p2; };
	tPoint GetP3() { return p3; };

	tPoint* GetpP1() { return &p1; };
	tPoint* GetpP2() { return &p2; };
	tPoint* GetpP3() { return &p3; };

	double GetParametr() { return triangleParametr; };

	//operator ==
	bool operator==(Triangle& t) {
		if (
			(t.GetP1() == this->GetP1() || t.GetP1() == this->GetP2() || t.GetP1() == this->GetP3()) &&
			(t.GetP2() == this->GetP1() || t.GetP2() == this->GetP2() || t.GetP2() == this->GetP3()) &&
			(t.GetP3() == this->GetP1() || t.GetP3() == this->GetP2() || t.GetP3() == this->GetP3())
			) return true;
		else
			return false;
	};
};
