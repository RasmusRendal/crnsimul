#!/usr/bin/env bash
set -e
rm -rf /target/htmloutput || true
mkdir /target/htmloutput
cd /target/htmloutput
gcovr -e /chem/bin/ -r /chem/ --html --html-details -o coverage.html
gcovr -e /chem/bin/ -r /chem/ --html -o coverage_short.html
