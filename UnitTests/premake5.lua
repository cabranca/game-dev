project "UnitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {"src/**.h", "src/**.cpp"}

    includedirs { "%{IncludeDir.spdlog}", "%{IncludeDir.Common}", "%{wks.location}/Cabrankengine/src", "%{IncludeDir.Catch2}" }

    links { "Cabrankengine", "Common" }

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

    filter "system:linux"
        systemversion "latest"
        pic "on"
        links { "pthread" }

    filter "configurations:Debug"
        defines "CBK_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CBK_RELEASE"
        runtime "Release"
        optimize "on"
