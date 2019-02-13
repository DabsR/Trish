workspace "Trish"
	architecture "x86"
	startproject "Trish"
    configurations
	{
		"Debug",
		"Release"
	}

project "Trish"
	location "Trish"
    kind "ConsoleApp"
    language "C"
	staticruntime "off"


    targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
    
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}
    
	filter "system:windows"
		cdialect "C11"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

    filter "configurations:Release"
        optimize "On"