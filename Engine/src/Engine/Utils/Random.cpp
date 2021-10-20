#include "pch.h"

#include "Random.h"

namespace tsEngine
{
	void Random::Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	float Random::Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<Random::result_t> Random::s_Distribution;
}