set_xmakever("2.2.5")

set_project("CosmoPC")
set_version("1.0.0")
set_languages("cxx17")

add_requires("sdl", "sdl2_ttf", {configs = {binaryonly = true}})

set_warnings("everything")
if is_mode("release") then
    add_cxxflags("-O3", "-DNDEBUG")
elseif is_mode("debug") then
    add_cxxflags("-Og", "-ggdb")
end

if is_plat("window") then
    ROOT_DIR = path.absolute(".") .. "\\"
else
    ROOT_DIR = path.absolute(".") .. "/"
includes("src/cosmopc")