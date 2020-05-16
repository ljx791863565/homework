#!/bin/bash

echo add=$1
/opt/carl/ipq8074/1101/OpenWrt-SDK-ipq-ipq807x_64_gcc-5.2.0_musl-1.1.16.Linux-x86_64/staging_dir/toolchain-aarch64_cortex-a53_gcc-5.2.0_musl-1.1.16/bin/aarch64-openwrt-linux-addr2line -a $1 -e quectel-CM
