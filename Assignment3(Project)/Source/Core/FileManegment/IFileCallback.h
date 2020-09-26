#pragma once
#include "..\..\Defines.h"

//Callback-Class
class IFileCallback
{
protected:
	virtual ~IFileCallback() {}

public:
	virtual void OnFileModified() = 0;
};