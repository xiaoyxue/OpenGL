#pragma once
#include "light/Light.h"
#include "visual/EvnMap.h"

namespace Light {
	using namespace Visual;

	class EnvLight : public Light {


	private:
		std::unique_ptr<EnvironmentMap> mpEnvMap;
		Vec3 mCenter;
		real mRadius;
	};
}