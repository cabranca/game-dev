workspace "Cabrankengine"
    architecture "x64"
    startproject "Sandbox"

    configurations {"Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Cabrankengine/vendor/GLFW/include"
IncludeDir["glad"] = "Cabrankengine/vendor/glad/include"
IncludeDir["ImGui"] = "Cabrankengine/vendor/imgui"
IncludeDir["glm"] = "Cabrankengine/vendor/glm"
IncludeDir["stb_image"] = "Cabrankengine/vendor/stb_image"

include "Cabrankengine/vendor/GLFW"
include "Cabrankengine/vendor/glad"
include "Cabrankengine/vendor/imgui"

project "Cabrankengine"
    location "Cabrankengine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/vendor/glm/glm/**.hpp", "%{prj.name}/vendor/glm/glm/**.inl", "%{prj.name}/vendor/stb_image/**.cpp", "%{prj.name}/vendor/stb_image/**.cpp"}
    includedirs {"%{prj.name}/src", "%{prj.name}/vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.glad}", "%{IncludeDir.ImGui}", "%{IncludeDir.glm}", "%{IncludeDir.stb_image}"}
    links {"GLFW", "glad", "ImGui"}

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

        removefiles { "%{prj.name}/src/Platform/Linux/**.cpp", "%{prj.name}/src/Platform/Linux/**.h" }
        links {"opengl32.lib"}
        defines {"GLFW_INCLUDE_NONE"}

    filter "system:linux"
        systemversion "latest"
        pic "on"

        removefiles { "%{prj.name}/src/Platform/Windows/**.cpp", "%{prj.name}/src/Platform/Windows/**.h" }
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
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"}
    includedirs {"Cabrankengine/vendor/spdlog/include", "Cabrankengine/src", "%{IncludeDir.glm}", "%{IncludeDir.ImGui}"}
    links {"Cabrankengine", "GLFW", "glad", "ImGui"}

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

    filter "system:linux"
        systemversion "latest"
        pic "on"
        
        links { "X11", "Xrandr", "Xi", "Xxf86vm", "Xcursor", "pthread", "dl", "GL" }
        
    filter "configurations:Debug"
        defines "CE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CE_RELEASE"
        runtime "Release"
        optimize "on"
