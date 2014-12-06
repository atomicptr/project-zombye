solution "project-zombye"
    configurations { "debug", "release"}
    language "C++"

    includedirs {
        "deps/bullet3",
        "deps/jsoncpp/include",
        "deps/gli/include",
        "deps/glm/include",
        "src/include"
    }

    configuration "debug"
        flags {"Symbols"}
        optimize "Off"

    configuration "release"
        optimize "Full"

    project "deps"
        kind "StaticLib"

        buildoptions {"-x c++", "-std=c++1y"}

        files {
            "deps/**.cpp",
            "deps/**.c"
        }

        warnings "Off"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

            links "OpenCL"

        configuration {"gmake", "macosx"}
            linkoptions "-framework OpenCL"

    project "mesh_converter"
        kind "ConsoleApp"

        buildoptions "-std=c++1y"

        files "src/source/mesh_converter/**.cpp"

        defines "GLM_FORCE_RADIANS"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end
            links "tinyxml2"

        configuration {"gmake", "macosx"}
            links "tinyxml2"

        configuration "debug"
            flags {"Symbols", "FatalWarnings"}
            optimize "Off"

        configuration "release"
            optimize "Full"

    project "animation_converter"
        kind "ConsoleApp"

        buildoptions "-std=c++1y"

        files "src/source/animation_converter/**.cpp"

        defines "GLM_FORCE_RADIANS"

        links {"assimp"}

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

        configuration "debug"
            flags {"FatalWarnings"}

    project "zombye"
        kind "WindowedApp"

        buildoptions "-std=c++1y"

        files "src/source/zombye/**.cpp"

        defines "GLM_FORCE_RADIANS"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end
            links { "GL", "GLEW", "SDL2", "SDL2_mixer", "deps" }

        configuration {"gmake", "macosx"}
            links { "OpenGL.framework", "GLEW", "SDL2", "SDL2_mixer", "deps" }

        configuration "debug"
            flags {"FatalWarnings"}
            defines "ZOMBYE_DEBUG"
