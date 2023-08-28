#ifndef __DUICALENDARCTRL_H__
#define __DUICALENDARCTRL_H__

#pragma once
#pragma pack(1)

class CDUICalendarWnd;
//////////////////////////////////////////////////////////////////////////
#define VER_CDUICalendarCtrl INTERFACE_VERSION(1,1)
static const GUID IID_CDUICalendarCtrl = { 0xA6D76CAA,0xF4CD,0x47F1,0xAD,0x6E,0xB1,0xFF,0x9A,0xE0,0x8B,0xB9 };
class DUITHINK_API CDUICalendarCtrl
	: public CDUIVerticalLayoutCtrl
	, public CDUINotifyPump
	, public IDUIControlCallBack
{
	DuiDeclare_CreateControl(CDUICalendarCtrl)
	MMDeclare_ClassName(CDUICalendarCtrl)
	DuiDeclare_Message_Map()

public:
	CDUICalendarCtrl(void);
	~CDUICalendarCtrl(void);

	//attribute
protected:
	CDUIAttributeGroup					m_AttributeGroupHeader;
	CDUIAttributeBool					m_AttributeHeaderVisible;
	CDUIAttributeLong					m_AttributeHeaderHeight;
	CDUIAttributeSize					m_AttributeLeftRightBtnSize;
	CDUIAttributeTextStyle				m_AttributeTextStyleYearNormal;
	CDUIAttributeTextStyle				m_AttributeTextStyleYearHot;
	CDUIAttributeTextStyle				m_AttributeTextStyleMonthNormal;
	CDUIAttributeTextStyle				m_AttributeTextStyleMonthHot;
	CDUIAttributeColorSwitch			m_AttributeColorYearNormal;
	CDUIAttributeColorSwitch			m_AttributeColorYearHot;
	CDUIAttributeColorSwitch			m_AttributeColorMonthNormal;
	CDUIAttributeColorSwitch			m_AttributeColorMonthHot;
	CDUIAttributeColorSwitch			m_AttributeColorLeftRightBtnNormal;
	CDUIAttributeColorSwitch			m_AttributeColorLeftRightBtnHot;
	CDUIAttriImageSection				m_AttributeImageLeftBtnNormal;
	CDUIAttriImageSection				m_AttributeImageLeftBtnHot;
	CDUIAttriImageSection				m_AttributeImageRightBtnNormal;
	CDUIAttriImageSection				m_AttributeImageRightBtnHot;

	CDUIAttributeGroup					m_AttributeGroupDays;
	CDUIAttributeCombox					m_AttributeWeekStyle;
	CDUIAttributeLong					m_AttributeWeekHeight;
	CDUIAttributeColorSwitch			m_AttributeColorHotDayBk;
	CDUIAttributeColorSwitch			m_AttributeColorSelDayBk;
	CDUIAttributeColorSwitch			m_AttributeColorTodayBk;
	CDUIAttriImageSection				m_AttributeImageHotDayBk;
	CDUIAttriImageSection				m_AttributeImageSelDayBk;
	CDUIAttriImageSection				m_AttributeImageTodayBk;
	CDUIAttributeTextStyle				m_AttributeTextStyleWeek;
	CDUIAttributeTextStyle				m_AttributeTextStyleOtherMonthDay;
	CDUIAttributeTextStyle				m_AttributeTextStyleCurMonthDay;
	CDUIAttributeTextStyle				m_AttributeTextStyleHotMonthDay;
	CDUIAttributeTextStyle				m_AttributeTextStyleToday;
	CDUIAttributeText					m_AttributeTextWeekDay[Dui_Count_WeekDay] = {};

	//variant
protected:
	CDUIHorizontalLayoutCtrl *			m_pHorizHeaderCtrl = NULL;
	CDUIButtonCtrl *					m_pBtnLeftCtrl = NULL;
	CDUIButtonCtrl *					m_pBtnYearCtrl = NULL;
	CDUIButtonCtrl *					m_pBtnMonthCtrl = NULL;
	CDUIButtonCtrl *					m_pBtnRightCtrl = NULL;

	CDUITabLayoutCtrl *					m_pTabSwitchCtrl = NULL;
	CDUIVerticalLayoutCtrl *			m_pVertPageCtrl1 = NULL;
	CDUIVerticalLayoutCtrl *			m_pVertPageCtrl2 = NULL;
	CDUIHorizontalLayoutCtrl *			m_pHorizWeekCtrl1 = NULL;
	CDUIHorizontalLayoutCtrl *			m_pHorizWeekCtrl2 = NULL;
	CDUIListViewCtrl *					m_pListDayCtrl1 = NULL;
	CDUIListViewCtrl *					m_pListDayCtrl2 = NULL;

	TCHAR								m_szWeekDayAttriName[Dui_Count_WeekDay][32] = { _T("Monday"),_T("Tuesday"), _T("Wednesday"), _T("Thursday"), _T("Friday"), _T("Saturday"), _T("Sunday"), };
	TCHAR								m_szWeekDayInitText[Dui_Count_WeekDay][32] = { _T("����һ"),_T("���ڶ�"), _T("������"), _T("������"), _T("������"), _T("������"), _T("������"), };

	tagDuiCalendarCmd					m_CalendarCmd;

	//override
protected:
	bool OnAttributeChange(CDUIAttributeObject *pAttributeObj) override;

	//override
protected:
	void OnRelease(CDUIControlBase *pControl) override;
	void OnSize(CDUIControlBase *pControl) override;

	//method
public:
	LPVOID QueryInterface(REFGUID Guid, DWORD dwQueryVer) override;
	CMMString GetDescribe() const override;

	//basic
	bool SetWndManager(CDUIWndManager *pWndManager) override;

	//calendar
	virtual bool IsHeaderVisible();
	virtual int GetHeaderHeight();
	virtual int GetWeekHeight();
	virtual CDUISize GetLeftRightBtnSize();
	virtual tagDuiCalendarCmd GetCalendarSelect();
	virtual void SetCalendarSelect(tagDuiCalendarCmd tTime);

	//weekday 1-7
	virtual CMMString GetMondayText();
	virtual CMMString GetTuesdayText();
	virtual CMMString GetWednesdayText();
	virtual CMMString GetThursdayText();
	virtual CMMString GetFridayText();
	virtual CMMString GetSaturdayText();
	virtual CMMString GetSundayText();
	virtual CMMString GetWeekText(int nWeekDay);

	//method
protected:
	//child
	bool InsertChild(CDUIControlBase *pChild, int nPos = -1) override;

	//proprety
protected:
	void InitProperty() override;
	void InitAttriValue() override;
	void InitComplete() override;

	//notify
protected:
	void OnDuiMouseWheel(const DuiNotify &Notify);
	void OnDuiItemSelected(const DuiNotify &Notify);
	void OnDuiBtnLeft(const DuiNotify &Notify);
	void OnDuiBtnRight(const DuiNotify &Notify);
	void OnDuiBtnYear(const DuiNotify &Notify);
	void OnDuiBtnMonth(const DuiNotify &Notify);

	//help
protected:
	void InitChildren();
	void InitCtrlInfo();
	void InitCalendarInfo();

	int ParseYear(DWORD dwDate);
	int ParseMonth(DWORD dwDate);
	int ParseDay(DWORD dwDate);
	DWORD ConstructDate(int nYear, int nMonth, int nDay);
	void ConstructHeader(int nYear, int nMonth);
	void ConstructDay(CDUIListItemCtrl *pDay, int nYear, int nMonth, int nDay);
	void ConstructCalendar(CDUIListViewCtrl *pListDays, int nYear, int nMonth);

	//help
protected:
	bool IsLeapYear(int nYear);
	int GetWeekDay(int nYear, int nMonth, int nDay);
	int GetYearDays(int nYear, int nMonth, int nDay);
	int GetMonthDays(int nYear, int nMonth);
	bool GetPreYearMonth(IN int nYear, IN int nMonth, OUT int &nYearOut, OUT int &nMonthOut);
	bool GetNextYearMonth(IN int nYear, IN int nMonth, OUT int &nYearOut, OUT int &nMonthOut);

	//help
protected:
	void SwitchYearMonth(bool bPre);
	void SwitchYearMonth(int nYear, int nMonth);
};

//////////////////////////////////////////////////////////////////////////
class DUITHINK_API CDUICalendar
{
public:
	CDUICalendar();
	virtual ~CDUICalendar();

protected:

	//method
public:
	virtual void LoadCalendar(LPCTSTR lpszCalendar);
	virtual void DestroyCalendar();
	virtual CDUICalendarCtrl * GetCalendarCtrl();
	virtual tagDuiCalendarCmd TrackPopupCalendar(HWND hWndParent, CDUIPoint pt = { -1, -1 });
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
