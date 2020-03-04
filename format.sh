#!/usr/bin/env bash
find -name '*.cc' | xargs clang-format -i
find -name '*.hh' | xargs clang-format -i
find -name '*.h' | xargs clang-format -i
find -name '*.cpp' | xargs clang-format -i
