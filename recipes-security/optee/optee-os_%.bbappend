PROVIDES += "virtual/optee-os"
RPROVIDES_${PN} += "virtual/optee-os"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0006-allow-setting-sysroot-for-libgcc-lookup.patch"

EXTRA_OEMAKE  += "${@bb.utils.contains('ENABLE_TA_SIGNING', '1', 'TA_PUBLIC_KEY=${TA_CUSTOM_PUBKEY}', '', d)}"
