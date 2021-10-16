#ifndef __TAB_ABOUT_H__
#define __TAB_ABOUT_H__

#include "Tab.h"

class TabAbout : public Tab
{
public:
	TabAbout(Application* app);
	~TabAbout();

	update_status Update();
};

#endif // __TAB_ABOUT_H__