solution "project-zombye"
    configurations { "debug", "release"}
    language "C++"

    includedirs {
        "deps/bullet3/src",
        "deps/jsoncpp/include",
        "deps/gli/include",
        "deps/glm/include",
        "deps/angelscript/include",
        "deps/angelscript/source",
        "deps/angelscript/include/scriptbuilder",
        "deps/angelscript/include/scriptstdstring",
        "deps/angelscript/include/scriptarray",
        "src/include",
        "mesh_converter/include",
        "animation_converter/include"
    }

    libdirs {
        "deps/jsoncpp",
        "deps/bullet3",
        "deps/angelscript"
    }

    configuration "debug"
        flags {"Symbols"}
        optimize "Off"

    configuration "release"
        optimize "Full"

    configuration "release"
        optimize "Full"

    project "animation_converter"
        kind "ConsoleApp"

        location "animation_converter/"

        buildoptions "-std=c++1y"

        files "animation_converter/src/**.cpp"

        defines "GLM_FORCE_RADIANS"

        links "jsoncpp"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

    project "mesh_converter"
        kind "ConsoleApp"

        location "mesh_converter"

        buildoptions "-std=c++1y"

        files "mesh_converter/src/**.cpp"

        defines "GLM_FORCE_RADIANS"

        links "jsoncpp"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

    project "bullet3"
        kind "StaticLib"

        location "deps/bullet3"

        buildoptions {"-x c++", "-std=c++1y"}

        files {
            "deps/bullet3/src/**.cpp",
            "deps/bullet3/src/**.c"
        }

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

    project "jsoncpp"
        kind "StaticLib"

        location "deps/jsoncpp"

        buildoptions {"-x c++", "-std=c++1y"}

        files {
            "deps/jsoncpp/src/**.cpp"
        }

        warnings "Off"

        configuration {"gmake", "windows"}
            linkoptions {"-lmingw32"}

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end

        configuration {"gmake", "macosx"}

    project "angelscript"
        kind "StaticLib"

        location "deps/angelscript"

        buildoptions {"-x c++", "-std=c++1y", "-fno-strict-aliasing"}

        files {
            "deps/angelscript/source/**.cpp"
        }

        configuration {"gmake", "windows"}
            linkoptions {"-lmingw32", "-lpthread"}

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end
            links "pthread"

        configuration {"gmake", "macosx"}
            links "pthread"

    project "zombye"
        kind "WindowedApp"

        buildoptions "-std=c++1y"

        files "src/source/zombye/**.cpp"

        defines {"GLM_FORCE_RADIANS", "AS_CAN_USE_CPP11"}

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

            linkoptions {"-lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lglew32s -lopengl32 -lpthread"}

            links {"jsoncpp", "bullet3", "angelscript"}

            postbuildcommands {
                "cp deps/mingw/SDL2-2.0.3/x86_64-w64-mingw32/bin/SDL2.dll SDL2.dll",
                "cp deps/mingw/SDL2_mixer-2.0.0/x86_64-w64-mingw32/bin/*.dll ."
            }

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end
            links { "GL", "GLEW", "SDL2", "SDL2_mixer", "jsoncpp", "bullet3", "angelscript", "pthread" }

        configuration {"gmake", "macosx"}
            links { "OpenGL.framework", "GLEW", "SDL2", "SDL2_mixer", "jsoncpp", "bullet3", "angelscript", "pthread" }

        configuration "debug"
            flags {"FatalWarnings"}
            defines "ZOMBYE_DEBUG"
