project "CBKAssetConverter"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.cpp", "vendor/stb_image/stb_image.cpp", }
    externalincludedirs { "%{IncludeDir.assimp}", "%{IncludeDir.lz4}", "%{IncludeDir.stb_image}" }
    links { "Assimp", "Common" }

    filter "system:linux"
        systemversion "latest"
        pic "on"
        links { "z" }

    filter "configurations:Debug"
        defines "CE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CE_RELEASE"
        runtime "Release"
        optimize "on"
