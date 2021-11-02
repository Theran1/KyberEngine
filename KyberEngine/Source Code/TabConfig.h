#ifndef __TAB_CONFIG_H__
#define __TAB_CONFIG_H__

#include "Tab.h"

#define FPS_LOG_SIZE 100

class TabConfig : public Tab
{
public:
	TabConfig(Application* app);
	~TabConfig();

	update_status Update();

	void UpdateFrameInfo(float framerate, float frametime);

private:
	std::vector<float> framerateLog;
	std::vector<float> frametimeLog;
};

#endif // __TAB_CONFIG_H__