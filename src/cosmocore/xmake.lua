target("cosmocore_static")
    set_version("1.0.0")
    set_kind("static")
    set_basename("cosmocore")
    set_configdir(".")
    add_configfiles("cosmocore_config.hpp.in")
    add_files(
        "cosmobus.cpp",
        "cosmoclk.cpp",
        "cosmocpu.cpp",
        "cosmodsk.cpp",
        "cosmokb.cpp",
        "cosmomem.cpp",
        "cosmoscr.cpp")
    add_includedirs(ROOT_DIR .. "include")
    add_links("SDL2", "SDL2_ttf")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()

target("cosmocore_shared")
    set_version("1.0.0")
    set_kind("shared")
    set_basename("cosmocore")
    set_configdir(".")
    add_configfiles("cosmocore_config.hpp.in")
    add_files(
        "cosmobus.cpp",
        "cosmoclk.cpp",
        "cosmocpu.cpp",
        "cosmodsk.cpp",
        "cosmokb.cpp",
        "cosmomem.cpp",
        "cosmoscr.cpp")
    add_includedirs(ROOT_DIR .. "include")
    add_links("SDL2", "SDL2_ttf")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()