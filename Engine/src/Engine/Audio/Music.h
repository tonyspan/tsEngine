#pragma once

typedef struct _Mix_Music Mix_Music;

namespace tsEngine
{
	struct Music
	{
	public:
		Music() = default;
		Music(const std::string& path);
		~Music();
		
		bool LoadMusic(const std::string& path);
	private:
		Mix_Music* m_Music{};

		friend class AudioSystem;
	};
}

