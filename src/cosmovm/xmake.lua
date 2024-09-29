target("cosmocore_static")
    set_version("2.0.0")
    set_kind("static")
    set_basename("cosmovm")
    add_files(
        "bus.cpp",
        "clock.cpp",
        "cpu.cpp",
        "disk.cpp",
        "display.cpp",
        "keyboard.cpp",
        "memory.cpp")
    add_includedirs(ROOT_DIR .. "include")
    add_links("SDL2", "SDL2_ttf")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()

target("cosmocore_shared")
    set_version("2.0.0")
    set_kind("shared")
    set_basename("cosmovm")
    -- set_configdir(".")
    -- add_configfiles("cosmocore_config.hpp.in")
    add_files(
        "bus.cpp",
        "clock.cpp",
        "cpu.cpp",
        "disk.cpp",
        "display.cpp",
        "keyboard.cpp",
        "memory.cpp")
    add_includedirs(ROOT_DIR .. "include")
    add_links("SDL2", "SDL2_ttf", "gomp")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()