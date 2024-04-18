#pragma once
#include "DataTypes.h"
#include <fstream>

//класс для нашей модели
class Model
{
private:
	//data
	vector<tPoint> points;			//vector with points not border
	vector<tPoint> borderPoints;	//vector with border points

	vector<Triangle> triangles;		//vector with triangles

	//equation system: A * C = B

	double** A = nullptr;			//mas with left matrix A
	double* B = nullptr;			//mas with right matrix B
	double* C = nullptr;			//mas with middle matrix C


	//functions
	void MatrixInFile(string);

	//find coef for matrix A
	double FindCoefA(int i, int j);

	//find coef for matrix B
	double FindCoefB(int i);

	//karchmarzh
	void kazf(double** A, double* b, double* x);

public:
	//constructor
	Model() {};

	//destructor
	~Model() {
		int N = points.size();
		if (A) 
			for (int i = 0; i < N; i++) 
				delete[] A[i];
		
		if (B)
			delete[] B;

		if (C)
			delete[] C;	

		A = nullptr;
		B = nullptr;
		C = nullptr;
	};

	//Set data to model
	void SetData(vector<tPoint> dataPoints, vector<Triangle> triangles);

	//start solving
	void StartSolving();

	//get data
	void GetData(vector<tPoint>& points, vector<Triangle>& triangles) {
		points.clear();
		triangles.clear();

		for (int i = 0; i < borderPoints.size(); i++)
			points.push_back(borderPoints[i]);

		for (int i = 0; i < this->points.size(); i++)
			points.push_back(this->points[i]);

		for (int i = 0; i < this->triangles.size(); i++)
			triangles.push_back(this->triangles[i]);
	};
};

