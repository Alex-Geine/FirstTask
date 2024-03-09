#include "pch.h"
#include "TriangulationClass.h"

//TRIANGLE STRUCT

//constructor
Triangle::Triangle(tPoint p1, tPoint p2, tPoint p3) : p1(p1), p2(p2), p3(p3) {
	FindParametrs();
};

//check point
bool Triangle::CheckPoint(tPoint p) {
	//find ABS(center - p)
	double temp = sqrt((center.X() - p.X()) * (center.X() - p.X()) + (center.Y() - p.Y()) * (center.Y() - p.Y()));

	if (temp <= Ro)
		return false;
	else
		return true;
};

//Find parametrs
void Triangle::FindParametrs() {
	//circumscribed circle radius
	double p1x2 = p1.X() * p1.X();
	double p2x2 = p2.X() * p2.X();
	double p3x2 = p3.X() * p3.X();

	double p1y2 = p1.Y() * p1.Y();
	double p2y2 = p2.Y() * p2.Y();
	double p3y2 = p3.Y() * p3.Y();

	center.SetX(
		((p2x2 - p1x2 + p2y2 - p1y2) * (p3.Y() - p1.Y()) - (p3x2 - p1x2 + p3y2 - p1y2) * (p2.Y() - p1.Y())) / 2 /
		((p2.X() - p1.X()) * (p3.Y() - p1.Y()) - (p3.X() - p1.X()) * (p2.Y() - p1.Y())) );
	center.SetY(
		((p3x2 - p1x2 + p3y2 - p1y2) * (p2.X() - p1.X()) - (p2x2 - p1x2 + p2y2 - p1y2) * (p3.X() - p1.X())) / 2 /
		((p2.X() - p1.X()) * (p3.Y() - p1.Y()) - (p3.X() - p1.X()) * (p2.Y() - p1.Y())));

	Ro = sqrt((center.X() - p1.X()) * (center.X() - p1.X()) + (center.Y() - p1.Y()) * (center.Y() - p1.Y()));

	//inscribed circle radius
	double
		a = sqrt((p1.X() - p2.X()) * (p1.X() - p2.X()) + (p1.Y() - p2.Y()) * (p1.Y() - p2.Y())),
		b = sqrt((p2.X() - p3.X()) * (p2.X() - p3.X()) + (p2.Y() - p3.Y()) * (p2.Y() - p3.Y())),
		c = sqrt((p3.X() - p1.X()) * (p3.X() - p1.X()) + (p3.Y() - p1.Y()) * (p3.Y() - p1.Y())),
		p = (a + b + c) / 2;

	Rv = sqrt((p - a) * (p - b) * (p - c) / p);

	//rating triangle
	triangleParametr = 2 * Rv / Ro;
};

//ELLIPCE STRUCT
tPoint* Ellipce::GetTPoints(int & size) {
	size = 5;		//пока что так

	tPoint* points = new tPoint[size];

	points[0] = tPoint(c0.first, c0.second, 1);			//central
	points[1] = tPoint(c0.first - a / 2, c0.second, 1);	//left
	points[2] = tPoint(c0.first + a / 2, c0.second, 1);	//right
	points[3] = tPoint(c0.first, c0.second - b / 2, 1);	//up
	points[4] = tPoint(c0.first, c0.second + b / 2, 1);	//down

	return points;
};

//TRIANGULATION CLASS

//if the triangle is on the border
bool TriangulationClass::IsBorder(Triangle tr) {
	double dHeight = height * areaBorder;
	double dWidth = width * areaBorder;

	double x1 = x0 + dWidth;
	double x2 = x0 + width - dWidth;
	double y1 = y0 + dHeight;
	double y2 = y0 + height - dHeight;
	if (
		(tr.GetP1().X() >= x1 && tr.GetP1().X() <= x2 && tr.GetP1().Y() >= y1 && tr.GetP1().Y() <= y2) ||
		(tr.GetP2().X() >= x1 && tr.GetP2().X() <= x2 && tr.GetP2().Y() >= y1 && tr.GetP2().Y() <= y2) ||
		(tr.GetP3().X() >= x1 && tr.GetP3().X() <= x2 && tr.GetP3().Y() >= y1 && tr.GetP3().Y() <= y2))
	
		return false;
	else
		return true;
};

//cut off unnecessary triangle
bool TriangulationClass::CutOff(Triangle tr) {
	if (tr.GetParametr() < koefTriangle)
		return true;
	else
		return false;
};

//generates Extra points
void TriangulationClass::GenExtraPoints() {
	

	//add extra points to the corners
	points.push_back(tPoint(x0 + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 + disp * (1 - 2 * (double)rand() / RAND_MAX), 1));
	points.push_back(tPoint(x0 + width + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 + disp * (1 - 2 * (double)rand() / RAND_MAX), 1));
	points.push_back(tPoint(x0 + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 + height + disp * (1 - 2 * (double)rand() / RAND_MAX), 1));
	points.push_back(tPoint(x0 + width + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 + height + disp * (1 - 2 * (double)rand() / RAND_MAX), 1));

	//add extra points into the ellipces
	for (int i = 0; i < elCount; i++) {
		int size = 0;
		tPoint* p = el[i].GetTPoints(size);
		for (int j = 0; j < size; j++) 
			points.push_back(tPoint(p[j].X() + disp * (1 - 2 * (double)rand() / RAND_MAX), p[j].Y() + disp * (1 - 2 * (double)rand() / RAND_MAX), 1));
		
		delete[] p;
	}

};

//check triangle
bool TriangulationClass::CheckTriangle(int i, int j, int k) {
	Triangle tr(points[i], points[j], points[k]);
	
	for (int f = 0; f < points.size(); f++) {
		if (f != i && f != j && f != k) {
			if (!tr.CheckPoint(points[f])) {
				return false;
			}
		}
	}

	//check for bad triangles
	//if ( CutOff(tr) && IsBorder(tr))
	//	return false;

	return true;
};

//simple triangulation
void TriangulationClass::SimpleTriangulation(vector<tPoint> points) {
	int size = points.size();

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (j != i)
				CommonFunc(i, j, points);
};

//check all Triangles
void TriangulationClass::CheckAllTriangles() {

};

//triangulation
void TriangulationClass::Triangulation() {
	//simple triang with extra points
	SimpleTriangulation(points);

	////this cicle adds new points and do new triangulation
	//double dx = width / (N - 1);
	//double dy = height / (N - 1);

	//for (int i = 0; i < N; i++) {
	//	for (int j = 0; j < N; j++) {
	//		//add new point
	//		tPoint newP = tPoint(x0 + dx * i + disp * (1 - 2 * (double)rand() / RAND_MAX),
	//			y0 + dy * j + disp * (1 - 2 * (double)rand() / RAND_MAX), 0);
	//		
	//		points.push_back(newP);
	//		tempPoints.push_back(newP);

	//		//check triangles


	//	}
	//}	 
};

//common function
void TriangulationClass::CommonFunc(int i, int j, vector<tPoint> points) {
	for (int k = 0; k < points.size(); k++) 
		if (k != i && k != j) 
			if (CheckTriangle(i, j, k)) 
				triangles.push_back(Triangle(points[i], points[j], points[k]));
};
