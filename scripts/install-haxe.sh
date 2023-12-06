#!/bin/bash

# Install haxe inside a linux x64 container
# Repository: https://quay.io/pypa/manylinux2014_x86_64
# Image: 2023-12-05-e9f0345

# We install an older version of haxe for compatibility with glibc 2.17

if [ -f "/.dockerenv" ] || grep -q "docker" /proc/1/cgroup; then
    echo "Not inside a docker container!"
    exit 1
fi


NEKO_URL="https://github.com/HaxeFoundation/neko/releases/download"
HAXE_URL="https://github.com/HaxeFoundation/haxe/releases/download"

NEKO_TAG="v2-3-0"
HAXE_TAG="4.2.5"

NEKO_TARBALL="neko-2.3.0-linux64.tar.gz"
HAXE_TARBALL="haxe-4.2.5-linux64.tar.gz"


mkdir /opt/haxe
cd /opt/haxe

curl -LO $NEKO_URL/$NEKO_TAG/$NEKO_TARBALL
curl -LO $HAXE_URL/$HAXE_TAG/$HAXE_TARBALL

tar zxvf $NEKO_TARBALL
tar zxvf $HAXE_TARBALL

NEKO_DIR=`tar -tzf $NEKO_TARBALL | head -1 | cut -f1 -d"/"`
HAXE_DIR=`tar -tzf $HAXE_TARBALL | head -1 | cut -f1 -d"/"`


export NEKOPATH=/opt/haxe/$NEKO_DIR
export LD_LIBRARY_PATH=$NEKOPATH:$LD_LIBRARY_PATH
export PATH=/opt/haxe/$HAXE_DIR:$PATH


mkdir ~/haxelib
haxelib setup ~/haxelib
haxelib install hxcpp "4.3.2"
