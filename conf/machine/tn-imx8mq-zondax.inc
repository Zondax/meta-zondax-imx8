MACHINEOVERRIDES =. "mx8:mx8m:mx8mq:"

require conf/machine/include/imx-base.inc
require conf/machine/include/arm/armv8a/tune-cortexa53.inc

MACHINE_FEATURES += "pci wifi bluetooth optee"

MACHINE_SOCARCH_FILTER:append:mx8mq = " virtual/libopenvg virtual/libgles1 virtual/libgles2 virtual/egl virtual/mesa virtual/libgl virtual/libg2d"

IMX_DEFAULT_KERNEL:mx8mq = "linux-tn-imx"

EXTRA_OEMAKE:append:pn-linux-tn-imx = " DTC_FLAGS='-@ -H epapr'"

IMX_DEFAULT_BOOTLOADER = "u-boot-tn-imx"

UBOOT_SUFFIX = "bin"

SPL_BINARY = "spl/u-boot-spl.bin"

# Set DDR FIRMWARE
DDR_FIRMWARE_NAME = " \
    lpddr4_pmu_train_1d_imem.bin \
    lpddr4_pmu_train_1d_dmem.bin \
    lpddr4_pmu_train_2d_imem.bin \
    lpddr4_pmu_train_2d_dmem.bin \
"

# Set ATF platform name
ATF_PLATFORM = "imx8mq"

# Extra firmware package name, that is required to build boot container for fslc bsp
IMX_EXTRA_FIRMWARE = "firmware-imx-8m"

# Set imx-mkimage boot target
IMXBOOT_TARGETS = "flash_evk flash_evk_no_hdmi flash_dp_evk"
IMX_BOOT_SOC_TARGET = "iMX8M"

SERIAL_CONSOLES = "115200;ttymxc0"

LOADADDR = ""
UBOOT_SUFFIX = "bin"
UBOOT_MAKE_TARGET = ""
IMX_BOOT_SEEK = "33"

OPTEE_BIN_EXT = "8mq"

KERNEL_CLASSES = " kernel-fitimage "
KERNEL_IMAGETYPE:aarch64 = "fitImage"
