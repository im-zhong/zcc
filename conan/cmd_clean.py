from conan.api.conan_api import ConanAPI
from conan.api.output import ConanOutput, Color
from conan.cli.command import OnceArgument, conan_command
from conans.client.userio import UserInput
import os
import shutil


@conan_command(group="Custom commands")
def clean(conan_api: ConanAPI, parser, *args):
    """
    Use clang-format to fmt current directory
    """
    # 真正的clean 把build system一起删了就行了
    # 不行玩意用错了就尴尬了 所以还是先检测是不是在一个conan文件夹下
    if not os.path.exists("conanfile.py"):
        ConanOutput().error("conanfile.py not found")
        return

    shutil.rmtree("build")
