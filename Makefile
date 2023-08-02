CFLAGS += -Wall -DDEBUG  -Wno-deprecated-declarations
LIBS    := -lcrypto
LDFLAGS :=

TARGET=cms-sign-pkcs11

all:  clean $(TARGET)

clean:
	$(RM) $(TARGET) *.o  *.asn *.sig* 

$(TARGET):
	$(CC) *.c  -o $@ $(LDFLAGS) $(LIBS) $(CFLAGS)$<

.PHONY: all clean

