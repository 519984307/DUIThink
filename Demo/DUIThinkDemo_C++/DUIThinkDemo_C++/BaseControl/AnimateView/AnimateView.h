#pragma once

//////////////////////////////////////////////////////////////////////////
class CAnimateView : public CDUINotifyPump
{
	DuiDeclare_Message_Map()

public:
	CAnimateView();

	//notify
protected:
	void OnDuiWndInited(const DuiNotify &Notify);
};

//////////////////////////////////////////////////////////////////////////

