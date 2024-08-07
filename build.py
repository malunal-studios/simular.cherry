#!/usr/bin/python3
import argparse
import os
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument(
    "-c",
    "--configure",
    default=False,
    action="store_true",
    dest="configure",
    required=False,
    help="Tells CMake to configure the project before building it."
)

parser.add_argument(
    "-r",
    "--release",
    default=False,
    action="store_true",
    dest="release",
    required=False,
    help="Tells CMake to build in Release mode instead of Debug mode."
)

parser.add_argument(
    "-i",
    "--install",
    default=False,
    action="store_true",
    dest="install",
    required=False,
    help="Tells CMake to install the project to the user's system."
)

parser.add_argument(
    "-t",
    "--threads",
    default=1,
    action="store",
    dest="threads",
    metavar="COUNT",
    nargs='?',
    required=False,
    help="Tells CMake to use the specified number of threads for the build."
)

parser.add_argument(
    "--profile",
    default=False,
    action="store_true",
    dest="profile",
    required=False,
    help="Configures the build to enable the use of the performance tooling library."
)

clargs = parser.parse_args()

arguments: [str] = []
if (clargs.configure == True):
    build_type: str = "Debug"
    cmake_options: [str] = [
        "cmake", "-E", "env", "CLICOLOR_FORCE=1",
        "cmake", "-G", "\"Unix Makefiles\""
    ];
    
    if (clargs.release == True):
        build_type = "Release"

    cmake_options.append(f"-DCMAKE_BUILD_TYPE={build_type}")
    if (clargs.profile == True):
        cmake_options.append(f"-DCHERRY_ENABLE_PROFILING=ON")

    arguments.extend(cmake_options)
    arguments.extend(["-S", ".", "-B", "build"])
    with subprocess.Popen(
        " ".join(arguments),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True,
        env=os.environ,
        text=True
    ) as process:
        for line in process.stdout:
            print(line.rstrip("\n"))
    arguments.clear()


arguments.extend([
    "cmake", "-E", "env", "CLICOLOR_FORCE=1",
    "cmake", "--build", "build", "--", f"-j{clargs.threads}"
])
with subprocess.Popen(
    " ".join(arguments),
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    shell=True,
    env=os.environ,
    text=True
) as process:
    for line in process.stdout:
        print(line.rstrip("\n"))
