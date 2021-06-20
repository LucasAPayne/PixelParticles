workspace "PixelParticles"
    architecture "x86"
    startproject "PixelParticles"
    configurations {"Debug", "Release"}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    group "Dependencies"
        include "vendor/premake"
    group ""

    include "PixelParticles"
