MACHINEOVERRIDES =. "mx8:mx8m:mx8mq:"

require conf/machine/include/imx-base.inc
require conf/machine/include/arm/armv8a/tune-cortexa53.inc

MACHINE_FEATURES += "pci wifi bluetooth optee bcm43455 bcm4356 bcm4359"

MACHINE_SOCARCH_FILTER:append:mx8mq = " virtual/libopenvg virtual/libgles1 virtual/libgles2 virtual/egl virtual/mesa virtual/libgl virtual/libg2d"

KERNEL_DEVICETREE = " \
    freescale/imx8mq-evk.dtb \
    freescale/imx8mq-evk-ak4497.dtb \
    freescale/imx8mq-evk-audio-tdm.dtb \
    freescale/imx8mq-evk-dcss-adv7535.dtb \
    freescale/imx8mq-evk-dcss-rm67191.dtb \
    freescale/imx8mq-evk-dcss-rm67199.dtb \
    freescale/imx8mq-evk-dp.dtb \
    freescale/imx8mq-evk-dual-display.dtb \
    freescale/imx8mq-evk-inmate.dtb \
    freescale/imx8mq-evk-lcdif-adv7535.dtb \
    freescale/imx8mq-evk-lcdif-rm67191.dtb \
    freescale/imx8mq-evk-lcdif-rm67199.dtb \
    freescale/imx8mq-evk-pcie-ep.dtb \
    freescale/imx8mq-evk-pcie1-m2.dtb \
    freescale/imx8mq-evk-pdm.dtb \
    freescale/imx8mq-evk-root.dtb \
    freescale/imx8mq-evk-rpmsg.dtb \
    freescale/imx8mq-evk-usd-wifi.dtb \
    freescale/imx8mq-evk-usdhc2-m2.dtb \
"

IMX_DEFAULT_KERNEL:mx8mq = "linux-imx"

IMX_DEFAULT_BOOTLOADER = "u-boot-imx"

UBOOT_SUFFIX = "bin"

UBOOT_CONFIG ??= "sd"
UBOOT_CONFIG[sd] = "imx8mq_evk_config,sdcard"
UBOOT_CONFIG[mfgtool] = "imx8mq_evk_config"

SPL_BINARY = "spl/u-boot-spl.bin"

# Set DDR FIRMWARE
DDR_FIRMWARE_NAME = " \
    lpddr4_pmu_train_1d_imem.bin \
    lpddr4_pmu_train_1d_dmem.bin \
    lpddr4_pmu_train_2d_imem.bin \
    lpddr4_pmu_train_2d_dmem.bin \
"

# Set u-boot DTB
UBOOT_DTB_NAME = "imx8mq-evk.dtb"

# Set ATF platform name
ATF_PLATFORM = "imx8mq"

# Extra firmware package name, that is required to build boot container for fslc bsp
IMX_EXTRA_FIRMWARE = "firmware-imx-8m"

# Set imx-mkimage boot target
IMXBOOT_TARGETS = "flash_evk flash_evk_no_hdmi flash_dp_evk"
IMX_BOOT_SOC_TARGET = "iMX8M"

SERIAL_CONSOLES = "115200;ttymxc0"

LOADADDR = ""
UBOOT_MAKE_TARGET = ""
IMX_BOOT_SEEK = "33"

OPTEE_BIN_EXT = "8mq"

# Add additional firmware
MACHINE_FIRMWARE:append = " linux-firmware-ath10k"

KERNEL_CLASSES = " kernel-fitimage "
KERNEL_IMAGETYPE:aarch64 = "fitImage"
