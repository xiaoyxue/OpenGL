#include "Preview.h"

namespace Preview
{
	bool gRendering;
	bool gShowCoordnates;
	bool gLockCamera;
	bool gEnableCameraRotation;
	bool gMouseLeftDown;
	bool gMouseRightDown;
	bool gMouseMiddleDown;
	bool gDrawWireFrame;
	float gMouseX, gMouseY;
	DispayMode gDisplayMode;
	EditMode gEditMode;
	RotateMode gRotateMode;
	TransformMode gTransformMode;
	bool gUseGbuffer = true;
}

