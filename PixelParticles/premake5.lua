project "PixelParticles"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "src/**.h", 
        "src/**.cpp"
    }

    includedirs {"src", "vendor/SFML/include"}

    libdirs {"vendor/SFML/lib"}

    links
    {
        "opengl32.lib", 
        "freetype.lib",
        "winmm.lib",
        "gdi32.lib",
        "openal32.lib",
        "flac.lib",
        "vorbisenc.lib",
        "vorbisfile.lib",
        "vorbis.lib",
        "ogg.lib",
        "ws2_32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        defines {"GAME_PLATFORM_WINDOWS", "SFML_STATIC"}

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        defines "GAME_DEBUG"
        links 
        {
            "sfml-audio-s-d.lib", 
            "sfml-graphics-s-d.lib", 
            "sfml-network-s-d.lib", 
            "sfml-system-s-d.lib", 
            "sfml-window-s-d.lib"
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        defines "GAME_PUBLIC"
        links
        {
            "sfml-audio-s.lib",
            "sfml-graphics-s.lib",
            "sfml-network-s.lib",
            "sfml-system-s.lib",
            "sfml-window-s.lib"
        }
        