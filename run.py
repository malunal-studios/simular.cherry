#!/usr/bin/python3
import argparse
import os
import subprocess

parser = argparse.ArgumentParser(description = "Build script for Cherry")
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

subparsers = parser.add_subparsers(dest="command", help="Available commands")
build_parser = subparsers.add_parser("build", help="Configures and builds Cherry")
build_parser.add_argument(
    "--no-tests",
    default=True,
    action="store_false",
    dest="enable_tests",
    required=False,
    help="Tells CMake to disable the compilation of the tests for the build."
)

build_parser.add_argument(
    "--no-benchmarks",
    default=True,
    action="store_false",
    dest="enable_benchmarks",
    required=False,
    help="Tells CMake to disable the compilation of the benchmarks for the build."
)

build_parser.add_argument(
    "--profile",
    default=False,
    action="store_true",
    dest="profile",
    required=False,
    help="Configures the build to enable the use of the performance tooling library."
)

install_parser = subparsers.add_parser("install", help="Installs Cherry for the system")


def execute_command(args: [str]):
    with subprocess.Popen(
        " ".join(args),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True,
        env=os.environ,
        text=True
    ) as process:
        for line in process.stdout:
            print(line.rstrip("\n"))


def configure_and_build():
    arguments: [str] = []
    if clargs.configure == True:
        build_type: str = "Debug"
        cmake_options: [str] = [
            "cmake", "-E", "env", "CLICOLOR_FORCE=1",
            "cmake", "-G", "\"Unix Makefiles\""
        ];
        
        if clargs.release == True:
            build_type = "Release"

        cmake_options.append(f"-DCMAKE_BUILD_TYPE={build_type}")
        if clargs.profile == True:
            cmake_options.append("-DCHERRY_ENABLE_PROFILING=ON")

        if clargs.enable_tests == True:
            cmake_options.append("-DCHERRY_BUILD_TESTS=ON")

        if clargs.enable_benchmarks == True:
            cmake_options.append("-DCHERRY_BUILD_BENCHMARKS=ON")

        arguments.extend(cmake_options)
        arguments.extend(["-S", ".", "-B", "build"])
        execute_command(arguments)
        arguments.clear()

    arguments.extend([
        "cmake", "-E", "env", "CLICOLOR_FORCE=1",
        "cmake", "--build", "build", "--", f"-j{clargs.threads}"
    ])
    execute_command(arguments)


def build_and_install():
    print("Not yet supported!")


clargs = parser.parse_args()
if clargs.command == "build":
    configure_and_build()
elif clargs.command == "install":
    build_and_install()
