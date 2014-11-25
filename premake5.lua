solution "project-zombye"
    configurations { "debug", "release"}
    language "C++"

    includedirs { "deps/include", "src/include", "deps/bullet3" }

    configuration "debug"
        flags {"Symbols"}
        optimize "Off"

    configuration "release"
        optimize "Full"

    project "deps"
        kind "StaticLib"

        buildoptions "-std=c++1y"

        files "deps/source/**.cpp"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

    project "bullet3"
        kind "StaticLib"

        files {"deps/bullet3/**.cpp", "deps/bullet3/**.c"}

        configuration {"gmake", "linux"}
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
            links { "GL", "GLEW", "SDL2", "SDL2_mixer", "deps", "bullet3" }

        configuration {"gmake", "macosx"}
            links { "OpenGL.framework", "GLEW", "SDL2", "SDL2_mixer", "deps", "bullet3" }

        configuration "debug"
            flags {"FatalWarnings"}
