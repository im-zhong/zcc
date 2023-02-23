import os
import subprocess

for root, _, files in os.walk("."):
    for file in files:
        if file.endswith((".h", ".cpp")):
            filename = os.path.join(root, file)
            print(filename)
            subprocess.run(["iconv", "-f", "GB2312", "-t",
                           "UTF-8", filename, "-o", filename])
