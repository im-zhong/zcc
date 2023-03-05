from conan.api.conan_api import ConanAPI
from conan.api.output import ConanOutput, Color
from conan.cli.command import OnceArgument, conan_command
from conans.client.userio import UserInput
import os
import re
import subprocess


@conan_command(group="Custom commands")
def ut(conan_api: ConanAPI, parser, *args):
    """
    automatically run unit test
    """

    parser.add_argument('-r', '--regex', default='', action=OnceArgument,
                        help='search unit test witl regex')

    if not os.path.exists("conanfile.py"):
        ConanOutput().error("conanfile.py not found")
        return

    args = parser.parse_args(*args)

    # 查找所有以test_开头的可执行程序 并且执行
    # 还是给出正则表达式吧
    for root, _, files in os.walk("build"):
        for file in files:
            if file.startswith("test_"):
                if args.regex and not re.search(args.regex, file):
                    continue
                filename = os.path.join(root, file)
                if os.access(filename, os.X_OK):
                    print(f"testing... {filename}")
                    # 这里还需要收集返回的信息
                    subprocess.run([filename])
