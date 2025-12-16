workspace "Cabrankengine"
    architecture "x64"
    startproject "Sandbox"

    configurations {"Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["assimp"] = "Cabrankengine/vendor/assimp/include"
IncludeDir["FreeType"] = "Cabrankengine/vendor/freetype/include"
IncludeDir["GLFW"] = "Cabrankengine/vendor/GLFW/include"
IncludeDir["glad"] = "Cabrankengine/vendor/glad/include"
IncludeDir["ImGui"] = "Cabrankengine/vendor/imgui"
IncludeDir["glm"] = "Cabrankengine/vendor/glm"
IncludeDir["stb_image"] = "Cabrankengine/vendor/stb_image"
IncludeDir["irrKlang"] = "Cabrankengine/vendor/irrKlang/include"
IncludeDir["json"] = "Cabrankengine/vendor/json/include"
IncludeDir["Catch2"] = "Cabrankengine/vendor/Catch2"

include "Cabrankengine/vendor/assimp"
include "Cabrankengine/vendor/freetype"
include "Cabrankengine/vendor/GLFW"
include "Cabrankengine/vendor/glad"
include "Cabrankengine/vendor/imgui"

project "Cabrankengine"
    location "Cabrankengine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
	pchsource "src/pch.cpp"

    files 
    {
        "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.hpp", "%{prj.name}/vendor/stb_image/**.cpp"
    }
    includedirs 
    {
        "%{prj.name}/src", "%{prj.name}/vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.glad}", "%{IncludeDir.ImGui}", 
        "%{IncludeDir.stb_image}", "%{IncludeDir.irrKlang}", "%{IncludeDir.json}", "%{IncludeDir.assimp}", "%{IncludeDir.FreeType}"
    }
    links {"GLFW", "glad", "ImGui", "IrrKlang", "Assimp", "FreeType"}

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

        removefiles { "%{prj.name}/src/Platform/Linux/**.cpp", "%{prj.name}/src/Platform/Linux/**.h" }
        libdirs { "%{prj.name}/vendor/irrKlang/lib" }
        links {"opengl32.lib"}
        defines {"GLFW_INCLUDE_NONE"}

    filter "system:linux"
        systemversion "latest"
        pic "on"

        removefiles { "%{prj.name}/src/Platform/Windows/**.cpp", "%{prj.name}/src/Platform/Windows/**.h" }
        
        libdirs {  "%{wks.location}/Cabrankengine/vendor/irrKlang/so" }
        links { "X11", "Xrandr", "Xi", "Xxf86vm", "Xcursor", "pthread", "dl", "GL" }
    
    filter "configurations:Debug"
        defines "CE_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "CE_RELEASE"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"}
    includedirs 
    {
        "Cabrankengine/vendor/spdlog/include", "Cabrankengine/src", "%{IncludeDir.ImGui}", "%{IncludeDir.irrKlang}", 
        "%{IncludeDir.freetype}", "%{IncludeDir.assimp}",
    }
    links {"Cabrankengine", "Assimp", "FreeType", "GLFW", "glad", "ImGui"}

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

        postbuildcommands 
        {
            'xcopy /Y /Q /E /I "%{wks.location}\\Cabrankengine\\vendor\\irrKlang\\dll\\*" "%{cfg.targetdir}"',
            'xcopy /Y /Q /E /I "%{prj.location}\\assets" "%{cfg.targetdir}\\assets"',
            'if exist "%{prj.location}\\config.json" copy /Y "%{prj.location}\\config.json" "%{cfg.targetdir}\\config.json"'
        }

    filter "system:linux"
        systemversion "latest"
        pic "on"

        libdirs { "%{wks.location}/Cabrankengine/vendor/irrKlang/so" }
        links { "IrrKlang" }
        links { "X11", "Xrandr", "Xi", "Xxf86vm", "Xcursor", "pthread", "dl", "GL", "z" }
        linkoptions { "-Wl,-rpath,'$$ORIGIN'" }
        postbuildcommands 
        {
            'cp -ru %{wks.location}/Cabrankengine/vendor/irrKlang/so/* %{cfg.targetdir}/',
            'cp -ru %{prj.location}/assets/ %{cfg.targetdir}/',
            'cp -u %{prj.location}/config.json %{cfg.targetdir}/config.json 2>/dev/null || true'
        }
        
    filter "configurations:Debug"
        defines "CE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CE_RELEASE"
        runtime "Release"
        optimize "on"

project "UnitTests"
    location "UnitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files { "UnitTests/src/**.cpp", "UnitTests/src/**.h" }

    includedirs {
        "Cabrankengine/src",
        "%{IncludeDir.glm}",
        "Cabrankengine/vendor/spdlog/include",
        "%{IncludeDir.Catch2}"
    }

    links { "Cabrankengine" }

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

    filter "system:linux"
        systemversion "latest"
        pic "on"
        links { "pthread" }

    filter "configurations:Debug"
        defines "CE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CE_RELEASE"
        runtime "Release"
        optimize "on"

