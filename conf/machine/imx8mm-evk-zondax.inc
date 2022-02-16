MACHINEOVERRIDES =. "mx8:mx8m:mx8mm:"

require conf/machine/include/imx-base.inc
require conf/machine/include/arm/armv8a/tune-cortexa53.inc

MACHINE_FEATURES += "pci wifi bluetooth optee bcm43455 bcm4356 bcm4359"

MACHINE_SOCARCH_FILTER:append:mx8mm = " virtual/libopenvg virtual/libgles1 virtual/libgles2 virtual/egl virtual/mesa virtual/libgl virtual/libg2d"

KERNEL_DEVICETREE = " \
    freescale/imx8mm-evk.dtb \
    freescale/imx8mm-evk-pcie-ep.dtb \
    freescale/imx8mm-evk-rm67191.dtb \
    freescale/imx8mm-evk-8mic-revE.dtb \
    freescale/imx8mm-evk-8mic-swpdm.dtb \
    freescale/imx8mm-evk-ak4497.dtb \
    freescale/imx8mm-evk-ak5558.dtb \
    freescale/imx8mm-evk-audio-tdm.dtb \
    freescale/imx8mm-evk-inmate.dtb \
    freescale/imx8mm-evk-qca-wifi.dtb \
    freescale/imx8mm-evk-revb-qca-wifi.dtb \
    freescale/imx8mm-evk-root.dtb \
    freescale/imx8mm-evk-rpmsg.dtb \
    freescale/imx8mm-evk-usd-wifi.dtb \
"

IMX_DEFAULT_KERNEL:mx8mm = "linux-imx"

IMX_DEFAULT_BOOTLOADER = "u-boot-imx"

UBOOT_SUFFIX = "bin"

UBOOT_CONFIG ??= "sd"
UBOOT_CONFIG[sd] = "imx8mm_evk_defconfig,sdcard"
UBOOT_CONFIG[mfgtool] = "imx8mm_evk_defconfig"

SPL_BINARY = "spl/u-boot-spl.bin"

# Set DDR FIRMWARE
DDR_FIRMWARE_NAME = " \
    lpddr4_pmu_train_1d_imem.bin \
    lpddr4_pmu_train_1d_dmem.bin \
    lpddr4_pmu_train_2d_imem.bin \
    lpddr4_pmu_train_2d_dmem.bin \
"

# Set u-boot DTB
UBOOT_DTB_NAME = "imx8mm-evk.dtb"

# Set ATF platform name
ATF_PLATFORM = "imx8mm"
ATF_LOAD_ADDR = "0x920000"

# Extra firmware package name, that is required to build boot container for fslc bsp
IMX_EXTRA_FIRMWARE = "firmware-imx-8m"

# Set imx-mkimage boot target
IMXBOOT_TARGETS = "flash_evk flash_evk_no_hdmi"
IMX_BOOT_SOC_TARGET = "iMX8MM"

SERIAL_CONSOLES = "115200;ttymxc1"

LOADADDR = ""
UBOOT_MAKE_TARGET = "all"
IMX_BOOT_SEEK = "33"

OPTEE_BIN_EXT = "8mm"
TEE_LOAD_ADDR = "0xbe000000"

# Add additional firmware
MACHINE_FIRMWARE:append = " linux-firmware-ath10k"

KERNEL_CLASSES = " kernel-fitimage "
KERNEL_IMAGETYPE:aarch64 = "fitImage"
