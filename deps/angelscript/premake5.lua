solution "project-zombye"
    configurations {"release"}
    language "C++"

    includedirs {
        "source/",
		"include/"
    }

    configuration "release"
        optimize "Full"

    project "angelscript"
        kind "StaticLib"

        buildoptions {"-x c++", "-std=c++1y", "-fno-strict-aliasing"}

        files {
            "source/**.cpp"
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
