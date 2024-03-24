#include "pch.h"
#include "DataTypes.h"

//TPOINT STRUCT

//add triangle in family
void tPoint::AddTriangle(Triangle* tr) {
	if(tr)
		familyTriangles.push_back(tr);
};

//check triangle for including this point
bool tPoint::CheckTriangleForIncluding(Triangle* tr) {
	if (tr->GetP1() == *(this) || tr->GetP2() == *(this) || tr->GetP3() == *(this))
		return 1;
	else
		return 0;
};

//find Cpoyes neigh
bool tPoint::FindCopyNeigh(tPoint p) {
	for (int i = 0; i < neighbour.size(); i++) 
		if (*neighbour[i] == p)
			return false;
	return true;
};

//find neigh
void tPoint::FindNeigh() {
	for (int i = 0; i < familyTriangles.size(); i++) {
		if (!(familyTriangles[i]->GetP1() == *this) && FindCopyNeigh(familyTriangles[i]->GetP1()))
			neighbour.push_back(familyTriangles[i]->GetpP1());
		if (!(familyTriangles[i]->GetP2() == *this) && FindCopyNeigh(familyTriangles[i]->GetP2()))
			neighbour.push_back(familyTriangles[i]->GetpP2());
		if (!(familyTriangles[i]->GetP3() == *this) && FindCopyNeigh(familyTriangles[i]->GetP3()))
			neighbour.push_back(familyTriangles[i]->GetpP3());
	}
};


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
		((p2.X() - p1.X()) * (p3.Y() - p1.Y()) - (p3.X() - p1.X()) * (p2.Y() - p1.Y())));
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
vector<tPoint> Ellipce::GetTPoints() {
	vector<tPoint> points;

	points.push_back(tPoint(c0.first, c0.second, 1, 0));			//central

	//half semiaxes
	points.push_back(tPoint(c0.first - a / 2, c0.second, 1, 0));	//left
	points.push_back(tPoint(c0.first + a / 2, c0.second, 1, 0));	//right
	points.push_back(tPoint(c0.first, c0.second - b / 2, 1, 0));	//up
	points.push_back(tPoint(c0.first, c0.second + b / 2, 1, 0));	//down

	//points on border have a non zero potential
	int size = 20;
	double A = 0;
	double da = (2 * PI) / (double)size;
	double t = 0;		//parametr
	for (int i = 0; i < size; i++) {
		A = i * da;			//angle

		t = atan2(a * sin(A), b * cos(A));
		tPoint p(a * cos(t + PI) + c0.first, b * sin(t + PI) + c0.second, 0, 1);

		//set a potencial
		p.SetP(borderValue);

		points.push_back(p);
	}

	return points;
};

//check point
bool Ellipce::CheckPoint(tPoint p) {
	//x^2 / a^2 + y^2 / b^2 <= 1 
	//double res = p.X() * p.X() / a / a + p.Y() * p.Y() / b / b;

	if ((p.X() - c0.first) * (p.X() - c0.first) / a / a + (p.Y() - c0.second) * (p.Y() - c0.second) / b / b <= 1)
		return false;
	else
		return true;
};