#pragma once

//////////////////////////////////////////////////////////////////////////
class CButtonView : public CDUINotifyPump
{
	DuiDeclare_Message_Map()

public:
	CButtonView();

	//notify
protected:
	void OnDuiWndInited(const DuiNotify &Notify);
};

//////////////////////////////////////////////////////////////////////////