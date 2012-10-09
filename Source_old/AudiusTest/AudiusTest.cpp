// AudiusTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "WinUnit.h"

BEGIN_TEST(CanRunTests)
{
	WIN_ASSERT_EQUAL(1, 1);
}
END_TEST
