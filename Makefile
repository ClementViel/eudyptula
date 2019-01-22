obj-m := task08.o
KERNELDIR := /lib/modules/`uname -r`/build



all:
	make -C $(KERNELDIR) M=$(shell pwd) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
