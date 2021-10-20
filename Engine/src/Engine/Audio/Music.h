#pragma once

typedef struct _Mix_Music Mix_Music;

namespace tsEngine
{
	struct Music
	{
		Mix_Music* Mus{};

		Music() = default;
		Music(const std::string& path);
		~Music();
		
		bool LoadMusic(const std::string& path);
	};
}

