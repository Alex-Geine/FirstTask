
// triangulation.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CtriangulationApp:
// Сведения о реализации этого класса: triangulation.cpp
//

class CtriangulationApp : public CWinApp
{
public:
	CtriangulationApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CtriangulationApp theApp;
