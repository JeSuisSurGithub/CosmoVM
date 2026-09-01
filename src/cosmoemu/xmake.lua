includes("../cosmovm")

target("cosmoemu")
    set_version("2.0.0")
    set_kind("binary")
    set_configdir(".")
    --add_configfiles("cosmoemu_config.hpp.in")
    add_files(
        "assembler.cpp",
        "main.cpp")
    add_includedirs(ROOT_DIR .. "include")
    add_deps("cosmocore_static")
    add_linkdirs(ROOT_DIR .. "build")
    add_links("SDL3", "SDL3_ttf", "cosmovm")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
        os.cp(local_ROOT_DIR .. "default.ttf", local_ROOT_DIR .. "build")
    end)
target_end()