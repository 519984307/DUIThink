#pragma once

//////////////////////////////////////////////////////////////////////////
class CRichEditView : public CDUINotifyPump
{
	DuiDeclare_Message_Map()

public:
	CRichEditView();

	//notify
protected:
	void OnDuiWndInited(const DuiNotify &Notify);
};

//////////////////////////////////////////////////////////////////////////

