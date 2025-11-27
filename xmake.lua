set_xmakever("2.2.5")

set_project("CosmoVM")
set_version("2.0.0")
set_languages("cxxlatest")

add_requires("sdl3", "sdl3_ttf", {configs = {binaryonly = true}})

set_warnings("everything")
if is_mode("release") then
    add_cxxflags("-Ofast", "-march=native", "-flto=auto", "-DNDEBUG") -- "-D_GLIBCXX_PARALLEL"
elseif is_mode("debug") then
    add_cxxflags("-Og", "-ggdb")
end

if is_plat("window") then
    ROOT_DIR = path.absolute(".") .. "\\"
else
    ROOT_DIR = path.absolute(".") .. "/"
end
includes("src/cosmoemu")