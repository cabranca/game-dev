workspace "Cabrankengine"
    architecture "x64"

    configurations {"Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Cabrankengine/vendor/GLFW/include"
IncludeDir["glad"] = "Cabrankengine/vendor/glad/include"
IncludeDir["ImGui"] = "Cabrankengine/vendor/imgui"
IncludeDir["glm"] = "Cabrankengine/vendor/glm"

include "Cabrankengine/vendor/GLFW"
include "Cabrankengine/vendor/glad"
include "Cabrankengine/vendor/imgui"

project "Cabrankengine"
    location "Cabrankengine"
    kind "SharedLib"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/vendor/glm/glm/**.hpp", "%{prj.name}/vendor/glm/glm/**.inl"}
    includedirs {"%{prj.name}/src", "%{prj.name}/vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.glad}", "%{IncludeDir.ImGui}", "%{IncludeDir.glm}"}
    links {"GLFW", "glad", "ImGui"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/utf-8" }

        removefiles { "%{prj.name}/src/Platform/Linux/**.cpp", "%{prj.name}/src/Platform/Linux/**.h" }
        links {"opengl32.lib"}

        defines {"CE_PLATFORM_WINDOWS", "CE_BUILD_DLL", "GLFW_INCLUDE_NONE"}
        postbuildcommands {("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")}

    filter "system:linux"
        cppdialect "C++20"
        staticruntime "Off"
        systemversion "latest"
        pic "On"

        removefiles { "%{prj.name}/src/Platform/Windows/**.cpp", "%{prj.name}/src/Platform/Windows/**.h" }
        links { "X11", "Xrandr", "Xi", "Xxf86vm", "Xcursor", "pthread", "dl", "GL" }
        defines {"CE_PLATFORM_LINUX", "CE_BUILD_DLL"}
        postbuildcommands {"cp %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"}
    
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
    includedirs {"Cabrankengine/vendor/spdlog/include", "Cabrankengine/src", "%{IncludeDir.glm}"}
    links {"Cabrankengine"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/utf-8" }

        defines {"CE_PLATFORM_WINDOWS"}

    filter "system:linux"
        cppdialect "C++20"
        staticruntime "Off"
        systemversion "latest"
        pic "On"

        defines {"CE_PLATFORM_LINUX"}
        links { "pthread", "dl" }
        
    filter "configurations:Debug"
        defines "CE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CE_RELEASE"
        symbols "On"