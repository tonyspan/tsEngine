project "imgui"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.h",
		"*.cpp"
	}

	includedirs
	{
		--SDL2
		"%{wks.location}/Engine/vendor/SDL2/include"
	}

	libdirs
	{
		--SDL2
		"%{wks.location}/Engine/vendor/SDL2/lib"
	}

	postbuildcommands
	{
		"{COPY} %{wks.location}/Engine/vendor/SDL2/lib/**.dll %{cfg.targetdir}"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
