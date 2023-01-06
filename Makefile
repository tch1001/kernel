obj-m += keycapture.o 
obj-m += hello-1.o 
obj-m += 1-mem.o
obj-m += 2-sched-spin.o
obj-m += 3-memory.o
# obj-m += device_drivers/extra/modul.o
obj-m += device_drivers/kernel/cdev.o
obj-m += epilepsy.o


PWD := $(CURDIR) 

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

test: device_drivers/user/cdev_test.c
	gcc -static -o cdev_test device_drivers/user/cdev_test.c

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-rm -f *~ *.o cdev_test
