CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n

KDIR = /lib/modules/$(shell uname -r)/build/

all:
	make -C $(KDIR) M="$(PWD)" modules
	$(CC) generator.c -o a.out
clean:
	make -C $(KDIR) M="$(PWD)" clean
	rm a.out