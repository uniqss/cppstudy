#include "noname.h"
#include <wx/wx.h>

class wxMyApp : public wxApp
{
	public:
		virtual bool OnInit();
};

IMPLEMENT_APP(wxMyApp);
bool wxMyApp::OnInit()
{
	auto frame = new MyFrame1(NULL, wxID_ANY, wxT("test"), wxDefaultPosition, wxSize(250, 150));
	frame->Show(true);
	return true;
}
