obj-m += ip_vs_prob.o

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
install:
	insmod ip_vs_prob.ko

uninstall:
	insmod ip_vs_prob