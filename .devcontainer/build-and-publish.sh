#!/bin/bash

echo "This script will build the devcontainer for linux/arm64 and linux/amd64."
echo
echo "Please make sure to:"
echo "1. have the right permissions to push the image;"
echo "2. have set up the docker multiarch build (see https://docs.docker.com/build/building/multi-platform/)."

script_dir=$(dirname "$0")

docker buildx build --push --platform linux/arm64,linux/amd64 --tag mntmtt/sourcevaultdev "$script_dir"