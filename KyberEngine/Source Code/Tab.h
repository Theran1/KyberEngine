#ifndef __TAB_H__
#define __TAB_H__

#include "Globals.h"

class Application;

class Tab
{
public:
	Tab(Application* application) : app(application) {}
	virtual ~Tab() {}

	virtual update_status Update()
	{
		return update_status::UPDATE_CONTINUE;
	}

public:
	bool active = false;
	Application* app;
};

#endif // __TAB_H__