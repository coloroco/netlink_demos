KDIR := /lib/modules/$(shell uname -r)/build

obj-m += netlinkKernel.o

all:
	make -C ${KDIR} M=${PWD} modules

clean:
	rm -rf *.o *.ko *.mod.* *.cmd .module* modules* Module* .*.cmd .tmp*
	make -C ${KDIR} M=${PWD} clean
