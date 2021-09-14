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
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		
		--spdlog
		"%{prj.name}/vendor/spdlog/include"
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
		"Engine/vendor/spdlog/include"
	}

	links
	{
		--Engine
		"Engine"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
