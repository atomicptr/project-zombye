solution "project-zombye"
    configurations { "debug", "release"}
    language "C++"

    configuration "windows"
        defines "ZOMBYE_WINDOOF"
    configuration "not windows"
        defines "ZOMBYE_NOT_WINDOOF"

    includedirs "src/include"
    buildoptions "-std=c++1y"

    project "zombye"
        kind "WindowedApp"

        files "src/source/**.cpp"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                buildoptions = "-stdlib=libc++"
                links "c++"
            end
            links {"GL","SDL2", "SDL2_mixer"}

        configuration {"gmake", "macosx"}
            links {"OpenGL.framework", "SDL2", "SDL2_mixer"}

        configuration "debug"
            warnings "Extra"
            flags {"Symbols", "FatalWarnings"}

        configuration "release"
            flags {"Optimize"}
