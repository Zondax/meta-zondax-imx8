# Enable bootloader verification
HAB_SIGN_ENABLE ?= "1"
HAB_GENERATE_KEYS ?= "1"
HAB_SIGN_CA_KEYNAME ?= "root-ca"
HAB_SIGN_SRK_KEYNAME ?= "root-srk"
HAB_SIGN_CSF_KEYNAME ?= "root-csf"
HAB_SIGN_IMG_KEYNAME ?= "root-img"
HAB_SIGN_KEYDIR ?= "${TOPDIR}/keys"
HAB_SIGN_KEY_PASS ?= "dummy"

# Enable kernel verification
FIT_GENERATE_KEYS ?= "1"
FIT_SIGN_INDIVIDUAL ?= "1"
UBOOT_SIGN_ENABLE ?= "1"
UBOOT_SIGN_KEYNAME ?= "kernel-fit-conf"
UBOOT_SIGN_IMG_KEYNAME ?= "kernel-fit-image"
UBOOT_SIGN_KEYDIR ?= "${TOPDIR}/keys"
UBOOT_MKIMAGE_DTCOPTS ?= "-I dts -O dtb -p 0x1000"
