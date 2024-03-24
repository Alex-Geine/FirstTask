#pragma once
#include "DataTypes.h"

//класс для нашей модели
class Model
{
private:
	//data
	vector<tPoint> points;			//vector with points not border
	vector<tPoint> borderPoints;	//vector with border points

	vector<Triangle> triangles;		//vector with triangles
public:
	//constructor
	Model() {};

	//destructor
	~Model() {};

	//Set data to model
	void SetData(vector<tPoint> dataPoints, vector<Triangle> triangles);

};

