includes("../cosmovm")

target("cosmoemu")
    set_version("1.0.0")
    set_kind("binary")
    set_configdir(".")
    add_configfiles("cosmovm_config.hpp.in")
    add_files(
        "cosmoasm.cpp",
        "main.cpp")
    add_includedirs(ROOT_DIR .. "include")
    add_deps("cosmocore_static", "cosmocore_shared")
    add_linkdirs(ROOT_DIR .. "build")
    add_links("SDL2", "SDL2_ttf", "cosmovm")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
        os.cp(local_ROOT_DIR .. "disk.bin", local_ROOT_DIR .. "build")
        os.cp(local_ROOT_DIR .. "vgafont.ttf", local_ROOT_DIR .. "build")
    end)
target_end()