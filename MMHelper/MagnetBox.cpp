#include "StdAfx.h"
#include "MagnetBox.h"

#define MAX_OFFSET_MAGNET					(5)
#define MIN_OFFSET_MAGNET					(1)

//////////////////////////////////////////////////////////////////////////
void CMagnetBox::PushBox(int nIndex, CRect rcBox)
{
	m_mapMagnetBox[nIndex] = rcBox;

	return;
}

void CMagnetBox::RemoveAt(int nIndex)
{
	m_mapMagnetBox.erase(nIndex);

	return;
}

void CMagnetBox::RemoveAll()
{
	m_mapMagnetBox.clear();

	return;
}

void CMagnetBox::SceneInit()
{
	m_CurBoxMagnetInfo = {};
	m_bBoxMoving = false;
	m_ptPullTotal = {};

	return;
}

CRect CMagnetBox::MoveBox(int nIndex, IN CPoint ptOffset)
{
	auto FindIt = m_mapMagnetBox.find(nIndex);
	if (FindIt == m_mapMagnetBox.end())
	{
		assert(false);

		return {};
	}

	CRect rcBox = FindIt->second;

	//can move
	if (false == MoveOffset(nIndex, ptOffset, ptOffset))
	{
		return rcBox;
	}

	//move
	rcBox.OffsetRect(ptOffset);
	FindIt->second = rcBox;
	int nMoveDirection = MagnetType_None, nBorderNeares = MagnetType_None;
	ptOffset.x < 0 ? nMoveDirection |= MagnetType_Left, nBorderNeares |= (MagnetType_Left | MagnetType_Right) : 0;
	ptOffset.x > 0 ? nMoveDirection |= MagnetType_Right, nBorderNeares |= (MagnetType_Left | MagnetType_Right) : 0;
	ptOffset.y < 0 ? nMoveDirection |= MagnetType_Top, nBorderNeares |= (MagnetType_Top | MagnetType_Bottom) : 0;
	ptOffset.y > 0 ? nMoveDirection |= MagnetType_Bottom, nBorderNeares |= (MagnetType_Top | MagnetType_Bottom) : 0;

	//magnet
	tagFindMagnetBox FindMagnetBox;
	FindNearestMagnetBox(nIndex, nMoveDirection, nBorderNeares, FindMagnetBox);
	CRect rcBoxPre = rcBox;
	
	if (-1 != FindMagnetBox.rcBoxLeft.nIndex)
	{
		tagMagnetBox &rcBoxLeft = FindMagnetBox.rcBoxLeft;
		int nLeft = MagnetType_Left == rcBoxLeft.MagnetType ? rcBoxLeft.left : rcBoxLeft.right + MIN_OFFSET_MAGNET;
		rcBox.OffsetRect(POINT{ nLeft - rcBox.left, 0 });
	}
	if (-1 != FindMagnetBox.rcBoxRight.nIndex)
	{
		tagMagnetBox &rcBoxRight = FindMagnetBox.rcBoxRight;
		int nRight = MagnetType_Left == rcBoxRight.MagnetType ? rcBoxRight.left - MIN_OFFSET_MAGNET : rcBoxRight.right;
		rcBoxRight.OffsetRect(POINT{ nRight - rcBox.right, 0 });
	}
	if (-1 != FindMagnetBox.rcBoxTop.nIndex)
	{
		tagMagnetBox &rcBoxTop = FindMagnetBox.rcBoxTop;
		int nTop = MagnetType_Top == rcBoxTop.MagnetType ? rcBoxTop.top : rcBoxTop.bottom + MIN_OFFSET_MAGNET;
		rcBox.OffsetRect(POINT{ 0, nTop - rcBox.top });
	}
	if (-1 != FindMagnetBox.rcBoxBottom.nIndex)
	{
		tagMagnetBox &rcBoxBottom = FindMagnetBox.rcBoxBottom;
		int nBottom = MagnetType_Top == rcBoxBottom.MagnetType ? rcBoxBottom.top - MIN_OFFSET_MAGNET : rcBoxBottom.bottom;
		rcBox.OffsetRect(POINT{ 0, nBottom - rcBox.bottom });
	}
	
	FindIt->second = rcBox;
	
	//init magnetinfo
	if (-1 != FindMagnetBox.rcBoxLeft.nIndex
		|| -1 != FindMagnetBox.rcBoxTop.nIndex
		|| -1 != FindMagnetBox.rcBoxRight.nIndex
		|| -1 != FindMagnetBox.rcBoxBottom.nIndex)
	{
		m_bBoxMoving = false;
		InitBoxMagnetInfo(nIndex);
	}

	return rcBox;
}

CRect CMagnetBox::StretchBox(int nIndex, IN CRect rcStretch)
{
	auto FindIt = m_mapMagnetBox.find(nIndex);
	if (FindIt == m_mapMagnetBox.end())
	{
		assert(false);

		return {};
	}

	CRect rcBox = FindIt->second;

	//can stretch
	if (false == StretchOffset(nIndex, rcStretch, rcStretch))
	{
		return rcBox;
	}

	//stretch
	rcBox.left += rcStretch.left;
	rcBox.top += rcStretch.top;
	rcBox.right += rcStretch.right;
	rcBox.bottom += rcStretch.bottom;
	FindIt->second = rcBox;
	int nMoveDirection = MagnetType_None, nBorderNeares = MagnetType_None;
	rcStretch.left < 0 || rcStretch.right < 0 ? nMoveDirection |= MagnetType_Left : 0;
	rcStretch.left > 0 || rcStretch.right > 0 ? nMoveDirection |= MagnetType_Right : 0;
	rcStretch.top < 0 || rcStretch.bottom < 0 ? nMoveDirection |= MagnetType_Top : 0;
	rcStretch.top > 0 || rcStretch.bottom > 0 ? nMoveDirection |= MagnetType_Bottom : 0;
	rcStretch.left < 0 || rcStretch.left > 0 ? nBorderNeares |= MagnetType_Left : 0;
	rcStretch.right < 0 || rcStretch.right > 0 ? nBorderNeares |= MagnetType_Right : 0;
	rcStretch.top < 0 || rcStretch.top > 0 ? nBorderNeares |= MagnetType_Top : 0;
	rcStretch.bottom < 0 || rcStretch.bottom > 0 ? nBorderNeares |= MagnetType_Bottom : 0;

	//magnet
	tagFindMagnetBox FindMagnetBox;
	FindNearestMagnetBox(nIndex, nMoveDirection, nBorderNeares, FindMagnetBox);
	CRect rcBoxPre = rcBox;

	if (-1 != FindMagnetBox.rcBoxLeft.nIndex)
	{
		tagMagnetBox &rcBoxLeft = FindMagnetBox.rcBoxLeft;
		int nLeft = MagnetType_Left == rcBoxLeft.MagnetType ? rcBoxLeft.left : rcBoxLeft.right + MIN_OFFSET_MAGNET;
		rcBox.left = nLeft;
	}
	if (-1 != FindMagnetBox.rcBoxRight.nIndex)
	{
		tagMagnetBox &rcBoxRight = FindMagnetBox.rcBoxRight;
		int nRight = MagnetType_Left == rcBoxRight.MagnetType ? rcBoxRight.left - MIN_OFFSET_MAGNET : rcBoxRight.right;
		rcBoxRight.right = nRight;
	}
	if (-1 != FindMagnetBox.rcBoxTop.nIndex)
	{
		tagMagnetBox &rcBoxTop = FindMagnetBox.rcBoxTop;
		int nTop = MagnetType_Top == rcBoxTop.MagnetType ? rcBoxTop.top : rcBoxTop.bottom + MIN_OFFSET_MAGNET;
		rcBox.top = nTop;
	}
	if (-1 != FindMagnetBox.rcBoxBottom.nIndex)
	{
		tagMagnetBox &rcBoxBottom = FindMagnetBox.rcBoxBottom;
		int nBottom = MagnetType_Top == rcBoxBottom.MagnetType ? rcBoxBottom.top - MIN_OFFSET_MAGNET : rcBoxBottom.bottom;
		rcBox.bottom = nBottom;
	}

	FindIt->second = rcBox;

	//init magnetinfo
	if (-1 != FindMagnetBox.rcBoxLeft.nIndex 
		|| -1 != FindMagnetBox.rcBoxTop.nIndex
		|| -1 != FindMagnetBox.rcBoxRight.nIndex
		|| -1 != FindMagnetBox.rcBoxBottom.nIndex)
	{
		m_bBoxMoving = false;
		InitBoxMagnetInfo(nIndex);
	}

	return rcBox;
}

void CMagnetBox::InitBoxMagnetInfo(int nIndex)
{
	if (m_bBoxMoving) return;

	m_CurBoxMagnetInfo = {};
	FindNearestMagnetBox(nIndex, MagnetType_None, MagnetType_None, m_CurBoxMagnetInfo);

	return;
}

bool CMagnetBox::MoveOffset(int nIndex, IN CPoint ptOffset, OUT CPoint &ptOffsetRealy)
{
	if (false == m_bBoxMoving)
	{
		InitBoxMagnetInfo(nIndex);

		m_ptPullTotal.x += ptOffset.x;
		m_ptPullTotal.y += ptOffset.y;

		if ((-1 != m_CurBoxMagnetInfo.rcBoxLeft.nIndex || -1 != m_CurBoxMagnetInfo.rcBoxRight.nIndex)
			&& abs(m_ptPullTotal.x) < MAX_OFFSET_MAGNET)
		{
			ptOffset.x = 0;
		}
		if ((-1 != m_CurBoxMagnetInfo.rcBoxTop.nIndex || -1 != m_CurBoxMagnetInfo.rcBoxBottom.nIndex)
			&& abs(m_ptPullTotal.y) < MAX_OFFSET_MAGNET)
		{
			ptOffset.y = 0;
		}
		if (0 == ptOffset.x && 0 == ptOffset.y)
		{
			ptOffsetRealy = ptOffset;
			return false;
		}

		m_bBoxMoving = true;
		m_CurBoxMagnetInfo = {};
		ptOffset = m_ptPullTotal;
		m_ptPullTotal = {};
	}

	ptOffsetRealy = ptOffset;

	return true;
}

bool CMagnetBox::StretchOffset(int nIndex, IN CRect rcStretch, OUT CRect &rcStretchRealy)
{
	if (false == m_bBoxMoving)
	{
		InitBoxMagnetInfo(nIndex);

		m_ptPullTotal.x += rcStretch.left;
		m_ptPullTotal.x += rcStretch.right;
		m_ptPullTotal.y += rcStretch.top;
		m_ptPullTotal.y += rcStretch.bottom;

		if ((-1 != m_CurBoxMagnetInfo.rcBoxLeft.nIndex || -1 != m_CurBoxMagnetInfo.rcBoxRight.nIndex)
			&& abs(m_ptPullTotal.x) < MAX_OFFSET_MAGNET)
		{
			rcStretch.left = rcStretch.right = 0;
		}
		if ((-1 != m_CurBoxMagnetInfo.rcBoxTop.nIndex || -1 != m_CurBoxMagnetInfo.rcBoxBottom.nIndex)
			&& abs(m_ptPullTotal.y) < MAX_OFFSET_MAGNET)
		{
			rcStretch.top = rcStretch.bottom = 0;
		}
		if (0 == rcStretch.left && 0 == rcStretch.right && 0 == rcStretch.top && 0 == rcStretch.bottom)
		{
			rcStretchRealy = rcStretch;
			return false;
		}

		m_bBoxMoving = true;
		m_CurBoxMagnetInfo = {};
		0 != rcStretch.left ? rcStretch.left += m_ptPullTotal.x : rcStretch.right += m_ptPullTotal.x;
		0 != rcStretch.top ? rcStretch.top += m_ptPullTotal.y : rcStretch.bottom += m_ptPullTotal.y;
		m_ptPullTotal = {};
	}

	rcStretchRealy = rcStretch;

	return true;
}

void CMagnetBox::FindNearestMagnetBox(int nIndex, int nMoveDirection, int nBorderNearest, tagFindMagnetBox &FindMagnetBox)
{
	auto FindIt = m_mapMagnetBox.find(nIndex);
	if (FindIt == m_mapMagnetBox.end()) return;

	CRect rcBox = FindIt->second;
	for (auto &BoxItem : m_mapMagnetBox)
	{
		if(BoxItem.first == nIndex) continue;

		CRect rcBoxItem = BoxItem.second;

		//no direction
		if (MagnetType_None == nMoveDirection)
		{
			//left
			if (-1 == FindMagnetBox.rcBoxLeft.nIndex 
				&& (rcBox.left == rcBoxItem.left || MIN_OFFSET_MAGNET == rcBox.left - rcBoxItem.right))
			{
				FindMagnetBox.rcBoxLeft = rcBoxItem;
				FindMagnetBox.rcBoxLeft.nIndex = BoxItem.first;
				FindMagnetBox.rcBoxLeft.MagnetType = rcBox.left == rcBoxItem.left ? MagnetType_Left : MagnetType_Right;
			}
			//top
			if (-1 == FindMagnetBox.rcBoxTop.nIndex
				&& (rcBox.top == rcBoxItem.top || MIN_OFFSET_MAGNET == rcBox.top - rcBoxItem.bottom))
			{
				FindMagnetBox.rcBoxTop = rcBoxItem;
				FindMagnetBox.rcBoxTop.nIndex = BoxItem.first;
				FindMagnetBox.rcBoxTop.MagnetType = rcBox.top == rcBoxItem.top ? MagnetType_Top : MagnetType_Bottom;
			}
			//right
			if (-1 == FindMagnetBox.rcBoxRight.nIndex
				&& (MIN_OFFSET_MAGNET == rcBoxItem.left - rcBox.right || rcBoxItem.right == rcBox.right))
			{
				FindMagnetBox.rcBoxRight = rcBoxItem;
				FindMagnetBox.rcBoxRight.nIndex = BoxItem.first;
				FindMagnetBox.rcBoxRight.MagnetType = rcBoxItem.right == rcBox.right ? MagnetType_Right : MagnetType_Left;
			}
			//bottom
			if (-1 == FindMagnetBox.rcBoxBottom.nIndex
				&& (MIN_OFFSET_MAGNET == rcBoxItem.top - rcBox.bottom || rcBoxItem.bottom == rcBox.bottom))
			{
				FindMagnetBox.rcBoxBottom = rcBoxItem;
				FindMagnetBox.rcBoxBottom.nIndex = BoxItem.first;
				FindMagnetBox.rcBoxBottom.MagnetType = rcBoxItem.bottom == rcBox.bottom ? MagnetType_Bottom : MagnetType_Top;
			}

			continue;
		}

		//left direction
		if (nMoveDirection & MagnetType_Left)
		{
			VerifyBoxLeftDirection(BoxItem.first, rcBoxItem, rcBox, nBorderNearest, FindMagnetBox);
		}
		//top direction
		if (nMoveDirection & MagnetType_Top)
		{
			VerifyBoxTopDirection(BoxItem.first, rcBoxItem, rcBox, nBorderNearest, FindMagnetBox);
		}
		//right direction
		if (nMoveDirection & MagnetType_Right)
		{
			VerifyBoxRightDirection(BoxItem.first, rcBoxItem, rcBox, nBorderNearest, FindMagnetBox);
		}
		//bottom direction
		if (nMoveDirection & MagnetType_Bottom)
		{
			VerifyBoxBottomDirection(BoxItem.first, rcBoxItem, rcBox, nBorderNearest, FindMagnetBox);
		}
	}

	return;
}

void CMagnetBox::VerifyBoxLeftDirection(int nIndex, CRect rcBoxItem, CRect rcBox, int nBorderNearest, tagFindMagnetBox &FindMagnetBox)
{
	//left
	auto &FindBoxLeft = FindMagnetBox.rcBoxLeft;
	if (nBorderNearest & MagnetType_Left)
	{
		if ((rcBoxItem.right <= rcBox.left && rcBoxItem.right + MAX_OFFSET_MAGNET >= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Left == FindBoxLeft.MagnetType && rcBoxItem.right > FindBoxLeft.left)))
			|| (rcBoxItem.right <= rcBox.left && rcBoxItem.right + MAX_OFFSET_MAGNET >= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Right == FindBoxLeft.MagnetType && rcBoxItem.right > FindBoxLeft.right)))
			|| (rcBoxItem.left <= rcBox.left && rcBoxItem.left + MAX_OFFSET_MAGNET >= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Left == FindBoxLeft.MagnetType && rcBoxItem.left > FindBoxLeft.left)))
			|| (rcBoxItem.left <= rcBox.left && rcBoxItem.left + MAX_OFFSET_MAGNET >= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Right == FindBoxLeft.MagnetType && rcBoxItem.left > FindBoxLeft.right))))
		{
			FindBoxLeft = rcBoxItem;
			FindBoxLeft.nIndex = nIndex;
			FindBoxLeft.MagnetType = rcBoxItem.right <= rcBox.left ? MagnetType_Right : MagnetType_Left;
		}
	}

	//right
	auto &FindBoxRight = FindMagnetBox.rcBoxRight;
	if (nBorderNearest & MagnetType_Right)
	{
		if ((rcBoxItem.right <= rcBox.right && rcBoxItem.right + MAX_OFFSET_MAGNET >= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Left == FindBoxRight.MagnetType && rcBoxItem.right > FindBoxRight.left)))
			|| (rcBoxItem.right <= rcBox.right && rcBoxItem.right + MAX_OFFSET_MAGNET >= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Right == FindBoxRight.MagnetType && rcBoxItem.right > FindBoxRight.right)))
			|| (rcBoxItem.left <= rcBox.right && rcBoxItem.left + MAX_OFFSET_MAGNET >= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Left == FindBoxRight.MagnetType && rcBoxItem.left > FindBoxRight.left)))
			|| (rcBoxItem.left <= rcBox.right && rcBoxItem.left + MAX_OFFSET_MAGNET >= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Right == FindBoxRight.MagnetType && rcBoxItem.left > FindBoxRight.right))))
		{
			FindBoxRight = rcBoxItem;
			FindBoxRight.nIndex = nIndex;
			FindBoxRight.MagnetType = rcBoxItem.right <= rcBox.right ? MagnetType_Right : MagnetType_Left;
		}
	}

	//only one
	if (MagnetType_None != FindBoxLeft.MagnetType && MagnetType_None != FindBoxRight.MagnetType)
	{
		int nOffsetLeft = MagnetType_Left == FindBoxLeft.MagnetType ? abs(rcBox.left - FindBoxLeft.left) : abs(rcBox.left - FindBoxLeft.right);
		int nOffsetRight = MagnetType_Right == FindBoxRight.MagnetType ? abs(rcBox.right - FindBoxRight.right) : abs(rcBox.right - FindBoxRight.left);
		if (nOffsetLeft <= nOffsetRight)
		{
			FindBoxRight.nIndex = -1;
			FindBoxRight.MagnetType = MagnetType_None;
		}
		else
		{
			FindBoxLeft.nIndex = -1;
			FindBoxLeft.MagnetType = MagnetType_None;
		}
	}

	return;
}

void CMagnetBox::VerifyBoxRightDirection(int nIndex, CRect rcBoxItem, CRect rcBox, int nBorderNearest, tagFindMagnetBox &FindMagnetBox)
{
	//right
	auto &FindBoxRight = FindMagnetBox.rcBoxRight;
	if (nBorderNearest & MagnetType_Right)
	{
		if ((rcBoxItem.left >= rcBox.right && rcBoxItem.left - MAX_OFFSET_MAGNET <= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Left == FindBoxRight.MagnetType && rcBoxItem.left < FindBoxRight.left)))
			|| (rcBoxItem.left >= rcBox.right && rcBoxItem.left - MAX_OFFSET_MAGNET <= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Right == FindBoxRight.MagnetType && rcBoxItem.left < FindBoxRight.right)))
			|| (rcBoxItem.right >= rcBox.right && rcBoxItem.right - MAX_OFFSET_MAGNET <= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Left == FindBoxRight.MagnetType && rcBoxItem.right > FindBoxRight.left)))
			|| (rcBoxItem.right >= rcBox.right && rcBoxItem.right - MAX_OFFSET_MAGNET <= rcBox.right && (-1 == FindBoxRight.nIndex || (MagnetType_Right == FindBoxRight.MagnetType && rcBoxItem.right > FindBoxRight.right))))
		{
			FindBoxRight = rcBoxItem;
			FindBoxRight.nIndex = nIndex;
			FindBoxRight.MagnetType = rcBoxItem.left >= rcBox.right ? MagnetType_Left : MagnetType_Right;
		}
	}

	//left
	auto &FindBoxLeft = FindMagnetBox.rcBoxLeft;
	if (nBorderNearest & MagnetType_Left)
	{
		if ((rcBoxItem.right >= rcBox.left && rcBoxItem.right - MAX_OFFSET_MAGNET <= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Left == FindBoxLeft.MagnetType && rcBoxItem.right < FindBoxLeft.left)))
			|| (rcBoxItem.right >= rcBox.left && rcBoxItem.right - MAX_OFFSET_MAGNET <= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Right == FindBoxLeft.MagnetType && rcBoxItem.right < FindBoxLeft.right)))
			|| (rcBoxItem.left >= rcBox.left && rcBoxItem.left - MAX_OFFSET_MAGNET <= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Left == FindBoxLeft.MagnetType && rcBoxItem.left < FindBoxLeft.left)))
			|| (rcBoxItem.left >= rcBox.left && rcBoxItem.left - MAX_OFFSET_MAGNET <= rcBox.left && (-1 == FindBoxLeft.nIndex || (MagnetType_Right == FindBoxLeft.MagnetType && rcBoxItem.left < FindBoxLeft.right))))
		{
			FindBoxLeft = rcBoxItem;
			FindBoxLeft.nIndex = nIndex;
			FindBoxLeft.MagnetType = rcBoxItem.right >= rcBox.left ? MagnetType_Right : MagnetType_Left;
		}
	}

	//only one
	if (MagnetType_None != FindBoxLeft.MagnetType && MagnetType_None != FindBoxRight.MagnetType)
	{
		int nOffsetLeft = MagnetType_Left == FindBoxLeft.MagnetType ? abs(rcBox.left - FindBoxLeft.left) : abs(rcBox.left - FindBoxLeft.right);
		int nOffsetRight = MagnetType_Right == FindBoxRight.MagnetType ? abs(rcBox.right - FindBoxRight.right) : abs(rcBox.right - FindBoxRight.left);
		if (nOffsetLeft <= nOffsetRight)
		{
			FindBoxRight.nIndex = -1;
			FindBoxRight.MagnetType = MagnetType_None;
		}
		else
		{
			FindBoxLeft.nIndex = -1;
			FindBoxLeft.MagnetType = MagnetType_None;
		}
	}

	return;
}

void CMagnetBox::VerifyBoxTopDirection(int nIndex, CRect rcBoxItem, CRect rcBox, int nBorderNearest, tagFindMagnetBox &FindMagnetBox)
{
	//top
	auto &FindBoxTop = FindMagnetBox.rcBoxTop;
	if (nBorderNearest & MagnetType_Top)
	{
		if ((rcBoxItem.bottom <= rcBox.top && rcBoxItem.bottom + MAX_OFFSET_MAGNET >= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Top == FindBoxTop.MagnetType && rcBoxItem.bottom > FindBoxTop.top)))
			|| (rcBoxItem.bottom <= rcBox.top && rcBoxItem.bottom + MAX_OFFSET_MAGNET >= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Bottom == FindBoxTop.MagnetType && rcBoxItem.bottom > FindBoxTop.bottom)))
			|| (rcBoxItem.top <= rcBox.top && rcBoxItem.top + MAX_OFFSET_MAGNET >= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Top == FindBoxTop.MagnetType && rcBoxItem.top > FindBoxTop.top)))
			|| (rcBoxItem.top <= rcBox.top && rcBoxItem.top + MAX_OFFSET_MAGNET >= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Bottom == FindBoxTop.MagnetType && rcBoxItem.top > FindBoxTop.bottom))))
		{
			FindBoxTop = rcBoxItem;
			FindBoxTop.nIndex = nIndex;
			FindBoxTop.MagnetType = rcBoxItem.bottom <= rcBox.top ? MagnetType_Bottom : MagnetType_Top;
		}
	}

	//bottom
	auto &FindBoxBottom = FindMagnetBox.rcBoxBottom;
	if (nBorderNearest & MagnetType_Bottom)
	{
		if ((rcBoxItem.bottom <= rcBox.bottom && rcBoxItem.bottom + MAX_OFFSET_MAGNET >= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Top == FindBoxBottom.MagnetType && rcBoxItem.bottom > FindBoxBottom.top)))
			|| (rcBoxItem.bottom <= rcBox.bottom && rcBoxItem.bottom + MAX_OFFSET_MAGNET >= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Bottom == FindBoxBottom.MagnetType && rcBoxItem.bottom > FindBoxBottom.bottom)))
			|| (rcBoxItem.top <= rcBox.bottom && rcBoxItem.top + MAX_OFFSET_MAGNET >= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Top == FindBoxBottom.MagnetType && rcBoxItem.top > FindBoxBottom.top)))
			|| (rcBoxItem.top <= rcBox.bottom && rcBoxItem.top + MAX_OFFSET_MAGNET >= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Bottom == FindBoxBottom.MagnetType && rcBoxItem.top > FindBoxBottom.bottom))))
		{
			FindBoxBottom = rcBoxItem;
			FindBoxBottom.nIndex = nIndex;
			FindBoxBottom.MagnetType = rcBoxItem.bottom <= rcBox.bottom ? MagnetType_Bottom : MagnetType_Top;
		}
	}

	//only one
	if (MagnetType_None != FindBoxTop.MagnetType && MagnetType_None != FindBoxBottom.MagnetType)
	{
		int nOffsetTop = MagnetType_Top == FindBoxTop.MagnetType ? abs(rcBox.top - FindBoxTop.top) : abs(rcBox.top - FindBoxTop.bottom);
		int nOffsetBottom = MagnetType_Bottom == FindBoxBottom.MagnetType ? abs(rcBox.bottom - FindBoxBottom.bottom) : abs(rcBox.bottom - FindBoxBottom.top);
		if (nOffsetTop <= nOffsetBottom)
		{
			FindBoxBottom.nIndex = -1;
			FindBoxBottom.MagnetType = MagnetType_None;
		}
		else
		{
			FindBoxTop.nIndex = -1;
			FindBoxTop.MagnetType = MagnetType_None;
		}
	}

	return;
}

void CMagnetBox::VerifyBoxBottomDirection(int nIndex, CRect rcBoxItem, CRect rcBox, int nBorderNearest, tagFindMagnetBox &FindMagnetBox)
{
	//bottom
	auto &FindBoxBottom = FindMagnetBox.rcBoxBottom;
	if (nBorderNearest & MagnetType_Bottom)
	{
		if ((rcBoxItem.top >= rcBox.bottom && rcBoxItem.top - MAX_OFFSET_MAGNET <= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Top == FindBoxBottom.MagnetType && rcBoxItem.top < FindBoxBottom.top)))
			|| (rcBoxItem.top >= rcBox.bottom && rcBoxItem.top - MAX_OFFSET_MAGNET <= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Bottom == FindBoxBottom.MagnetType && rcBoxItem.top < FindBoxBottom.bottom)))
			|| (rcBoxItem.bottom >= rcBox.bottom && rcBoxItem.bottom - MAX_OFFSET_MAGNET <= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Top == FindBoxBottom.MagnetType && rcBoxItem.bottom < FindBoxBottom.top)))
			|| (rcBoxItem.bottom >= rcBox.bottom && rcBoxItem.bottom - MAX_OFFSET_MAGNET <= rcBox.bottom && (-1 == FindBoxBottom.nIndex || (MagnetType_Bottom == FindBoxBottom.MagnetType && rcBoxItem.bottom < FindBoxBottom.bottom))))
		{
			FindBoxBottom = rcBoxItem;
			FindBoxBottom.nIndex = nIndex;
			FindBoxBottom.MagnetType = rcBoxItem.top >= rcBox.bottom ? MagnetType_Top : MagnetType_Bottom;
		}
	}

	//top
	auto &FindBoxTop = FindMagnetBox.rcBoxTop;
	if (nBorderNearest & MagnetType_Top)
	{
		if ((rcBoxItem.bottom >= rcBox.top && rcBoxItem.bottom - MAX_OFFSET_MAGNET <= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Top == FindBoxTop.MagnetType && rcBoxItem.bottom > FindBoxTop.top)))
			|| (rcBoxItem.bottom >= rcBox.top && rcBoxItem.bottom - MAX_OFFSET_MAGNET <= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Bottom == FindBoxTop.MagnetType && rcBoxItem.bottom > FindBoxTop.bottom)))
			|| (rcBoxItem.top >= rcBox.top && rcBoxItem.top - MAX_OFFSET_MAGNET <= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Top == FindBoxTop.MagnetType && rcBoxItem.top > FindBoxTop.top)))
			|| (rcBoxItem.top >= rcBox.top && rcBoxItem.top - MAX_OFFSET_MAGNET <= rcBox.top && (-1 == FindBoxTop.nIndex || (MagnetType_Bottom == FindBoxTop.MagnetType && rcBoxItem.top > FindBoxTop.bottom))))
		{
			FindBoxTop = rcBoxItem;
			FindBoxTop.nIndex = nIndex;
			FindBoxTop.MagnetType = rcBoxItem.bottom >= rcBox.top ? MagnetType_Bottom : MagnetType_Top;
		}
	}

	//only one
	if (MagnetType_None != FindBoxTop.MagnetType && MagnetType_None != FindBoxBottom.MagnetType)
	{
		int nOffsetTop = MagnetType_Top == FindBoxTop.MagnetType ? abs(rcBox.top - FindBoxTop.top) : abs(rcBox.top - FindBoxTop.bottom);
		int nOffsetBottom = MagnetType_Bottom == FindBoxBottom.MagnetType ? abs(rcBox.bottom - FindBoxBottom.bottom) : abs(rcBox.bottom - FindBoxBottom.top);
		if (nOffsetTop <= nOffsetBottom)
		{
			FindBoxBottom.nIndex = -1;
			FindBoxBottom.MagnetType = MagnetType_None;
		}
		else
		{
			FindBoxTop.nIndex = -1;
			FindBoxTop.MagnetType = MagnetType_None;
		}
	}

	return;
}