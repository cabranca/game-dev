project "Cabrankengine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
	pchsource "src/pch.cpp"

    files
    {
        "src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
    }

    includedirs
    {
        "src", "%{IncludeDir.assimp}", "%{IncludeDir.FreeType}", "%{IncludeDir.glad}", "%{IncludeDir.GLFW}", "%{IncludeDir.ImGui}",
        "%{IncludeDir.irrKlang}", "%{IncludeDir.json}", "%{IncludeDir.Metal}", "%{IncludeDir.spdlog}", "%{IncludeDir.stb_image}", 
    }

    links {"GLFW", "glad", "ImGui", "IrrKlang", "Assimp", "FreeType"}

    defines {"NS_PRIVATE_IMPLEMENTATION", "MTL_PRIVATE_IMPLEMENTATION", "CA_PRIVATE_IMPLEMENTATION"}

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

        removefiles { "src/Platform/Linux/**.cpp", "src/Platform/Linux/**.h" }
        libdirs { "vendor/irrKlang/lib" }
        links {"opengl32.lib"}
        defines {"GLFW_INCLUDE_NONE"}

    filter "system:linux"
        systemversion "latest"
        pic "on"

        removefiles { "src/Platform/Windows/**.cpp", "src/Platform/Windows/**.h" }

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
