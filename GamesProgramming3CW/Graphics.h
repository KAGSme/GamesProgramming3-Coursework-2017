#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "Common.h"

class Graphics
{
private:
	static ivec2 viewportRect;

	Graphics();

public:
	~Graphics();

	static void Init();
	static void SetViewport(int width, int height);
	static ivec2 GetViewport() { return viewportRect; }
};

#endif
