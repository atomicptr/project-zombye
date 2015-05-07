solution "project-zombye"
    configurations {"release"}
    language "C++"

    includedirs {
        "include",
        "../deps/glm/include",
        "../deps/jsoncpp/include"
    }

    libdirs {
        "../deps/jsoncpp"
    }

    configuration "release"
        optimize "Full"

    project "mesh_converter"
        kind "ConsoleApp"

        buildoptions "-std=c++1y"

        files "src/**.cpp"

        defines "GLM_FORCE_RADIANS"

        links "jsoncpp"

        configuration {"gmake", "linux"}
            if _OPTIONS["cc"] == "clang" then
                toolset "clang"
                buildoptions "-stdlib=libc++"
                links "c++"
            end
