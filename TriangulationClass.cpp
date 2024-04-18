#include "pch.h"
#include "TriangulationClass.h"

//TRIANGULATION CLASS

//searching for connections between points and triangles
void TriangulationClass::SearchConnections() {
	//check all points
	for (int i = 0; i < points.size(); i++) {
		//check all triangles
		for (int j = 0; j < triangles.size(); j++) {
			if (points[i].CheckTriangleForIncluding(&triangles[j]))
				points[i].AddTriangle(&triangles[j]);
		}
	}
};

//search neigh
void TriangulationClass::SearchForNeight() {
	//check all points
	for (int i = 0; i < points.size(); i++) {
		points[i].FindNeigh();
	}
};

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
	double offset = (height + width) / 2 / 10;

	//add extra points to the corners
	points.push_back(tPoint(x0 - offset + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 - offset + disp * (1 - 2 * (double)rand() / RAND_MAX), 1, 0));
	points.push_back(tPoint(x0 + offset + width + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 - offset + disp * (1 - 2 * (double)rand() / RAND_MAX), 1, 0));
	points.push_back(tPoint(x0 - offset + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 + offset + height + disp * (1 - 2 * (double)rand() / RAND_MAX), 1, 0));
	points.push_back(tPoint(x0 + offset + width + disp * (1 - 2 * (double)rand() / RAND_MAX), y0 + offset + height + disp * (1 - 2 * (double)rand() / RAND_MAX), 1, 0));

	//add extra points into the ellipces
	for (int i = 0; i < elCount; i++) {
		vector<tPoint> p = el[i].GetTPoints();
		for (int j = 0; j < p.size(); j++) {
			p[j].SetX(p[j].X() + disp * (1 - 2 * (double)rand() / RAND_MAX));
			p[j].SetY(p[j].Y() + disp * (1 - 2 * (double)rand() / RAND_MAX));
			points.push_back(p[j]);
		}
	}
};

//deletes extra points and tiangles
void TriangulationClass::DeleteExtraPoints() {
	//delete triangles with extra points and bad triangles on the board
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].GetP1().isExtraP() || triangles[i].GetP2().isExtraP() || triangles[i].GetP3().isExtraP() || (CutOff(triangles[i]) && IsBorder(triangles[i]))) {
			triangles.erase(triangles.begin() + i);
			i = -1;
		}
	}

	//delete points
	for (int i = 0; i < points.size(); i++) 
		if (points[i].isExtraP()) {
			points.erase(points.begin() + i);
			i = -1;
		}
};

//check triangle
bool TriangulationClass::CheckTriangle(int i, int j, int k) {
	Triangle tr(points[i], points[j], points[k]);
	
	//check if this triangle already exist
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i] == tr)
			return false;
	}

	for (int f = 0; f < points.size(); f++) {
		if (f != i && f != j && f != k) {
			if (!tr.CheckPoint(points[f])) {
				return false;
			}
		}
	}

	return true;
};

//check all Ellipces
bool TriangulationClass::CheckEllipces(tPoint p) {
	for (int i = 0; i < elCount; i++)
		if (!el[i].CheckPoint(p))
			return false;
	return true;
};

//check triangle
bool TriangulationClass::CheckTriangle(int i, int j, int k, tPoint p) {
	Triangle tr(tempPoints[i], tempPoints[j], tempPoints[k]);

	//check if there is a new node in the triangle
	if (!(
		(p == tempPoints[i]) ||
		(p == tempPoints[j]) ||
		(p == tempPoints[k])
		))
		return false;

	//check if this triangle already exist
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i] == tr)
			return false;
	}

	for (int f = 0; f < tempPoints.size(); f++) {
		if (f != i && f != j && f != k) {
			if (!tr.CheckPoint(tempPoints[f])) {
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
void TriangulationClass::SimpleTriangulation() {
	int size = points.size();

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (j != i)
				CommonFunc(i, j);
};

//simple triangulation
void TriangulationClass::SimpleTriangulation(tPoint p) {
	int size = tempPoints.size();

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (j != i)
				CommonFunc(i, j, p);
};

//search for duplicates
bool TriangulationClass::SearchForDupl(tPoint p) {
	for (int i = 0; i < tempPoints.size(); i++)
		if (tempPoints[i] == p)
			return false;

	return true;
};

//check all Triangles
void TriangulationClass::CheckAllTriangles() {
	tPoint pointToCheck = tempPoints[0];
	
	//vector<Triangle> temp;
	for (int i = 0; i < triangles.size(); i++) {
		//if a point lies inside a triangle, save these points and delete the triangle
		if (!triangles[i].CheckPoint(pointToCheck)) {
			if (SearchForDupl(triangles[i].GetP1()))
				tempPoints.push_back(triangles[i].GetP1());
			if (SearchForDupl(triangles[i].GetP2()))
				tempPoints.push_back(triangles[i].GetP2());
			if (SearchForDupl(triangles[i].GetP3()))
				tempPoints.push_back(triangles[i].GetP3());

			//delete triangle
			triangles.erase(triangles.begin() + i);
			i = -1;
		}
		//else
			//temp.push_back(triangles[i]);
	}

	//delete triangles
	//triangles.clear();
	//for (int i = 0; i < temp.size(); i++)
	//	triangles.push_back(temp[i]);
};

//update triangles
void TriangulationClass::UpdateTrianglesNPoints() {
	trianglesUser.clear();
	pointsUser.clear();
	for (int i = 0; i < triangles.size(); i++)
		trianglesUser.push_back(triangles[i]);

	for (int i = 0; i < points.size(); i++)
		pointsUser.push_back(points[i]);
};

//triangulation
void TriangulationClass::Triangulation() {
	//if there are no extra points
	if (points.empty())
		GenExtraPoints(); 

	
	//simple triang with extra points
	SimpleTriangulation();
	
	//send data to user
	UpdateTrianglesNPoints();

	//this cicle adds new points and do new triangulation
	double dx = width / (N - 1);
	double dy = height / (N - 1);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			//sets border values
			bool isBorder = 0;
			if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
				isBorder = 1;

			//add new point
			tPoint newP = tPoint(x0 + dx * i + disp * (1 - 2 * (double)rand() / RAND_MAX),
				y0 + dy * j + disp * (1 - 2 * (double)rand() / RAND_MAX), 0, isBorder);

			
			
			//check if this point in the ellipce
			if (CheckEllipces(newP)) {
				points.push_back(newP);
				tempPoints.push_back(newP);
				

				//check all triangles 
				CheckAllTriangles();

				//start Simple Triangulation
				SimpleTriangulation(newP);

				tempPoints.clear();
			}
			//send data to user
			UpdateTrianglesNPoints();
		}
	}

	//delete extra points and triangles
	DeleteExtraPoints();

	//check for connections
	SearchConnections();

	//check for neigh
	SearchForNeight();

	//send data to user
	UpdateTrianglesNPoints();
};

//common function
void TriangulationClass::CommonFunc(int i, int j) {
	for (int k = 0; k < points.size(); k++) 
		if (k != i && k != j) 
			if (CheckTriangle(i, j, k)) 
				triangles.push_back(Triangle(points[i], points[j], points[k]));
};

//common function
void TriangulationClass::CommonFunc(int i, int j, tPoint p) {
	for (int k = 0; k < tempPoints.size(); k++)
		if (k != i && k != j)
			if (CheckTriangle(i, j, k, p))
				triangles.push_back(Triangle(tempPoints[i], tempPoints[j], tempPoints[k]));
};
