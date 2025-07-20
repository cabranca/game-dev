workspace "Cabrankengine"
    architecture "x64"

    configurations {"Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Cabrankengine/vendor/GLFW/include"

include "Cabrankengine/vendor/GLFW"

project "Cabrankengine"
    location "Cabrankengine"
    kind "SharedLib"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"}
    includedirs {"%{prj.name}/src", "%{prj.name}/vendor/spdlog/include", "%{IncludeDir.GLFW}"}
    links {"GLFW", "opengl32.lib"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/utf-8" }

        defines {"CE_PLATFORM_WINDOWS", "CE_BUILD_DLL"}
        postbuildcommands {("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")}
    
    filter "configurations:Debug"
        defines "CE_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "CE_RELEASE"
        symbols "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"}
    includedirs {"Cabrankengine/vendor/spdlog/include", "Cabrankengine/src"}
    links {"Cabrankengine"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/utf-8" }

        defines {"CE_PLATFORM_WINDOWS"}
        
    filter "configurations:Debug"
        defines "CE_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "CE_RELEASE"
        symbols "On"