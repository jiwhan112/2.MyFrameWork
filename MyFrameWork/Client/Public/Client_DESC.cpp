#include "stdafx.h"

tag_TERRAIN_DESC::~tag_TERRAIN_DESC()
{

	Safe_Delete_Array(mArrayIndes);
	Safe_Delete_Array(mArrayModelObjects);


}
