#!/usr/bin/env bash

if [ $# -lt 1 ] || [ $# -gt 1 ]; then
    echo "Please provide one of the following commands:"
    echo "  build"
    echo "  clean"
    exit 1;
fi

if [ $1 = "build" ]; then
    set -e

    mkdir -p blogs_html
    for file in ./blogs/*
    do
        output="./blogs_html/$(basename "$file" .typ).html"
        (set -x
            typst compile --root . --features html -f html "$file" "$output")
    done

    (set -x
        gcc -o build build.c
        ./build > index.gen.c
        gcc -o index.gen index.gen.c
        ./index.gen > index.html
    )

elif  [ $1 = "clean" ]; then
    set -xe
    rm -rf build index.gen.c index.gen index.html
    rm -rf blogs_html
else
    echo "unknown command " $#
    exit 1
fi
