#pragma once

#ifdef SINDY_EXPORTS
#define SINDY_API __declspec(dllexport)
#else
// ʹ�����߲���Ҫ��дһ��
#define SINDY_API __declspec(dllimport)
#pragma comment(lib, "Sindy.lib")
#pragma message("link Sindy.lib")
#endif



#include "Sindy\Common\StringUtil.h"
#include "Sindy\Common\UtilityClass.h"
#include "Sindy\Common\template.h"
#include "Sindy\Common\ConstDefine.h"

#include "Sindy\SQLite\SQLiteUtility.h"

#include "Sindy\RangeBound\RangeBound.h"

#include "Sindy\Test\TestRangeBound.h"
