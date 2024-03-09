#include "pch.h"
#include "Controller.h"

//функция, которая работает в потоке с триангуляцией
DWORD WINAPI Controller::TriangleFunk() {
	tc->ClearTriangulation();
	tc->Triangulation();

	TriangulationReady = true;

	return 0;
};

//apdate model
void Controller::UpdateModel(double R, double disp, double x0, double y0, double height, double width, double N,
	pair<double, double> c1, pair<double, double> c2, double A1, double A2, double B1, double B2) {
	this->R = R;
	Ellipce* el = new Ellipce[2];
	el[0] = Ellipce(c1, A1, B1);
	el[1] = Ellipce(c2, A2, B2);
	tc = new TriangulationClass(N, x0, y0, height, width, disp, el);
};

//очищает данные
void Controller::Clear() {
	if (tc != nullptr)
		delete tc;
	R = 0;
};

//запускает триангуляцию
void Controller::StartTriangulation() {
	TriangulationReady = false;
	TriangleHandle = CreateThread(NULL, NULL, StaticTriangleFunk, (void*)this, 0, NULL);	
};

//запускает отрисовку 2d графика
void Controller::DrawMainGr(LPDRAWITEMSTRUCT Item1) {
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;

	//ïàðàìåòðû ãðàôèêà â ìèðîâûõ êîîðèíàòàõ
	double top = 2 * R;
	double bottom = 0;
	double left = 0;
	double right = 2 * R;

	double
		steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right + right / 15 - left);
	double yScale = -ItemHeight / (top + top / 5 - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//ñãëàæèâàíèå
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//ïðåîáðàçîâàíèå ìèðà(ïåðåõîä ê êîîðäèíàòàì ñàìîãî ãðàôèêà
	Matrix matr;
	
	matr.Scale(xScale * scaleKoef, (yScale + 1) * scaleKoef); //ìàøòàáèðîâàíèå
	matr.Translate(right / 2 - chX * right / 10, -top / 2 + chY * top / 10); //ïåðåíîñ íà÷àëà êîîðäèíàò
	

	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::BlueViolet, 0.006);
	Pen pen3(Color::BlueViolet, 0.001);
	Pen pen4(Color::White, 0.002);
	Pen pen5(Color::Orange, 0.002);
	Pen pen2(Color::Green, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//Ðàçìåòêà
	for (int i = 0; i < 10; i++)
	{
		//ãîðèçîíòàëüíàÿ
		gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

		//âåðòèêàëüíàÿ
		gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
	}

	//Áîëüøèå îñè
	//ãîðèçîíòàëüíàÿ
	gr.DrawLine(&pen, PointF(-right / 2, -top / 2), PointF(right / 2, -top / 2));

	//âåðòèêàëüíàÿ
	gr.DrawLine(&pen, PointF(right / 2, top / 2), PointF(right / 2, -top / 2));

	gr.ResetTransform();


	//ïîäïèñè ê îñÿì
	for (int i = 0; i < 11; i++)
	{
		CString str;

		//ïî Y
		str.Format(L"%.2f", top / 2 - i * steplenY);
		PointF strPoint(right / 2, top / 2 - i * steplenY);
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//ïî X
		str.Format(L"%.2f", right / 2 - i * steplenX);
		strPoint.X = right / 2 - i * steplenX;
		strPoint.Y = -top / 2;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);		
	}

	gr.SetTransform(&matr);

	//отрисовка области
	gr.DrawRectangle(&pen2, RectF(PointF(tc->GetX0(), tc->GetY0()), SizeF(tc->GetWidth(), tc->GetHeight())));

	//отрисовка эллипсов
	Ellipce* el = tc->GetEllipces();
	for (int i = 0; i < tc->GetElCount(); i++)
		gr.DrawEllipse(&pen2, RectF(PointF(el[i].GetX0(), el[i].GetY0()), SizeF(el[i].GetWidth(), el[i].GetHeight())));

	//отрисовка точек для триангуляции
	int n = tc->GetN();
	vector<tPoint> points = *tc->GetPoins();
	double radius = R / 100;
	for (int i = 0; i < n; i++) {
		gr.DrawEllipse(&pen5, RectF(PointF(points[i].X() - radius / 2, points[i].Y() - radius / 2), SizeF(radius, radius)));
	}
	//отрисовка треугольников
	vector<Triangle> tr = *tc->GetTriangles();
	if (!tr.empty()) {
		for (int i = 0; i < tr.size(); i++) {
			gr.DrawLine(&pen3, PointF(tr[i].GetP1().X(), tr[i].GetP1().Y()), PointF(tr[i].GetP2().X(), tr[i].GetP2().Y()));
			gr.DrawLine(&pen3, PointF(tr[i].GetP3().X(), tr[i].GetP3().Y()), PointF(tr[i].GetP1().X(), tr[i].GetP1().Y()));
			gr.DrawLine(&pen3, PointF(tr[i].GetP2().X(), tr[i].GetP2().Y()), PointF(tr[i].GetP3().X(), tr[i].GetP3().Y()));
		}
	}

	
	
	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//apdate scale
void Controller::UdpateScale(short sign) {
	double dscale = 0.2;
	if (sign > 0) {
		scaleKoef += dscale;
	}
	else {
		if (scaleKoef - dscale <= 0) return;
		scaleKoef -= dscale;
	}
};

//apdate translate state
void Controller::UpdateTranslateState(bool state, double x, double y) {
	translateFlag = state;
	startX = x;
	startY = y;
};

//apdate translate endpoint
void Controller::UpdateTranslateEndPoint(double x, double y) {
	if (translateFlag) {
		double signX = 1;
		double signY = 1;
		if (startX - x < 0)
			signX = -1;
		if (startY - y < 0)
			signY = -1;
		chX += signX;
		chY += signY;

		startX = x;
		startY = y;
	}
};