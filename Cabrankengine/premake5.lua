project "Cabrankengine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    

    files
    {
        "src/Cabrankengine/**.h",
		"src/Cabrankengine/**.cpp",
        "src/Cabrankengine/Cabrankengine.h",
        "src/Cabrankengine/pch*",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
    }
    
    externalincludedirs
    {
        "src",
        "vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.glad}", "%{IncludeDir.ImGui}", "%{IncludeDir.stb_image}", 
        "%{IncludeDir.irrKlang}", "%{IncludeDir.json}", "%{IncludeDir.assimp}", "%{IncludeDir.FreeType}"
    }

    links {"GLFW", "ImGui", "IrrKlang", "Assimp", "FreeType"}

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

        pchheader "pch.h"
	    pchsource "src/pch.cpp"

        files 
        { 
            "src/Platform/Windows/**.h",
            "src/Platform/Windows/**.cpp",
            "src/Platform/OpenGL/**.h",
            "src/Platform/OpenGL/**.cpp",
        }

        removefiles { "src/Platform/Linux/**.cpp", "src/Platform/Linux/**.h" }
        libdirs { "vendor/irrKlang/lib" }
        links {"opengl32.lib", "glad" }
        defines {"GLFW_INCLUDE_NONE"}

    filter "system:linux"
        systemversion "latest"
        pic "on"

        pchheader "pch.h"
	    pchsource "src/pch.cpp"

        files 
        { 
            "src/Platform/Linux/**.h",
            "src/Platform/Linux/**.cpp",
            "src/Platform/OpenGL/**.h",
            "src/Platform/OpenGL/**.cpp",
        }

        removefiles { "src/Platform/Windows/**.cpp", "src/Platform/Windows/**.h" }

        libdirs {  "%{wks.location}/Cabrankengine/vendor/irrKlang/so" }
        links { "X11", "Xrandr", "Xi", "Xxf86vm", "Xcursor", "pthread", "dl", "GL", "glad" }

    filter "system:macosx"
        systemversion "latest"
        pic "On"
        
        pchheader "src/pch.h"
	    pchsource "src/pch.cpp"

        defines { "IMGUI_IMPL_METAL_CPP" }

        links { "Cocoa.framework", "Foundation.framework", "Metal.framework", "QuartzCore.framework" }

        files 
        { 
            "vendor/imgui/backends/imgui_impl_metal.mm",
            "src/Platform/MacOS/**.h",
            "src/Platform/MacOS/**.cpp",
            "src/Platform/Metal/**.h",
            "src/Platform/Metal/**.cpp",
        }

        externalincludedirs { "%{IncludeDir.Metal}" }

    filter "configurations:Debug"
        defines "CE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CE_RELEASE"
        runtime "Release"
        optimize "on"
