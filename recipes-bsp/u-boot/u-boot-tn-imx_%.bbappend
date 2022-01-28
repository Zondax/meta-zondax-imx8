FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0001-flex-imx8mm-pico-imx8mm-pico-imx8mq-enable-signature.patch"

require u-boot-add-pubkey.inc
