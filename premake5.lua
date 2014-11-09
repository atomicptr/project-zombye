solution "project-zombye"
    configurations { "debug", "release"}
    language "C++"

    configuration "windows"
        defines "ZOMBYE_WINDOOF"
    configuration "not windows"
        defines "ZOMBYE_NOT_WINDOOF"

    includedirs { "deps/include", "src/include" }
    buildoptions "-std=c++1y"

    project "deps"
        kind "StaticLib"

        files "deps/source/**.cpp"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

    project "zombye"
        kind "WindowedApp"

        files "src/source/**.cpp"

        defines "GLM_FORCE_RADIANS"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end
            links { "GL", "SDL2", "SDL2_mixer", "deps" }

        configuration {"gmake", "macosx"}
            links { "OpenGL.framework", "SDL2", "SDL2_mixer", "deps" }

        configuration "debug"
            flags {"Symbols", "FatalWarnings"}
            optimize "Off"

        configuration "release"
            optimize "Full"
