#pragma once
#include "time.h"
#include "iostream"
#include "vector"
#include "cmath"
#include "math.h"
#define PI 3.14
using namespace std;

//struct points
struct tPoint {
private:
	double x = 0;
	double y = 0;
	bool isExtra = false;
	bool isBorder = false;

public:
	//default constructor
	tPoint() {};
	
	//constructor
	tPoint(double x, double y, bool isExtra, bool isBorder): x(x), y(y), isExtra(isExtra), isBorder(isBorder) {};
	
	//get x
	double X() { return x; };
	
	//get y
	double Y() { return y; };

	//set X
	void SetX(double x) { this->x = x; };
	
	//set Y
	void SetY(double y) { this->y = y; };

	//isExtra
	bool isExtraP() { return isExtra; };

	//isExtra
	bool isBorderP() { return isBorder; };

	//operator ==
	bool operator ==(tPoint& p) {
		if (this->x == p.x && this->y == p.y && this->isExtra == p.isExtra && this->isBorder == p.isBorder)
			return true;
		else
			return false;
	}
};

//структура эллипса
struct Ellipce {
private:
	pair<double, double> c0;	//point of center
	double a = 0;				//semiaxes
	double b = 0;				//semiaxes

	//for drawer
	double x0 = 0;
	double y0 = 0;
	
public:
	//constructor
	Ellipce(pair<double, double> c0, double a, double b) : c0(c0), a(a),b(b) {
		x0 = c0.first - a;
		y0 = c0.second - b;
	};

	//default constructor
	Ellipce() :c0({0,0}) {};

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

class TriangulationClass
{
private:

	int N = 0;								//points count
	double									//parametrs of area
		x0 = 0,
		y0 = 0,
		height = 0,
		width = 0,
		areaBorder = 0.1;					//to cut off unnecessary triangles
	vector<tPoint> points;		//mass poins
	vector<tPoint> pointsUser;		//mass poins
	vector<tPoint> tempPoints;//mass poins temp
	double disp = 0;						//dispersion
	double koefTriangle = 0.5;				//for rading triangles
	int elCount = 2;					//count of ellipces
	
	//vector for Triangles
	vector<Triangle> triangles;

	//vector for Triangles to send
	vector<Triangle> trianglesUser;

	//mass of Ellipses
	Ellipce* el;

	//generates Extra points
	void GenExtraPoints();

	//deletes extra points and tiangles
	void DeleteExtraPoints();

	//check triangle
	bool CheckTriangle(int i, int j, int k);

	//check triangle
	bool CheckTriangle(int i, int j, int k, tPoint);

	//search for duplicates
	bool SearchForDupl(tPoint);

	//check all Triangles
	void CheckAllTriangles();

	//check all Ellipces
	bool CheckEllipces(tPoint p);

	//cut off unnecessary triangle
	bool CutOff(Triangle tr);

	//if the triangle is on the border
	bool IsBorder(Triangle tr);

	//common function
	void CommonFunc(int i, int j);

	//common function
	void CommonFunc(int i, int j, tPoint);

	//simple triangulation
	void SimpleTriangulation();

	//simple triangulation
	void SimpleTriangulation(tPoint);

	//update triangles and points
	void UpdateTrianglesNPoints();	
public:

	//constructor
	TriangulationClass(int N, double x0, double y0, double height, double width, double dispersion, Ellipce* el) :
		N(N), x0(x0), y0(y0), height(height), width(width), disp(dispersion), el(el) {
		

		srand(time(0));
		GenExtraPoints();
		//send data to user
		UpdateTrianglesNPoints();
	};

	//destructor
	~TriangulationClass() {
	};

	//GetPoints func
	vector<tPoint>* GetPoins() { return &pointsUser; };

	//Getters

	int GetN() { return points.size(); };
	double GetX0() { return x0; };
	double GetY0() { return y0; };
	double GetHeight() { return height; };
	double GetWidth() { return width; };
	int GetElCount() { return elCount; };
	vector<Triangle>* GetTriangles(){ return &trianglesUser; };
	Ellipce* GetEllipces() { return el; };

	//clear old points
	void ClearOldPoints() {		
		for (int i = 0; i < points.size(); i++) {
			if (!points[i].isExtraP() || !points[i].isBorderP()) {
				points.erase(points.begin() + i);
				i = -1;
			}
		}
	};

	//triangulation
	void Triangulation();

	//clear functions
	void ClearTriangulation() { triangles.clear(); ClearOldPoints(); };
	void ClearPoints() { points.clear(); };
};

