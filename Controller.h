#pragma once

#define _USE_MATH_DEFINES
#include <gdiplus.h>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace Gdiplus;

#include "triangulationClass.h"
#include "Model.h"

class Controller {	
private:	
	ULONG_PTR token;
	TriangulationClass* tc = nullptr;
	Model* mod = nullptr;

	HANDLE TriangleHandle = NULL;
	HANDLE ModelHandle = NULL;

	//функция, которая работает в потоке с триангуляцией
	DWORD WINAPI TriangleFunk();

	static DWORD WINAPI StaticTriangleFunk(PVOID param) {
		Controller* This = (Controller*)param;
		return This->TriangleFunk();
	};


	//функция, которая работает в потоке с моделью
	DWORD WINAPI ModelFunk();

	static DWORD WINAPI StaticModelFunk(PVOID param) {
		Controller* This = (Controller*)param;
		return This->ModelFunk();
	};


	
	//For Scaling

	//parametr
	double scaleKoef = 1;

	//translate flag
	bool translateFlag = 0;

	//start point
	double startX = 0, startY = 0;
	
	//coord change
	double chX = 0, chY = 0;
	
	//for drawing
	double R = 0;

	//Some Flags
	bool TriangulationReady = false;

	//Model data
	
	double Potencial = 10;		//value of potincial on ellipces

	Ellipce* el = nullptr;
	vector<tPoint> points;
	vector<Triangle> tr;	

public:	
	//apdate model
	void UpdateModel(double R, double disp, double x0, double y0, double height, double width, double N,
		pair<double, double> c1, pair<double, double> c2, double A1, double A2, double B1, double B2);
		
	//очищает данные
	void Clear();
	
	//запускает отрисовку графика
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);	
	
	//запусткает вычисления
	void StartSolve();

	//запускает триангуляцию
	void StartTriangulation(); 

	Controller() {
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);	
	}

	//деструктор
	~Controller() {
		GdiplusShutdown(token);
	}	
	
	//забирает данные из модели
	void GetData();	

	//создает новую модель
	void InitModel();

	//apdate scale
	void UdpateScale(short);

	//apdate translate state
	void UpdateTranslateState(bool, double, double);

	//apdate translate endpoint
	void UpdateTranslateEndPoint(double, double);

	//getters
	bool IsTriangReady() { return TriangulationReady; };
};