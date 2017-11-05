TARGETS = \
	server\
	client

AddOpt = -l pthread -I /usr/local/opt/openssl/include -L /usr/local/opt/openssl/lib -lcrypto


all : $(TARGETS)

$(TARGETS) : $(patsubst %,%.c,$(TARGETS))
	$(CC) -g -o $@ $@.c $(AddOpt)
	@rm -rf $@.o

clean:
	rm -rf $(TARGETS)
	rm -rf *.dSYM
	@echo "Clear ~"
