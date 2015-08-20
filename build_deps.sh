# create makefiles for deps and tools
premake5 --file=deps_and_tools.lua gmake
make -j 4

# recreate original makefiles
premake5 gmake
