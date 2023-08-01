## cms-sign-pkcs11
openssl cms signature using pkcs11 engine

## Generate sign Key/Cert

```
pkcs11-tool --module /usr/lib/softhsm/libsofthsm2.so --keypairgen --key-type EC:prime256v1 --label "testkeyEC7598" --id 7598  --login --usage-sign
```

```
OPENSSL_CONF=openssl.cnf openssl req -engine pkcs11 -new -key 7598 -keyform engine -out sign-cert.pem -text -x509 -subj "/O=Embetrix/CN=HSM-Test/emailAddress=info@embetrix.com"
```

## Sign

create a random file and sign it:

```
dd if=/dev/urandom of=DATA bs=1M count=1
```

```
./cms-sign-pkcs11 sign-cert.pem 7598 DATA DATA.sig
```

## Verify

```
openssl cms -verify -binary -content DATA -in DATA.sig -certfile sign-cert.pem -noverify -inform DER -outform DER -out /dev/null
```
