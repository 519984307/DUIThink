// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <CommDlg.h>

//////////////////////////////////////////////////////////////////////////
#include "../../../DUIThink/DUIThinkHead.h"

#ifdef _DEBUG
#pragma comment(lib, "../../../lib/MMHelperD.lib")
#pragma comment(lib, "../../../lib/DUIThinkD.lib")
#else
#pragma comment(lib, "../../../lib/MMHelper.lib")
#pragma comment(lib, "../../../lib/DUIThink.lib")
#endif

//////////////////////////////////////////////////////////////////////////
#include "../DUIThinkDemo.DuiProj/ResourceCtrlID.h"
#include "Define.h"
#include "BaseControl/StaticView/StaticView.h"
#include "BaseControl/CheckBoxView/CheckBoxView.h"
#include "BaseControl/RadioBoxView/RadioBoxView.h"
#include "BaseControl/ProgressView/ProgressView.h"
#include "BaseControl/ComboxView/ComboxView.h"
#include "Usage/DpiDlg/DpiDlgView.h"
#include "Usage/SvgImage/SvgImageView.h"
#include "Usage/SwitchSkin/SwitchSkinView.h"
#include "Usage/AnimateDlg/AnimateDlgView.h"
#include "QQ/QQView.h"
#include "QQ/DlgQQFace.h"
#include "QQ/DlgQQChat.h"
#include "DlgDpi.h"
#include "DlgDemo.h"

//////////////////////////////////////////////////////////////////////////
extern CDUIGlobalHelper g_DuiGlobal;