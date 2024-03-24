
// triangulationDlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include "Controller.h"


// Диалоговое окно CtriangulationDlg
class CtriangulationDlg : public CDialogEx
{
// Создание
public:
	CtriangulationDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRIANGULATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer drawer;
	Controller con;
	MSG msg;
	UINT_PTR timer;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	unsigned int N;
	double disp;
	afx_msg void OnBnClickedOk2();
	double el1X;
	double el1y;
	double el1A;
	double el1B;
	double el2x;
	double el2y;
	double el2A;
	double el2B;
};
