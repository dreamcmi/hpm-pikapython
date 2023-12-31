# Copyright (c) 2023 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

set(src_crypto
    aes.c
    aesni.c
    aria.c
    asn1parse.c
    asn1write.c
    base64.c
    bignum.c
    blowfish.c
    camellia.c
    ccm.c
    chacha20.c
    chachapoly.c
    cipher.c
    cipher_wrap.c
    cmac.c
    ctr_drbg.c
    des.c
    certs.c
    dhm.c
    ecdh.c
    ecdsa.c
    ecjpake.c
    ecp.c
    ecp_curves.c
    entropy.c
    entropy_poll.c
    error.c
    gcm.c
    havege.c
    hkdf.c
    hmac_drbg.c
    md.c
    md4.c
    md5.c
    memory_buffer_alloc.c
    nist_kw.c
    oid.c
    padlock.c
    pem.c
    pk.c
    pk_wrap.c
    pkcs12.c
    pkcs5.c
    pkparse.c
    pkwrite.c
    platform.c
    platform_util.c
    poly1305.c
    psa_crypto.c
    psa_crypto_aead.c
    psa_crypto_cipher.c
    psa_crypto_client.c
    psa_crypto_driver_wrappers.c
    psa_crypto_ecp.c
    psa_crypto_hash.c
    psa_crypto_mac.c
    psa_crypto_rsa.c
    psa_crypto_se.c
    psa_crypto_slot_management.c
    psa_crypto_storage.c
    psa_its_file.c
    ripemd160.c
    rsa.c
    rsa_internal.c
    sha1.c
    sha256.c
    sha512.c
    threading.c
    timing.c
    version.c
)

sdk_src(${src_crypto})

set(src_x509
    x509.c
    x509_create.c
    x509_crl.c
    x509_crt.c
    x509_csr.c
    x509write_crt.c
    x509write_csr.c
)
sdk_src(${src_x509})

set(src_tls
    debug.c
    mps_reader.c
    mps_trace.c
    net_sockets.c
    ssl_cache.c
    ssl_ciphersuites.c
    ssl_cookie.c
    ssl_msg.c
    ssl_ticket.c
    ssl_tls.c
    ssl_cli.c
    ssl_srv.c
    ssl_tls13_keys.c
)
sdk_src(${src_tls})

