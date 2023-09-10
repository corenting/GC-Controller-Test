#!/usr/bin/env bash
# Simple helper script to build through Docker to avoid installing devkitpro toolchain

set -euo pipefail

# help print function
print_help () {
    echo "You need to specify the command to run: wii, gamecube or clean"
    exit 1
}

# Check only argument is provided
if [ $# -ne 1 ]
  then
    print_help
fi

# Base docker command
COMMON_DOCKER_ARGS=(-v "$PWD":/project -w /project -u "$(id -u "${USER}")":"$(id -g "${USER}")" devkitpro/devkitppc make)

# Choose operation
case $1 in

  wii)
    docker run -e "TARGET_CONSOLE=wii" "${COMMON_DOCKER_ARGS[@]}"
    ;;

  gamecube)
     docker run -e "TARGET_CONSOLE=gamecube" "${COMMON_DOCKER_ARGS[@]}"
    ;;

  clean)
     rm -rf build_wii
     rm -rf build_gamecube
     rm -rf -- *.elf
     rm -rf -- *.dol
    ;;

  *)
    print_help
    ;;
esac
