## cms-sign-pkcs11
openssl cms signature using pkcs11 engine

## List Tokens URLs

```
p11tool --list-token-urls --provider=/usr/lib/softhsm/libsofthsm2.so
pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=3c73b9b698a4edb1;token=token1
```

## Generate sign Key/Cert

```
pkcs11-tool --module /usr/lib/softhsm/libsofthsm2.so --keypairgen --key-type EC:prime256v1 --label "testkeyEC7598" --id 7598  --login --usage-sign
```

```
openssl req -engine pkcs11 -new -key "pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=09abc2fa100c0143;token=token1;object=testkeyEC7598&pin-value=12345" -keyform engine -out sign-cert.pem -text -x509 -subj "/O=Embetrix/CN=HSM-Test/emailAddress=info@embetrix.com"
```

## Sign

create a random file and sign it:

```
dd if=/dev/urandom of=DATA bs=1M count=1
```

```
./cms-sign-pkcs11 sign-cert.pem "pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=09abc2fa100c0143;token=token1;object=testkeyEC7598&pin-value=12345" DATA DATA.sig
```

## Verify

```
openssl cms -verify -binary -content DATA -in DATA.sig -certfile sign-cert.pem -noverify -inform DER -outform DER -out /dev/null
```
