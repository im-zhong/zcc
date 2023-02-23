from conan.api.conan_api import ConanAPI
from conan.api.output import ConanOutput, Color
from conan.cli.command import OnceArgument, conan_command
from conans.client.userio import UserInput
import os
import subprocess


@conan_command(group="Custom commands")
def fmt(conan_api: ConanAPI, parser, *args):
    """
    Use clang-format to fmt current directory
    """

    if not os.path.exists("conanfile.py"):
        ConanOutput().error("conanfile.py not found")
        return

    if not os.path.exists(".clang-format"):
        ConanOutput().error(".clang-format not found")
        return

    # 执行clang-format命令
    # 这还不如让python遍历一下当前目录呢
    # 这个牛逼了 os.walk会帮你递归的遍历文件 方便
    for root, _, files in os.walk("tool"):
        for file in files:
            if file.endswith((".h", ".hpp", ".c", ".cxx", ".cpp")):
                filename = os.path.join(root, file)
                print(filename)
                subprocess.run(["clang-format", "-i", filename])

    for root, _, files in os.walk("util"):
        for file in files:
            if file.endswith((".h", ".hpp", ".c", ".cxx", ".cpp")):
                filename = os.path.join(root, file)
                print(filename)
                subprocess.run(["clang-format", "-i", filename])

    for root, _, files in os.walk("zcc"):
        for file in files:
            if file.endswith((".h", ".hpp", ".c", ".cxx", ".cpp")):
                filename = os.path.join(root, file)
                print(filename)
                subprocess.run(["clang-format", "-i", filename])
