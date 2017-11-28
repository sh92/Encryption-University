CC := gcc
TARGETS = \
	sha256

AddOpt = -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include -lpthread -lssl -lcrypto

all : $(TARGETS)

$(TARGETS) : $(patsubst %,%.c,$(TARGETS))
	$(CC) -o $@ $@.c $(AddOpt)
	@rm -rf $@.o

clean:
	rm -rf $(TARGETS)
	@echo "Clear ~"
