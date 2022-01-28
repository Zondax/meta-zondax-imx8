DEPENDS += "u-boot-tools-native"

UBOOT_DTB_BINARY = ""

UBOOT_FDT_ADD_PUBKEY = "uboot-fdt_add_pubkey"

do_deploy:append:mx8m() {
    if [ "${UBOOT_SIGN_ENABLE}" = "1" ]; then
        if [ "${FIT_GENERATE_KEYS}" = "1" ]; then
            # Generate keys to sign configuration nodes, only if they don't already exist
            if [ ! -f "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_KEYNAME}".key ] || \
                [ ! -f "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_KEYNAME}".crt ]; then
                # make directory if it does not already exist
                mkdir -p "${UBOOT_SIGN_KEYDIR}"

                echo "Generating RSA private key for signing fitImage"
                openssl genrsa ${FIT_KEY_GENRSA_ARGS} -out \
                    "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_KEYNAME}".key \
                    "${FIT_SIGN_NUMBITS}"

                echo "Generating certificate for signing fitImage"
                openssl req ${FIT_KEY_REQ_ARGS} "${FIT_KEY_SIGN_PKCS}" \
                    -key "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_KEYNAME}".key \
                    -out "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_KEYNAME}".crt
            fi

            # Generate keys to sign image nodes, only if they don't already exist
            if [ ! -f "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_IMG_KEYNAME}".key ] || \
                [ ! -f "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_IMG_KEYNAME}".crt ]; then
                # make directory if it does not already exist
                mkdir -p "${UBOOT_SIGN_KEYDIR}"

                echo "Generating RSA private key for signing fitImage"
                openssl genrsa ${FIT_KEY_GENRSA_ARGS} -out \
                    "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_IMG_KEYNAME}".key \
                    "${FIT_SIGN_NUMBITS}"

                echo "Generating certificate for signing fitImage"
                openssl req ${FIT_KEY_REQ_ARGS} "${FIT_KEY_SIGN_PKCS}" \
                    -key "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_IMG_KEYNAME}".key \
                    -out "${UBOOT_SIGN_KEYDIR}/${UBOOT_SIGN_IMG_KEYNAME}".crt
            fi
        fi

        if [ -n "${UBOOT_CONFIG}" ]; then
            unset i j

            for config in ${UBOOT_MACHINE}; do
                i=$(expr $i + 1)

                for type in ${UBOOT_CONFIG}; do
                    j=$(expr $j + 1)

                    if [ $j -eq $i ]; then
                        for dtb in ${UBOOT_DTB_NAME}; do
                            filepath="${DEPLOYDIR}/${BOOT_TOOLS}/${dtb}"

                            if [ -f "${filepath}" ]; then
                                ${UBOOT_FDT_ADD_PUBKEY} \
                                    -a "${FIT_HASH_ALG},${FIT_SIGN_ALG}" \
                                    -k "${UBOOT_SIGN_KEYDIR}" \
                                    -n "${UBOOT_SIGN_KEYNAME}" \
                                    -r conf ${filepath}

                                if [ "${FIT_SIGN_INDIVIDUAL}" = "1" ]; then
                                    ${UBOOT_FDT_ADD_PUBKEY} \
                                        -a "${FIT_HASH_ALG},${FIT_SIGN_ALG}" \
                                        -k "${UBOOT_SIGN_KEYDIR}" \
                                        -n "${UBOOT_SIGN_IMG_KEYNAME}" \
                                        -r image ${filepath}
                                fi
                            fi
                        done
                    fi
                done

                unset j
            done

            unset i
        fi
    fi
}
