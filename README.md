## cms-sign-pkcs11
openssl cms signature using pkcs11 engine

## Generate sign Key/Cert

```
pkcs11-tool --keypairgen --key-type EC:prime256v1 --label "testkeyEC" --id 1111  --login --usage-sign
```

```
OPENSSL_CONF=openssl.cnf openssl req -engine pkcs11 -new -key 1111 -keyform engine -out sign-cert.pem -text -x509 -subj "/O=Embetrix/CN=HSM-Test/emailAddress=info@embetrix.com"
```

## Sign

create a random file and sign it:

```
dd if=/dev/urandom of=DATA bs=1M count=1
```

```
./cms-sign-pkcs11 sign-cert.pem 1111 DATA DATA.sig
```

## Verify

```
openssl cms -verify -binary -content DATA -in DATA.sig -certfile sign-cert.pem -noverify -inform DER -outform DER -out /dev/null
```
