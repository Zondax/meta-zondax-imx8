FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0001-tools-add-fdt_add_pubkey.patch"

do_install:append() {
    # fdt_add_pubkey
    install -m 0755 tools/fdt_add_pubkey ${D}${bindir}/uboot-fdt_add_pubkey
    ln -sf uboot-fdt_add_pubkey ${D}${bindir}/fdt_add_pubkey
}

FILES:${PN}-mkimage += "${bindir}/uboot-fdt_add_pubkey ${bindir}/fdt_add_pubkey"
