#include "pch.h"
#include "Model.h"

//functions
void Model::MatrixInFile(string s) {
	std::ofstream out("Matrix.txt", std::ios::app);
	
	if (out.is_open())
	{
		out << s << std::endl;

		int N = points.size();

		for (int i = 0; i < N; i++) {
			//matrix A
			/*for (int j = 0; j < N; j++) {
				out << A[i][j] << "\t";
			}*/

			//matrix C
			out  << C[i] << endl;
			
			//matrix B
			//out << "\t\t" << B[i];
		}

	}
	out.close();
};

//Set data to model
void Model::SetData(vector<tPoint> dataPoints, vector<Triangle> triangles) {  
	//points
	for (int i = 0; i < dataPoints.size(); i++) {
		if (dataPoints[i].isBorderP())
			borderPoints.push_back(dataPoints[i]);
		else
			points.push_back(dataPoints[i]);
	}

	//triangles
	for (int i = 0; i < triangles.size(); i++)
		this->triangles.push_back(triangles[i]);

	//size of matrix
	int N = points.size();
	
	//init matrix
	
	A = new double* [N];
	C = new double[N];
	B = new double[N];


	for (int i = 0; i < N; i++) {
		A[i] = new double[N];
		C[i] = 0;
		B[i] = 0;

		for (int j = 0; j < N; j++)
			A[i][j] = 0;
	}

	//MatrixInFile("First.");
};



//find coef for matrix A
double Model::FindCoefA(int i, int j) {	
	double res = 0;
	//when i = j
	if (i == j) {
		//get all triangles
		vector<Triangle*> t = points[i].GetFamilyTriangles();
		for (int k = 0; k < t.size(); k++) {
			double A = 0, B = 0;	//differ
			t[k]->GetDiff(points[i], A, B);
			res += (A * A + B * B) * t[k]->GetS();
		}
	}
	//when i is neighbour j
	else if (points[i].IsNeigh(points[j])) {
		vector<Triangle*> t = points[i].GetTwoTriangles(points[j]);
		for (int k = 0; k < t.size(); k++) {
			double A1 = 0, A2 = 0, B1 = 0, B2 = 0;		//differ
			t[k]->GetDiff(points[i], A1, B1);
			t[k]->GetDiff(points[j], A2, B2);
			res += (A1 * A2 + B1 * B2) * t[k]->GetS();
		}
	}
	
	return res;
};

//find coef for matrix B
double Model::FindCoefB(int i) {
	double res = 0;
	
	for (int j = 0; j < borderPoints.size(); j++) {
		//when i is neighbour j
		if (points[i].IsNeigh(borderPoints[j])) {
			vector<Triangle*> t = points[i].GetTwoTriangles(borderPoints[j]);
			double tempRes = 0;
			
			for (int k = 0; k < t.size(); k++) {
				double A1 = 0, A2 = 0, B1 = 0, B2 = 0;		//differ
				t[k]->GetDiff(points[i], A1, B1);
				t[k]->GetDiff(points[j], A2, B2);
				tempRes += (A1 * A2 + B1 * B2) * t[k]->GetS();
			}
			res += tempRes * borderPoints[j].P();
		}
	}
	
	return res;
};



//start solving
void Model::StartSolving() {
	int N = points.size();
	//find coef for Matrix A
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[i][j] = FindCoefA(i, j);
		}
	}

	//find coef for Matrix B
	for (int i = 0; i < N; i++)
		B[i] = FindCoefB(i);

	kazf(A, B, C);

	//update points values
	for (int i = 0; i < N; i++)
		points[i].SetP(C[i]);
	

	//MatrixInFile("Second.");
};

//метод Качмаржа
void Model::kazf(double** A, double* b, double* x)
{
	int N = points.size();
	//changing massive
	double* a = new double[N * N];

	int count = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			a[count] = A[i][j];
			count++;
		}
	}

	int nn = N;
	int ny = N;
	// nn - количество неизвестных;  ny - количество уравнений
	float eps = 1.e-6f;
	int i, j, k;
	float s1, s2, fa1, t;
	float* x1;

	x1 = new float[nn];

	x[0] = 0.5f;
	for (i = 1; i < nn; i++)  x[i] = 0.f;

	s1 = s2 = 1.f;
	while (s1 > eps * s2)
	{
		for (i = 0; i < nn; i++) x1[i] = x[i];

		for (i = 0; i < ny; i++)
		{
			s1 = 0.0;
			s2 = 0.0;
			for (j = 0; j < nn; j++)
			{
				fa1 = a[i * nn + j];
				s1 += fa1 * x[j];
				s2 += fa1 * fa1;
			}
			t = (b[i] - s1) / s2;
			for (k = 0; k < nn; k++)    x[k] += a[i * nn + k] * t;
		}

		s1 = 0.0;
		s2 = 0.0;
		for (i = 0; i < nn; i++)
		{
			s1 += (x[i] - x1[i]) * (x[i] - x1[i]);
			s2 += x[i] * x[i];
		}
		s1 = (float)sqrt(s1);
		s2 = (float)sqrt(s2);
	}
	delete[] x1;
	delete[] a;
}
