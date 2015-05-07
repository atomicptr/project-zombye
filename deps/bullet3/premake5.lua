solution "project-zombye"
    configurations {"release"}
    language "C++"

    includedirs {
        "src/"
    }

    configuration "release"
        optimize "Full"

    project "bullet3"
        kind "StaticLib"

        buildoptions {"-x c++", "-std=c++1y"}

        files {
            "src/**.cpp",
            "src/**.c"
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
