#pragma once

namespace tsEngine
{
	class Random
	{
	public:
		static void Init();

		static float Float();

		template<typename T>
		static T Ranged(T from, T to)
		{
			// NOTE: To be random each run
			std::mt19937 randomEngine(std::chrono::system_clock::now().time_since_epoch().count());

			s_Distribution.param(param_t(from, to));

			return (T)s_Distribution(randomEngine);
		}
	private:
		using result_t = std::mt19937::result_type;
		using param_t = std::uniform_int_distribution<result_t>::param_type;

		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<result_t> s_Distribution;
	};
}