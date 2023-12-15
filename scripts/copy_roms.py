import os
import shutil


def copy_roms(source, destination):
    if not os.path.exists(destination):
        os.makedirs(destination)
    for file in os.listdir(source):
        if file.endswith(".gb"):
            shutil.copy(os.path.join(source, file), destination)


def before_build(source, target, env):
    print("Copying ROMs to the build output folder...")
    rom_source_dir = env.subst("$PROJECT_DIR") + "/roms"
    build_output_dir = env.subst("$BUILD_DIR")
    copy_roms(rom_source_dir, build_output_dir)


env.AddPreAction("buildprog", before_build)
