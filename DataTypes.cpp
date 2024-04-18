#include "pch.h"
#include "DataTypes.h"

//TPOINT STRUCT

//check if tPoint is neight
bool tPoint::IsNeigh(tPoint p) {
	for (int i = 0; i < neighbour.size(); i++)
		if (p == *neighbour[i])
			return true;

	return false;
};

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

//get Two triangles
vector<Triangle*> tPoint::GetTwoTriangles(tPoint p) {
	vector<Triangle*> res;
	for (int i = 0; i < this->familyTriangles.size(); i++)
		for (int j = 0; j < p.familyTriangles.size(); j++)
			if (this->familyTriangles[i] == p.familyTriangles[j])
				res.push_back(this->familyTriangles[i]);		
	return res;
};


//TRIANGLE STRUCT

//constructor
Triangle::Triangle(tPoint p1, tPoint p2, tPoint p3) : p1(p1), p2(p2), p3(p3) {
	FindParametrs();
};

//‘ункци€, возвращающа€ дифференциал
void Triangle::GetDiff(tPoint p, double& A, double& B) {
	//we find the equation of the plane through a point and two vectors
	// | x - x0	a1 - x0	b1 - x0	|
	// | y - y0	a2 - x0	b2 - x0	| = 0
	// | z - z0 a3 - x0	b3 - x0	|

	//another method
	tPoint pi, pl, pk;

	//edit value of point p
	if (p == p1) {
		//p1.SetZ(1);
		pi = p1;
		pl = p2;
		pk = p3;
	}
	else if (p == p2) { 
		//p2.SetZ(1);
		pi = p2;
		pl = p1;
		pk = p3;
	}
	else {
		pi = p3;
		pl = p1;
		pk = p2;
		//p3.SetZ(1);
	}

	//determinant of sistem 
	double det = pi.X() * (pk.Y() - pl.Y()) + pk.X() * (pl.Y() - pi.Y()) + pl.X() * (pi.Y() - pk.Y());

	A = (pk.Y() - pl.Y()) / det;
	B = (pl.X() - pk.X()) / det;

	////temp vectors
	//double
	//	a1 = p1.X() - p3.X(),
	//	a2 = p1.Y() - p3.Y(),
	//	a3 = p1.Z() - p3.Z(),
	//	b1 = p2.X() - p3.X(),
	//	b2 = p2.X() - p3.X(),
	//	b3 = p2.X() - p3.X();

	//A = a2 * b3 - a3 * b2;
	//B = -1. * (a1 * b3 - a3 * b1);

	//

	////edit value of point p
	//p1.SetZ(0);
	//p2.SetZ(0);
	//p3.SetZ(0);
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

	//area of triangle
	//formula gerona: S = sqrt(p * (p Ч AB) * (p Ч AC) * (p Ч BC))
	area = sqrt(p * (p - a) * (p - b) * (p - c));
};

//check triangle for isolines
bool Triangle::CheckIsolines(double T, pair<tPoint, tPoint>& buf) {
	bool res = false;

	double maxX, maxY, minX, minY, maxZ, minZ;

	double x[3] = { p1.X(), p2.X(), p3.X() };
	double y[3] = { p1.Y(), p2.Y(), p3.Y() };
	double z[3] = { p1.P(), p2.P(), p3.P() };

	//tPoint p(0, 0, 0, 0);

	//проверка 3х граней треугольника
	for (int i = 0; i < 3; i++) {
		int idnext = i + 1;
		if (i == 2)
			idnext = 0;

		if (((z[i] <= T) && (z[idnext] >= T) || ((z[i] >= T) && (z[idnext] <= T)))) {

			//находим максимальное и минимальное значение по z
			if (z[i] > z[idnext]) {
				maxX = x[i];
				maxY = y[i];
				maxZ = z[i];
				minX = x[idnext];
				minY = y[idnext];
				minZ = z[idnext];
			}
			else {
				minX = x[i];
				minY = y[i];
				minZ = z[i];
				maxX = x[idnext];
				maxY = y[idnext];
				maxZ = z[idnext];
			}

			//находим значени€ пересечени€ по x и по y
			//p.SetX(maxX - (maxX - minX) * (maxZ - T) / (maxZ - minZ));
			//p.SetY(maxY - (maxY - minY) * (maxZ - T) / (maxZ - minZ));

			if (!res)
				buf.first = tPoint(maxX - (maxX - minX) * (maxZ - T) / (maxZ - minZ), maxY - (maxY - minY) * (maxZ - T) / (maxZ - minZ),0,0);
			else
				buf.second = tPoint(maxX - (maxX - minX) * (maxZ - T) / (maxZ - minZ), maxY - (maxY - minY) * (maxZ - T) / (maxZ - minZ), 0, 0);

			res = true;
		}
	}

	return res;
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