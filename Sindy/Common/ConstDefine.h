#pragma once


#define _New(TYPE) new TYPE
#define _NewParm(TYPE,Parm) new TYPE Parm
#define _Delete(TYPE) {delete TYPE;(TYPE)=nullptr;}

#define _TEST_DB_NAME _T("TestData.db")

namespace Sindy
{
} // namespace Sindy