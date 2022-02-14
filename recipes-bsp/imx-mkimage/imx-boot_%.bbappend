FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://openssl.cnf"

DEPENDS += "openssl-native imx-cst-native"

HAB_SIGN_ENABLE ?= "0"

HAB_GENERATE_KEYS ?= "0"

HAB_SIGN_CA_KEYNAME ?= ""
HAB_SIGN_SRK_KEYNAME ?= ""
HAB_SIGN_CSF_KEYNAME ?= ""
HAB_SIGN_IMG_KEYNAME ?= ""
HAB_SIGN_KEYDIR ?= ""

HAB_SIGN_CA_KEY = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_CA_KEYNAME}_key.pem"
HAB_SIGN_CA_CRT = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_CA_KEYNAME}_crt.pem"
HAB_SIGN_SRK_KEY = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_SRK_KEYNAME}_key.pem"
HAB_SIGN_SRK_CRT = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_SRK_KEYNAME}_crt.pem"
HAB_SIGN_CSF_KEY = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_CSF_KEYNAME}_key.pem"
HAB_SIGN_CSF_CRT = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_CSF_KEYNAME}_crt.pem"
HAB_SIGN_IMG_KEY = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_IMG_KEYNAME}_key.pem"
HAB_SIGN_IMG_CRT = "${HAB_SIGN_KEYDIR}/${HAB_SIGN_IMG_KEYNAME}_crt.pem"
HAB_SIGN_KEY_PASS ?= ""

HAB_SIGN_KEY_DAYS ?= "3650"

SRK_TABLE_FILE = "${HAB_SIGN_KEYDIR}/srk-table.bin"
SRK_HASH_FUSE_FILE = "${B}/srk-hash-fuses.bin"

#copy from "meta-freescale/recipes-bsp/imx-mkimage/imx-boot_1.0.bb"
compile_mx8m() {
    bbnote 8MQ/8MM/8MN/8MP boot binary build
    for ddr_firmware in ${DDR_FIRMWARE_NAME}; do
        bbnote "Copy ddr_firmware: ${ddr_firmware} from ${DEPLOY_DIR_IMAGE} -> ${BOOT_STAGING} "
        cp ${DEPLOY_DIR_IMAGE}/${ddr_firmware}               ${BOOT_STAGING}
    done
    cp ${DEPLOY_DIR_IMAGE}/signed_dp_imx8m.bin               ${BOOT_STAGING}
    cp ${DEPLOY_DIR_IMAGE}/signed_hdmi_imx8m.bin             ${BOOT_STAGING}
    cp ${DEPLOY_DIR_IMAGE}/u-boot-spl.bin-${MACHINE}-${UBOOT_CONFIG} \
                                                             ${BOOT_STAGING}/u-boot-spl.bin

    for dtb in ${UBOOT_DTB_NAME}; do
        cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/${dtb}          ${BOOT_STAGING}
    done

    cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/u-boot-nodtb.bin-${MACHINE}-${UBOOT_CONFIG} \
                                                             ${BOOT_STAGING}/u-boot-nodtb.bin
    cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/${ATF_MACHINE_NAME} ${BOOT_STAGING}/bl31.bin
    cp ${DEPLOY_DIR_IMAGE}/${UBOOT_NAME}                     ${BOOT_STAGING}/u-boot.bin
}

do_install:append:mx8m() {
    if [ "${HAB_SIGN_ENABLE}" = "1" ]; then
        if [ "${HAB_GENERATE_KEYS}" = "1" ]; then
            # Make directory if it does not already exist
            mkdir -p "${HAB_SIGN_KEYDIR}"

            # Make files required for signing
            cd "${WORKDIR}"
            touch index.txt
            echo "00" > serial

            # Generate CA key and certificate
            if [ ! -f "${HAB_SIGN_CA_KEY}" ] || \
                [ ! -f "${HAB_SIGN_CA_CRT}" ]; then
                echo "Removing dependent keys and certificates"
                rm -f "${HAB_SIGN_SRK_KEY}"
                rm -f "${HAB_SIGN_SRK_CRT}"
                rm -f "${HAB_SIGN_CSF_KEY}"
                rm -f "${HAB_SIGN_CSF_CRT}"
                rm -f "${HAB_SIGN_IMG_KEY}"
                rm -f "${HAB_SIGN_IMG_CRT}"

                echo "Generating CA private key"
                openssl genpkey -algorithm rsa \
                    -pkeyopt rsa_keygen_bits:2048 \
                    -pkeyopt rsa_keygen_pubexp:65537 \
                    -des3 -pass pass:"${HAB_SIGN_KEY_PASS}" \
                    -out "${HAB_SIGN_CA_KEY}"

                echo "Generating CA certificate"
                openssl req -x509 \
                    -key "${HAB_SIGN_CA_KEY}" \
                    -passin pass:"${HAB_SIGN_KEY_PASS}" \
                    -subj "/CN=${HAB_SIGN_CA_KEYNAME}/" \
                    -config "${WORKDIR}/openssl.cnf" -extensions v3_ca \
                    -days "${HAB_SIGN_KEY_DAYS}" \
                    -out "${HAB_SIGN_CA_CRT}"
            fi

            # Generate SRK key and certificate
            if [ ! -f "${HAB_SIGN_SRK_KEY}" ] || \
                [ ! -f "${HAB_SIGN_SRK_CRT}" ]; then
                echo "Removing dependent keys and certificates"
                rm -f "${HAB_SIGN_CSF_KEY}"
                rm -f "${HAB_SIGN_CSF_CRT}"
                rm -f "${HAB_SIGN_IMG_KEY}"
                rm -f "${HAB_SIGN_IMG_CRT}"

                echo "Generating SRK private key"
                openssl genpkey -algorithm rsa \
                    -pkeyopt rsa_keygen_bits:2048 \
                    -pkeyopt rsa_keygen_pubexp:65537 \
                    -des3 -pass pass:"${HAB_SIGN_KEY_PASS}" \
                    -out "${HAB_SIGN_SRK_KEY}"

                echo "Generating SRK certificate signing request"
                openssl req -new -batch \
                    -key "${HAB_SIGN_SRK_KEY}" \
                    -passin pass:"${HAB_SIGN_KEY_PASS}" \
                    -subj "/CN=${HAB_SIGN_SRK_KEYNAME}/" \
                    -out csr.pem

                echo "Generating and signing SRK certificate"
                openssl ca -batch -md sha256 \
                    -in csr.pem \
                    -cert "${HAB_SIGN_CA_CRT}" \
                    -keyfile "${HAB_SIGN_CA_KEY}" \
                    -passin pass:"${HAB_SIGN_KEY_PASS}" \
                    -config "${WORKDIR}/openssl.cnf" -extensions v3_ca \
                    -days "${HAB_SIGN_KEY_DAYS}" \
                    -out "${HAB_SIGN_SRK_CRT}"

                echo "Removing temporary files"
                rm -f csr.pem
            fi

            # Generate CSF key and certificate
            if [ ! -f "${HAB_SIGN_CSF_KEY}" ] || \
                [ ! -f "${HAB_SIGN_CSF_CRT}" ]; then
                echo "Generating CSF private key"
                openssl genpkey -algorithm rsa \
                    -pkeyopt rsa_keygen_bits:2048 \
                    -pkeyopt rsa_keygen_pubexp:65537 \
                    -des3 -pass pass:"${HAB_SIGN_KEY_PASS}" \
                    -out "${HAB_SIGN_CSF_KEY}"

                echo "Generating CSF certificate signing request"
                openssl req -new -batch \
                    -key "${HAB_SIGN_CSF_KEY}" \
                    -passin pass:"${HAB_SIGN_KEY_PASS}" \
                    -subj "/CN=${HAB_SIGN_CSF_KEYNAME}/" \
                    -out csr.pem

                echo "Generating and signing CSF certificate"
                openssl ca -batch -md sha256 \
                    -in csr.pem \
                    -cert "${HAB_SIGN_CA_CRT}" \
                    -keyfile "${HAB_SIGN_CA_KEY}" \
                    -passin pass:"${HAB_SIGN_KEY_PASS}" \
                    -config "${WORKDIR}/openssl.cnf" -extensions v3_usr \
                    -days "${HAB_SIGN_KEY_DAYS}" \
                    -out "${HAB_SIGN_CSF_CRT}"

                echo "Removing temporary files"
                rm -f csr.pem
            fi

            # Generate IMG key and certificate
            if [ ! -f "${HAB_SIGN_IMG_KEY}" ] || \
                [ ! -f "${HAB_SIGN_IMG_CRT}" ]; then
                echo "Generating IMG private key"
                openssl genpkey -algorithm rsa \
                    -pkeyopt rsa_keygen_bits:2048 \
                    -pkeyopt rsa_keygen_pubexp:65537 \
                    -des3 -pass pass:"${HAB_SIGN_KEY_PASS}" \
                    -out "${HAB_SIGN_IMG_KEY}"

                echo "Generating IMG certificate signing request"
                openssl req -new -batch \
                    -key "${HAB_SIGN_IMG_KEY}" \
                    -passin pass:"${HAB_SIGN_KEY_PASS}" \
                    -subj "/CN=${HAB_SIGN_IMG_KEYNAME}/" \
                    -out csr.pem

                echo "Generating and signing IMG certificate"
                openssl ca -batch -md sha256 \
                    -in csr.pem \
                    -cert "${HAB_SIGN_CA_CRT}" \
                    -keyfile "${HAB_SIGN_CA_KEY}" \
                    -passin pass:"${HAB_SIGN_KEY_PASS}" \
                    -config "${WORKDIR}/openssl.cnf" -extensions v3_usr \
                    -days "${HAB_SIGN_KEY_DAYS}" \
                    -out "${HAB_SIGN_IMG_CRT}"

                echo "Removing temporary files"
                rm -f csr.pem
            fi

            # Generate SRK table and hash fuses
            echo "Generating SRK public key table and hash fuses"
            srktool -h 4 -d sha256 \
                -t "${SRK_TABLE_FILE}" \
                -e "${SRK_HASH_FUSE_FILE}" \
                -c "${HAB_SIGN_SRK_CRT}"

            # Install hash fuses to /boot
            install -d ${D}/boot
            install -m 644 ${SRK_HASH_FUSE_FILE} ${D}/boot
        fi
    fi
}
