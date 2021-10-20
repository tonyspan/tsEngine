workspace "tsEngine"
	architecture "x64"
	startproject "Sandbox"

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
	characterset ("MBCS")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Engine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

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

		--yaml
		"%{prj.name}/vendor/yaml-cpp/include",

		--ImGui
		"%{prj.name}/vendor/imgui",

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
		"SDL2_image",
		"SDL2_ttf",
		"SDL2_mixer",

		"imgui",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

group "Dependencies"
	include "Engine/vendor/imgui"
	include "Engine/vendor/yaml-cpp"
group ""

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	characterset ("MBCS")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/src",

		"Engine/vendor/spdlog/include",
		"Engine/vendor/glm",
		"Engine/vendor/SDL2/include",
		"Engine/vendor/imgui",
		"Engine/vendor/yaml-cpp/include",
		"Engine/vendor/entt/include"
	}

	libdirs
	{
		"Engine/vendor/SDL2/lib"
	}

	links
	{	
		"Engine"
	}

	postbuildcommands
	{
		"{COPY} %{wks.location}/Engine/vendor/SDL2/lib/**.dll %{cfg.targetdir}"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "Sandbox-NativeScript"
	location "Sandbox-NativeScript"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	characterset ("MBCS")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/src",

		"Engine/vendor/spdlog/include",
		"Engine/vendor/glm",
		"Engine/vendor/SDL2/include",
		"Engine/vendor/imgui",
		"Engine/vendor/yaml-cpp/include",
		"Engine/vendor/entt/include"
	}

	libdirs
	{
		"Engine/vendor/SDL2/lib"
	}

	links
	{	
		"Engine"
	}

	postbuildcommands
	{
		"{COPY} %{wks.location}/Engine/vendor/SDL2/lib/**.dll %{cfg.targetdir}"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "EngineEd"
	location "EngineEd"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	characterset ("MBCS")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/src",

		"Engine/vendor/spdlog/include",
		"Engine/vendor/glm",
		"Engine/vendor/SDL2/include",
		"Engine/vendor/imgui",
		"Engine/vendor/yaml-cpp/include",
		"Engine/vendor/entt/include"
	}

	libdirs
	{
		"Engine/vendor/SDL2/lib"
	}

	links
	{	
		"Engine"
	}

	postbuildcommands
	{
		"{COPY} %{wks.location}/Engine/vendor/SDL2/lib/**.dll %{cfg.targetdir}"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
