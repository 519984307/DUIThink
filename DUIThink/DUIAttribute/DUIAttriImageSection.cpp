#include "StdAfx.h"
#include "DUIAttriImageSection.h"

//////////////////////////////////////////////////////////////////////////
DuiImplement_CreateAttribute(CDUIAttriImageSection)
MMImplement_ClassName(CDUIAttriImageSection)

CDUIAttriImageSection::CDUIAttriImageSection()
{
#ifdef DUI_DESIGN
	m_DuiAttributeType = DuiAttribute_ImageSection;
#endif

	return;
}

CDUIAttriImageSection::~CDUIAttriImageSection(void)
{
	return;
}

LPVOID CDUIAttriImageSection::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(CDUIAttriImageSection, Guid, dwQueryVer);

	return __super::QueryInterface(Guid, dwQueryVer);
}

void CDUIAttriImageSection::Draw(HDC hDC, const CDUIRect &rcItem, const CDUIRect &rcPaint)
{
	CDUIImageBase *pImageBaseCur = GetCurImageBase();
	if (NULL == pImageBaseCur || NULL == m_pOwner) return;

	//info
	tagDuiImageSection ImageSection = GetImageSection();
	
	Draw(pImageBaseCur, ImageSection, hDC, rcItem, rcPaint);

	return;
}

void CDUIAttriImageSection::Draw(CDUIImageBase *pImageBase, const tagDuiImageSection &ImageSection, HDC hDC, const CDUIRect &rcItem, const CDUIRect &rcPaint)
{
	if (NULL == pImageBase) return;

	CDUIWndManager *pWndManager = GetOwnerWndManager();
	if (NULL == pWndManager) return;

	bool bScale = pImageBase->IsScale();
	bool bAlpha = pImageBase->IsAlpha();
	HBITMAP hBitmap = pImageBase->GetHandle();

	//source
	CDUIRect rcSource = GetSource(ImageSection);

	//dest
	CDUIRect rcDest = GetDest(rcSource, rcItem);
	rcDest.Offset(rcItem.left, rcItem.top);
	IntersectRect(&rcDest, &rcDest, &rcItem);

	//corner
	CDUIRect rcCorner = bScale ? DuiDpiScaleAttri(ImageSection.rcCorner) : ImageSection.rcCorner;

	//verify
	if (rcDest.IsEmpty()) return;

	//mask
	if (GetMask() > 0xff000000)
	{
		hBitmap = CDUIRenderEngine::CopyBitmap(hBitmap, GetMask());

		if (pWndManager->IsGdiplusRenderImage())
		{
			Gdiplus::Bitmap *pBmp = CDUIRenderEngine::GetAlphaBitmap(hBitmap);
			
			CDUIRenderEngine::DrawImage(hDC, pBmp, rcDest, rcPaint, rcSource,
				rcCorner, ImageSection.cbAlpha, true, ImageSection.bHole, ImageSection.bTiledX, ImageSection.bTiledY);
			
			MMSafeDelete(pBmp);
		}
		else
		{
			CDUIRenderEngine::DrawImage(hDC, hBitmap, rcDest, rcPaint, rcSource,
				rcCorner, ImageSection.cbAlpha, true, ImageSection.bHole, ImageSection.bTiledX, ImageSection.bTiledY);
		}

		MMSafeDeleteObject(hBitmap);

		return;
	}

	//draw
	if (pWndManager->IsGdiplusRenderImage())
	{
		CDUIRenderEngine::DrawImage(hDC, pImageBase->GetBitmap(), rcDest, rcPaint, rcSource,
			rcCorner, ImageSection.cbAlpha, bAlpha, ImageSection.bHole, ImageSection.bTiledX, ImageSection.bTiledY);
	}
	else
	{
		CDUIRenderEngine::DrawImage(hDC, hBitmap, rcDest, rcPaint, rcSource,
			rcCorner, ImageSection.cbAlpha, bAlpha, ImageSection.bHole, ImageSection.bTiledX, ImageSection.bTiledY);
	}

	return;
}

void CDUIAttriImageSection::DrawAnimate(HDC hDC, const CDUIRect &rcItem, const CDUIRect &rcPaint, const tagDuiAnimateImageInfo &AnimateImageInfo, int nFrameCur)
{
	CDUIImageBase *pImageBaseCur = GetCurImageBase();
	if (NULL == pImageBaseCur) return;

	//dest
	CDUIRect rcSource = { 0, 0, pImageBaseCur->GetWidth(), pImageBaseCur->GetHeight() };
	CDUIRect rcDest = GetDest(rcSource, rcItem);
	rcDest.Offset(rcItem.left, rcItem.top);
	IntersectRect(&rcDest, &rcDest, &rcItem);

	//gif
	if (AnimateImage_Gif == AnimateImageInfo.AnimateImageType)
	{
		Gdiplus::Bitmap *pBmpAnimate = pImageBaseCur->GetAnimateImage();
		if (NULL == pBmpAnimate) return;

		CDUIRenderEngine::DrawAnimateImage(hDC, pBmpAnimate, rcDest, nFrameCur);

		return;
	}

	//sequence frame
	CDUISize szSequenceFrameSize = AnimateImageInfo.szSequenceFrameSize;
	if (AnimateImage_SequenceFrame == AnimateImageInfo.AnimateImageType
		&& szSequenceFrameSize.cx > 0
		&& szSequenceFrameSize.cy > 0)
	{
		tagDuiImageSection ImageSection = GetImageSection();

		int nRow = nFrameCur / (pImageBaseCur->GetWidth() / szSequenceFrameSize.cx);
		int nColumn = nFrameCur % (pImageBaseCur->GetWidth() / szSequenceFrameSize.cx);
		CDUIRect rcSource;
		rcSource.left = szSequenceFrameSize.cx * nColumn;
		rcSource.top = szSequenceFrameSize.cy * nRow;
		rcSource.right = rcSource.left + szSequenceFrameSize.cx;
		rcSource.bottom = rcSource.top + szSequenceFrameSize.cy;
		ImageSection.ImageSourceType = ImageSource_Custom;
		ImageSection.mapSourceCustomScale[100] = rcSource;

		Draw(pImageBaseCur, ImageSection, hDC, rcItem, rcPaint);
	}

	return;
}

bool CDUIAttriImageSection::IsEmpty()
{
	tagDuiImageSection ImggeSection = GetImageSection();
	return ImggeSection.vecImageResSwitch.empty();
}

tagDuiImageSection CDUIAttriImageSection::GetImageSection()
{
	return CDUIGlobal::GetInstance()->GetAttriImageSection(m_uValueHash);
}

void CDUIAttriImageSection::SetImageSection(const tagDuiImageSection &ImageSection)
{
	if (ImageSection == GetImageSection()) return;

	m_uValueHash = CDUIGlobal::GetInstance()->SetAttriImageSection(ImageSection);

	NotifyChange();

	return;
}

CDUIImageBase * CDUIAttriImageSection::GetCurImageBase()
{
	tagDuiImageSection ImageSection = GetImageSection();
	int nIndexRes = CDUIGlobal::GetInstance()->GetSwitchResIndex();
	nIndexRes = min(nIndexRes, (int)ImageSection.vecImageResSwitch.size() - 1);
	if (nIndexRes < ImageSection.vecImageResSwitch.size())
	{
		return CDUIGlobal::GetInstance()->GetImageResource(ImageSection.vecImageResSwitch[nIndexRes]);
	}

	return NULL;
}

int CDUIAttriImageSection::GetCurImageWidth()
{
	CDUIImageBase *pImageBase = GetCurImageBase();

	return pImageBase ? pImageBase->GetWidth() : 0;
}

int CDUIAttriImageSection::GetCurImageHeight()
{
	CDUIImageBase *pImageBase = GetCurImageBase();

	return pImageBase ? pImageBase->GetHeight() : 0;
}

void CDUIAttriImageSection::SetImageResSwitch(const vector<CMMString> &vecImageResSwitch)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.vecImageResSwitch = vecImageResSwitch;
	SetImageSection(ImageSection);

	NotifyChange();

	return;
}

void CDUIAttriImageSection::SetImageResSwitch(const CMMString &strImageResSwitch)
{
	return SetImageResSwitch(CMMStrHelp::ParseStrFromString(strImageResSwitch, _T(";")));
}

vector<CMMString> CDUIAttriImageSection::GetImageResSwitch()
{
	tagDuiImageSection ImageSection = GetImageSection();

	return ImageSection.vecImageResSwitch;
}

LPCTSTR CDUIAttriImageSection::GetImageResName()
{
	CDUIImageBase *pImageBaseCur = GetCurImageBase();
	return pImageBaseCur ? pImageBaseCur->GetResourceName() : _T("");
}

CDUIRect CDUIAttriImageSection::GetSource()
{
	return GetSource(GetImageSection());
}

CDUIRect CDUIAttriImageSection::GetSource(tagDuiImageSection ImageSection)
{
	CDUIImageBase *pImageBase = NULL;
	int nIndexRes = CDUIGlobal::GetInstance()->GetSwitchResIndex();
	nIndexRes = min(nIndexRes, (int)ImageSection.vecImageResSwitch.size() - 1);
	if (nIndexRes < ImageSection.vecImageResSwitch.size())
	{
		pImageBase = CDUIGlobal::GetInstance()->GetImageResource(ImageSection.vecImageResSwitch[nIndexRes]);
	}
	if (NULL == pImageBase)
	{
		return {};
	}

	//info
	bool bScale = pImageBase->IsScale();

	CDUIRect rcSource;
	switch (ImageSection.ImageSourceType)
	{
		case ImageSource_Normal:
		{
			rcSource.right = pImageBase->GetWidth();
			rcSource.bottom = pImageBase->GetHeight();

			break;
		}
		case ImageSource_Part:
		{
			if (ImageSection.cbPartAll <= 0)
			{
				assert(false);
				break;
			}

			int nWidth = 0, nHeight = 0;
			if (false == ImageSection.bPartVert)
			{
				nWidth = pImageBase->GetWidth() / ImageSection.cbPartAll;
				nHeight = pImageBase->GetHeight();
				rcSource.right = nWidth * ImageSection.cbPartSel;
				rcSource.bottom = nHeight;
			}
			else
			{
				nHeight = pImageBase->GetHeight() / ImageSection.cbPartAll;
				nWidth = pImageBase->GetWidth();
				rcSource.bottom = nHeight * ImageSection.cbPartSel;
				rcSource.right = nWidth;
			}

			rcSource.left = rcSource.right - nWidth;
			rcSource.top = rcSource.bottom - nHeight;

			break;
		}
		case ImageSource_Custom:
		{
			if (ImageSection.mapSourceCustomScale.find(GetScale()) != ImageSection.mapSourceCustomScale.end())
			{
				rcSource = ImageSection.mapSourceCustomScale[GetScale()];
			}
			else
			{
				rcSource = DuiDpiScaleAttri(CDUIRect());
			}

			break;
		}
	}

	return rcSource;
}

void CDUIAttriImageSection::SetSourceNormal()
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.ImageSourceType = ImageSource_Normal;

	SetImageSection(ImageSection);

	return;
}

void CDUIAttriImageSection::SetSourcePart(BYTE cbPartAll, BYTE cbPartSel)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.ImageSourceType = ImageSource_Part;
	ImageSection.cbPartAll = cbPartAll;
	ImageSection.cbPartSel = cbPartSel;

	SetImageSection(ImageSection);

	return;
}

void CDUIAttriImageSection::SetSourceCustom(const CDUIRect &rcSource)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.ImageSourceType = ImageSource_Custom;
	ImageSection.mapSourceCustomScale[100] = rcSource;

	SetImageSection(ImageSection);

	return;
}

BYTE CDUIAttriImageSection::GetAlpha()
{
	tagDuiImageSection ImageSection = GetImageSection();

	return ImageSection.cbAlpha;
}

void CDUIAttriImageSection::SetAlpha(BYTE cbAlpha)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.cbAlpha = cbAlpha;
	SetImageSection(ImageSection);

	NotifyChange();

	return;
}

ARGB CDUIAttriImageSection::GetMask()
{
	tagDuiImageSection ImageSection = GetImageSection();

	return ImageSection.dwMask;
}

void CDUIAttriImageSection::SetMask(ARGB dwMask)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.dwMask = dwMask;
	SetImageSection(ImageSection);

	NotifyChange();

	return;
}

CDUIRect CDUIAttriImageSection::GetCorner()
{
	tagDuiImageSection ImageSection = GetImageSection();

	return ImageSection.rcCorner;
}

void CDUIAttriImageSection::SetCorner(const CDUIRect &rcCorner)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.rcCorner = rcCorner;
	SetImageSection(ImageSection);

	NotifyChange();

	return;
}

bool CDUIAttriImageSection::IsHole()
{
	tagDuiImageSection ImageSection = GetImageSection();

	return ImageSection.bHole;
}

void CDUIAttriImageSection::SetHole(bool bHole)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.bHole = bHole;
	SetImageSection(ImageSection);

	NotifyChange();

	return;
}

bool CDUIAttriImageSection::IsTiledX()
{
	tagDuiImageSection ImageSection = GetImageSection();

	return ImageSection.bTiledX;
}

void CDUIAttriImageSection::SetTiledX(bool bTiledX)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.bTiledX = bTiledX;
	SetImageSection(ImageSection);

	NotifyChange();

	return;
}

bool CDUIAttriImageSection::IsTiledY()
{
	tagDuiImageSection ImageSection = GetImageSection();

	return ImageSection.bTiledY;
}

void CDUIAttriImageSection::SetTiledY(bool bTiledY)
{
	tagDuiImageSection ImageSection = GetImageSection();
	ImageSection.bTiledY = bTiledY;
	SetImageSection(ImageSection);

	NotifyChange();

	return;
}

bool CDUIAttriImageSection::SetAttribute(LPCSTR lpszName, LPCSTR lpszValue)
{
	if (__super::SetAttribute(lpszName, lpszValue)) return true;

	if (0 == strcmp(lpszName, Dui_Key_AttriObjValueID))
	{
		m_uValueHash = strtoul(lpszValue, NULL, 10);

#ifdef DUI_DESIGN
		CDUIGlobal::GetInstance()->OnAttriValueIDRead(GetAttributeType(), GetValueID());
#endif

		return true;
	}

	return false;
}

uint32_t CDUIAttriImageSection::GetValueID()
{
	return m_uValueHash;
}

void CDUIAttriImageSection::SetValueID(uint32_t uValueID)
{
	m_uValueHash = uValueID;

	return;
}

CDUIRect CDUIAttriImageSection::GetDest(CDUIRect rcSource, CDUIRect rcItem)
{
	tagDuiImageSection ImageSection = GetImageSection();
	CDUIRect rcDest(0, 0, rcItem.GetWidth(), rcItem.GetHeight());

	do
	{
		if (HorizImageAlign_None == ImageSection.HorizImageAlign && VertImageAlign_None == ImageSection.VertImageAlign)
		{
			break;
		}
		switch (ImageSection.HorizImageAlign)
		{
			case HorizImageAlign_Right:
			{
				rcDest.left = rcDest.right - rcSource.GetWidth();
				rcDest.Offset(-ImageSection.nHorizOffset, 0);

				break;
			}
			case HorizImageAlign_Center:
			{
				rcDest.left += (rcDest.GetWidth() - rcSource.GetWidth()) / 2;
				rcDest.left += ImageSection.nHorizOffset;
				rcDest.right = rcDest.left + rcSource.GetWidth();

				break;
			}
			case HorizImageAlign_Left:
			default:
			{
				rcDest.left += ImageSection.nHorizOffset;
				rcDest.right = rcDest.left + rcSource.GetWidth();

				break;
			}
		}
		switch (ImageSection.VertImageAlign)
		{
			case VertImageAlign_Bottom:
			{
				rcDest.top = rcDest.bottom - rcSource.GetHeight();
				rcDest.Offset(0, -ImageSection.nVertOffset);

				break;
			}
			case VertImageAlign_Center:
			{
				rcDest.top += (rcDest.GetHeight() - rcSource.GetHeight()) / 2;
				rcDest.top += ImageSection.nVertOffset;
				rcDest.bottom = rcDest.top + rcSource.GetHeight();

				break;
			}
			case VertImageAlign_Top:
			default:
			{
				rcDest.top += ImageSection.nVertOffset;
				rcDest.bottom = rcDest.top + rcSource.GetHeight();

				break;
			}
		}

	} while (false);

	return rcDest;
}