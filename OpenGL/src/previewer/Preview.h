#pragma once

namespace Preview
{
	enum class DispayMode
	{
		CameraView = 0,
		Mesh = 1,
		Edit = 2
	};

	enum class EditMode
	{
		Select = 0,
		Translate = 1,
		Rotate = 2,
		Scale = 3
	};

	enum class RotateMode
	{
		Y = 0,
		X = 1,
		XY = 2,
		Z = 3
	};

	enum class TransformMode
	{
		Local = 0,
		Global = 1
	};

	extern bool gRendering;
	extern bool gShowCoordnates;
	extern bool gLockCamera;
	extern bool gEnableCameraRotation;
	extern bool gMouseLeftDown;
	extern bool gMouseRightDown;
	extern bool gMouseMiddleDown;
	extern bool gDrawWireFrame;
	extern float gMouseX, gMouseY;
	extern DispayMode gDisplayMode;
	extern EditMode gEditMode;
	extern RotateMode gRotateMode;
	extern TransformMode gTransformMode;
}