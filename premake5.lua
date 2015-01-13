solution "project-zombye"
    configurations { "debug", "release"}
    language "C++"

    includedirs {
        "deps/bullet3",
        "deps/jsoncpp/include",
        "deps/gli/include",
        "deps/glm/include",
        "deps/tinyxml2",
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

        defines "GLM_FORCE_RADIANS"

        warnings "Off"

        configuration {"gmake", "windows"}
            includedirs {
                "$(AMDAPPSDKROOT)/include"
                -- TODO: add NVidia OpenCL include path
                -- TODO: add Intel OpenCL include path
            }
            libdirs {
                "$(AMDAPPSDKROOT)/lib/x64"
                -- TODO: add NVidia OpenCL lib path
                -- TODO: add Intel OpenCL lib path
            }

            linkoptions {"-lmingw32 -lOpenCL"}

            defines {"WINVER=0x0601", "_WIN32_WINNT=0x0601"}

            buildoptions "-fpermissive"

            links "OpenCL"

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

        links "deps"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

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

        links "deps"

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

        configuration {"gmake", "windows"}
            buildoptions "-std=gnu++1y"

            includedirs {
                "deps/mingw/SDL2-2.0.3/x86_64-w64-mingw32/include",
                "deps/mingw/SDL2-2.0.3/x86_64-w64-mingw32/include/SDL2",
                "deps/mingw/SDL2_mixer-2.0.0/x86_64-w64-mingw32/include",
                "deps/mingw/glew/include"
            }

            libdirs {
                "deps/mingw/SDL2-2.0.3/x86_64-w64-mingw32/lib",
                "deps/mingw/SDL2_mixer-2.0.0/x86_64-w64-mingw32/lib",
                "deps/mingw/glew/lib/x64"
            }

            defines {"GLEW_STATIC"}

            linkoptions {"-lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lglew32s -lopengl32"}

            links {"deps"}

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
