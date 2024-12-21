#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GMSJ01",  # 0
    "GMSP01",  # 1
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v1.3.0"
config.objdiff_tag = "v2.4.0"
config.sjiswrap_tag = "v1.2.0"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
    f"--defsym VERSION_{config.version}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = 61  # Super Mario Sunshine

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-str reuse,pool,readonly",
    "-inline deferred,auto",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

cflags_jsystem = [
    *cflags_base,
    "-str reuse,readonly",
]

cflags_game = [
    *cflags_base,
    "-str reuse,readonly",
]

cflags_system = [
    *cflags_game,
    "-opt all,nostrength",
]

config.linker_version = "GC/1.2.5"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_base,
        "progress_category": "sdk",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "main",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "objects": [
            Object(Matching, "main.cpp"),
        ],
    },
    {
        "lib": "JSystem",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_jsystem,
        "objects": [
            Object(Matching, "JSystem/J3DClusterLoader.cpp"),
            Object(Matching, "JSystem/JASProbe.cpp"),
            Object(Matching, "JSystem/JDRPlacement.cpp"),
            Object(Matching, "JSystem/JDRResolution.cpp"),
            Object(Matching, "JSystem/JSUList.cpp"),
            Object(Matching, "JSystem/JUTRect.cpp"),
            # Object(Matching, "JSystem/J3DModel.cpp"),
            Object(Matching, "JSystem/J3DVertex.cpp"),
            Object(Matching, "JSystem/J3DPacket.cpp"),
            # Object(Matching, "JSystem/J3DShape.cpp"),
        ],
    },
    {
        "lib": "MarioUtil",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "objects": [
            Object(Matching, "MarioUtil/MapUtil.cpp"),
            Object(Matching, "MarioUtil/RumbleType.cpp"),
            Object(Matching, "MarioUtil/EffectUtil.cpp"),
            Object(Matching, "MarioUtil/ModelUtil.cpp"),
            Object(Matching, "MarioUtil/TexUtil.cpp"),
            # Object(Matching, "MarioUtil/DrawUtil.cpp"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_runtime,
        "objects": [
            Object(NonMatching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
            Object(NonMatching, "Runtime.PPCEABI.H/Gecko_ExceptionPPC.cp"),
            Object(NonMatching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
        ],
    },
    {
        "lib": "MSL_C.PPCEABI.bare.H",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_runtime,
        "objects": [
            Object(Matching, "MSL_C.PPCEABI.bare.H/hyperbolicsf.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/rand.c"),
        ],
    },
    {
        "lib": "NPC",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "objects": [
            Object(Matching, "NPC/NpcBalloon.cpp"),
            Object(Matching, "NPC/NpcInitActionData.cpp"),
        ],
    },
    {
        "lib": "os",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_base,
        "objects": [
            Object(Matching, "os/__ppc_eabi_init.cpp"),
            Object(Matching, "os/__start.c"),
        ],
    },
    {
        "lib": "Player",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_game,
        "objects": [
            Object(Matching, "Player/MarioAccess.cpp"),
        ],
    },
    {
        "lib": "System",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_system,
        "objects": [
            Object(
                Matching,
                "System/FlagManager.cpp",
                cflags=[*cflags_system, "-inline all,level=1,deferred"],
            ),
            Object(Matching, "System/ParamInst.cpp"),
            Object(Matching, "System/ProcessMeter.cpp"),
            Object(Matching, "System/Resolution.cpp"),
            Object(Matching, "System/StageUtil.cpp"),
            Object(Matching, "System/TexCache.cpp"),
        ],
    },
]

# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "SDK Code"),
]
config.progress_each_module = args.verbose

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
