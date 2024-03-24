#pragma once
#include "DataTypes.h"



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

	//searching for connections between points and triangles
	void SearchConnections();

	//search neigh
	void SearchForNeight();
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

