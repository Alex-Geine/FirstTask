#pragma once
#include <afxwin.h>
#include "Controller.h"

// Drawer
class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)
	friend class Controller;
public:
	Drawer();
	virtual ~Drawer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	
	int drawerID;
	Controller* control;
	bool draw = false;
	void GetContr(Controller* control);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
