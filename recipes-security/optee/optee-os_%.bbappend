PROVIDES += "virtual/optee-os"
RPROVIDES_${PN} += "virtual/optee-os"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0006-allow-setting-sysroot-for-libgcc-lookup.patch"