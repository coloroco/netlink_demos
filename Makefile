KDIR := /lib/modules/$(shell uname -r)/build

obj-m += netlinkKernel.o

USERDEMO:=netlinkUser

all:
	make -C ${KDIR} M=${PWD} modules
	gcc -Werror -o${USERDEMO} ${USERDEMO}.c

clean:
	rm -rf *.o *.ko *.mod.* *.cmd .module* modules* Module* .*.cmd .tmp*
	make -C ${KDIR} M=${PWD} clean
	rm ${USERDEMO}
