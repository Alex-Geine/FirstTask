#include "pch.h"
#include "Drawer.h"

IMPLEMENT_DYNAMIC(Drawer, CStatic)

Drawer::Drawer()
{	
}

Drawer::~Drawer()
{	
}

BEGIN_MESSAGE_MAP(Drawer, CStatic)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void Drawer::GetContr(Controller* control) {
	
	this->control = control;
}

void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
	if (draw) {
		if (drawerID == 1)
			control->DrawMainGr(lpDrawItemStruct);		
	}	
}




//lb Down
void Drawer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//MessageBox(L"down");
	control->UpdateTranslateState(1, point.x, point.y);
	CStatic::OnLButtonDown(nFlags, point);
}

//lb up
void Drawer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//MessageBox(L"up");
	control->UpdateTranslateEndPoint(point.x, point.y);
	Invalidate(false);
	control->UpdateTranslateState(0, 0, 0);
	CStatic::OnLButtonUp(nFlags, point);
}

//move
//void Drawer::OnMouseMove(UINT nFlags, CPoint point)
//{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	
//	CStatic::OnMouseMove(nFlags, point);
//}
