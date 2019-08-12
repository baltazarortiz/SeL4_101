#!/bin/bash
git clone --single-branch --branch 10.1.1 https://github.com/seL4/seL4.git kernel
git clone --single-branch --branch 10.1.x-compatible https://github.com/seL4/seL4_tools.git tools
git clone --single-branch --branch 10.1.x-compatible https://github.com/SEL4PROJ/riscv-pk.git tools/riscv-pk 

mkdir projects
git clone --single-branch --branch 10.1.x-compatible https://github.com/seL4/seL4_libs.git projects/seL4_libs 
git clone --single-branch --branch 10.1.x-compatible https://github.com/seL4/musllibc.git projects/musllibc 
git clone --single-branch --branch 10.1.x-compatible https://github.com/seL4/util_libs.git projects/util_libs 

ln -s tools/cmake-tool/init-build.sh init-build.sh

mkdir build
