workspace "AlfPasswordRemover"
    	startproject "AlfPasswordRemover"

	configurations
	{
		"Debug-x64",
		"Release-x64",

		"Debug-x86",
		"Release-x86"
	}

	filter "configurations:*x64"
      		architecture "x86_64"
	
	filter "configurations:*x86"
      		architecture "x86"

outputdir = "%{cfg.buildcfg}-%{cfg.system}"
SystemVersion = "latest"

project "AlfPasswordRemover"
	location "."
	kind "ConsoleApp"
	language "C++"
	characterset("MBCS")

	targetdir("bin/" .. outputdir)
	objdir("bin-int/" .. outputdir)
	
	files
	{
		"src/**.cpp",
		"src/**.h",

		--ZipLib
		"src/ziplib/Source/ZipLib/*.cpp",
		"src/ziplib/Source/ZipLib/detail/*.cpp",

		--ZipLib dependencies
		"src/ziplib/Source/ZipLib/extlibs/zlib/*.c",
		"src/ziplib/Source/ZipLib/extlibs/bzip2/*.c"
	}

	filter "system:windows"
		files "src/ziplib/Source/ZipLib/extlibs/lzma/*.c"
		defines "_CRT_SECURE_NO_WARNINGS"

	filter "system:linux"
		files "src/ziplib/Source/ZipLib/extlibs/lzma/unix/*.c"

	filter "configurations:Debug*"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release*"
		optimize "On"
		runtime "Release"