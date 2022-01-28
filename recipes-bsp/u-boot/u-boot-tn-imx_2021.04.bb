# Copyright (C) 2013-2016 Freescale Semiconductor
# Copyright 2017-2018 NXP
# Copyright (C) 2020 TechNexion.

DESCRIPTION = "i.MX U-Boot suppporting TechNexion i.MX boards."
HOMEPAGE = "http://www.denx.de/wiki/U-Boot/WebHome"
SECTION = "bootloaders"

require recipes-bsp/u-boot/u-boot.inc

PROVIDES += "u-boot"
DEPENDS:append = " dtc-native flex-native bison-native"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://Licenses/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263"

PR = "r0"
SRCSERVER = "git://github.com/TechNexion/u-boot-tn-imx.git;protocol=https"
SRCOPTIONS = ""
SRCBRANCH = "tn-imx_v2021.04_5.10.72_2.2.0-next"
SRC_URI = "${SRCSERVER};branch=${SRCBRANCH}${SRCOPTIONS}"
SRCREV = "510b22fcbf369f6ac08c349981333f7cf6f2a9cc"
SRC_URI:append = " file://splash.bmp"

S = "${WORKDIR}/git"

inherit fsl-u-boot-localversion

LOCALVERSION ?= "-${SRCREV}"

BOOT_TOOLS = "imx-boot-tools"

do_deploy:append () {
    install -d ${DEPLOYDIR}
    install ${WORKDIR}/splash.bmp ${DEPLOYDIR}/splash.bmp
}

do_deploy:append:mx8m() {
    # Deploy u-boot-nodtb.bin and fsl-imx8m*-XX.dtb for mkimage to generate boot binary
    if [ -n "${UBOOT_CONFIG}" ]
    then
        for config in ${UBOOT_MACHINE}; do
            i=$(expr $i + 1);
            for type in ${UBOOT_CONFIG}; do
                j=$(expr $j + 1);
                if [ $j -eq $i ]
                then
                    install -d ${DEPLOYDIR}/${BOOT_TOOLS}
                    for dtb in ${UBOOT_DTB_NAME}; do
                        install -m 0777 ${B}/${config}/arch/arm/dts/${dtb}  ${DEPLOYDIR}/${BOOT_TOOLS}
                    done
                    install -m 0777 ${B}/${config}/u-boot-nodtb.bin  ${DEPLOYDIR}/${BOOT_TOOLS}/u-boot-nodtb.bin-${MACHINE}-${type}
                fi
            done
            unset  j
        done
        unset  i
    fi
}

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "(mx6|mx7|mx8)"

UBOOT_NAME:mx6 = "u-boot-${MACHINE}.bin-${UBOOT_CONFIG}"
UBOOT_NAME:mx7 = "u-boot-${MACHINE}.bin-${UBOOT_CONFIG}"
UBOOT_NAME:mx8 = "u-boot-${MACHINE}.bin-${UBOOT_CONFIG}"
