#pragma once

#include "wx/wx.h"

#define TEST_1
#define TEST_2

#undef TEST_1
//#undef TEST_2



class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
#ifdef TEST_1
	wxButton* m_btn1 = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxListBox* m_list1 = nullptr;
#endif

#ifdef TEST_2
	int nFieldWidth = 10;
	int nFieldHeight = 10;
	wxButton** btn;
	int* nField = nullptr;
	bool bFirstClick = true;
#endif

	void OnButtonClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

