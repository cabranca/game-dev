project "Common"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "vendor/lz4/lz4.c",
        "src/Common/**.h",
        "src/Common/**.cpp",
    }
    externalincludedirs { "src", "vendor/spdlog/include", "%{IncludeDir.lz4}" }

    filter "system:linux"
        systemversion "latest"
        pic "on"

    filter "configurations:Debug"
        defines "CBK_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CBK_RELEASE"
        runtime "Release"
        optimize "on"