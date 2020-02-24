#!/usr/bin/env bash
find -name '*.h' | xargs clang-format -i
find -name '*.cpp' | xargs clang-format -i
