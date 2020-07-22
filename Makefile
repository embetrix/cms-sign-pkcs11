CFLAGS += -Wall -DDEBUG -DUSE_PKCS11

LDFLAGS = -lcrypto

TARGET=cms-sign-pkcs11

all:  clean $(TARGET)

clean:
	$(RM) $(TARGET) *.o  *.asn *.sig* 

$(TARGET):
	$(CC) *.c  -o $@ $(LDFLAGS) $(CFLAGS)$<

.PHONY: all clean

