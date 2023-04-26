## cms-sign-pkcs11
openssl cms signature using pkcs11 engine

## Sign

        $ cms-sign-pkcs11 <sign cert> <sign key id> <input> <signature output>


## Verify

        $ openssl cms -verify -binary -content <input> -in <signature output> -certfile <sign cert> -noverify -inform DER -outform DER -out /dev/null
