﻿
// triangulationDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "triangulation.h"
#include "triangulationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CtriangulationDlg



CtriangulationDlg::CtriangulationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRIANGULATION_DIALOG, pParent)
	, N(20)
	, disp(0.001)
	, el1X(-0.5)
	, el1y(-0.5)
	, el1A(0.1)
	, el1B(0.2)
	, el2x(0.5)
	, el2y(0.5)
	, el2A(0.1)
	, el2B(0.2)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtriangulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, drawer);
	DDX_Text(pDX, IDC_EDIT1, N);
	DDX_Text(pDX, IDC_EDIT2, disp);
	DDX_Text(pDX, IDC_EDIT4, el1X);
	DDX_Text(pDX, IDC_EDIT3, el1y);
	DDX_Text(pDX, IDC_EDIT6, el1A);
	DDX_Text(pDX, IDC_EDIT5, el1B);
	DDX_Text(pDX, IDC_EDIT8, el2x);
	DDX_Text(pDX, IDC_EDIT7, el2y);
	DDX_Text(pDX, IDC_EDIT10, el2A);
	DDX_Text(pDX, IDC_EDIT9, el2B);
}

BEGIN_MESSAGE_MAP(CtriangulationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CtriangulationDlg::OnBnClickedOk)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDOK2, &CtriangulationDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &CtriangulationDlg::OnBnClickedOk3)
END_MESSAGE_MAP()


// Обработчики сообщений CtriangulationDlg

BOOL CtriangulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	drawer.GetContr(&con);
	drawer.drawerID = 1;

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CtriangulationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CtriangulationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CtriangulationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Timer
void CtriangulationDlg::OnTimer(UINT_PTR nIDEvent)
{
	drawer.draw = 1;
	con.GetData();
	drawer.Invalidate(false);
	
	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (con.IsTriangReady() || con.IsSolvingReady())
		KillTimer(timer);

	drawer.draw = 1;
	con.GetData();
	drawer.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//Сгенерировать точки
void CtriangulationDlg::OnBnClickedOk()
{
	UpdateData();
	con.UpdateModel(2, disp, -1, -1, 2, 2, N, { el1X, el1y }, {el2x, el2y}, el1A, el2A, el1B, el2B);
	con.GetData();
	drawer.draw = 1;
	drawer.Invalidate(false);
	

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//прокрутка колеса
BOOL CtriangulationDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	con.UdpateScale(zDelta);
	drawer.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

//Триангулировать
void CtriangulationDlg::OnBnClickedOk2()
{	
	con.StartTriangulation();
	timer = SetTimer(1, 10, 0);	
	drawer.draw = 1;
	con.GetData();
	drawer.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//Посчитать
void CtriangulationDlg::OnBnClickedOk3()
{
	con.StartSolve();

	timer = SetTimer(1, 10, 0);
}

//Timer 2
void OnTimer2(UINT_PTR nIDEvent) {
	
};

