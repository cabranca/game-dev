project "UnitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {"src/**.h", "src/**.cpp"}

    includedirs { "%{wks.location}/Cabrankengine/vendor/spdlog/include", "%{wks.location}/Cabrankengine/src", "%{IncludeDir.Catch2}" }

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
