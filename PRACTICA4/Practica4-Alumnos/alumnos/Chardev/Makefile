obj-m += chardev.o

all:
	make -d -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -d -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
