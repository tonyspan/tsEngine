workspace "tsEngine"
	architecture "x64"
	startproject "Game"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		--glm
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

		--SDL2
		"%{prj.name}/vendor/SDL2/include/**.h",

		--entt
		"%{prj.name}/vendor/entt/include/*.hpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		
		--spdlog
		"%{prj.name}/vendor/spdlog/include",

		--glm
		"%{prj.name}/vendor/glm",

		--SDL2
		"%{prj.name}/vendor/SDL2/include",

		--entt
		"%{prj.name}/vendor/entt/include"
	}

	libdirs
	{
		--SDL2
		"%{prj.name}/vendor/SDL2/lib"
	}

	links
	{
		"SDL2",
		"SDL2main",
		"SDL2test",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		--Engine
		"Engine/src",
		"Engine/vendor/spdlog/include",
		"Engine/vendor/SDL2/include"
	}

	libdirs
	{
		--SDL2
		"Engine/vendor/SDL2/lib"
	}

	links
	{
		--SDL2
		"SDL2",
		"SDL2main",
		"SDL2test",
		
		--Engine
		"Engine"
	}

	postbuildcommands
	{
		"{COPY} %{wks.location}/Engine/vendor/SDL2/lib/SDL2.dll %{cfg.targetdir}"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
