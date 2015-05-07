solution "project-zombye"
    configurations {"release"}
    language "C++"

    includedirs {
        "include/"
    }

    configuration "release"
        optimize "Full"

    project "jsoncpp"
        kind "StaticLib"

        buildoptions {"-x c++", "-std=c++1y"}

        files {
            "src/**.cpp"
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
