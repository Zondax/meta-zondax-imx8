FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0001-imx8mm_evk-imx8mq_evk-enable-signature-verification.patch"

require u-boot-add-pubkey.inc
