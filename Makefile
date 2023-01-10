#obj-m += keycapture.o 
#obj-m += hello-1.o 
#obj-m += 1-mem.o
#obj-m += 2-sched-spin.o
#obj-m += 3-memory.o
# obj-m += device_drivers/extra/modul.o
#obj-m += device_drivers/kernel/cdev.o
obj-m += epilepsy.o
#obj-m += filetest.o


PWD := $(CURDIR) 

all: invert
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

invert: invert.c invert_back.c
	gcc -c invert.c -I /usr/X11R6/include -DXCALIB_VERSION="0.10" -g
	gcc -L /usr/X11R6/lib -lm -o invert invert.o -lX11 -lXrandr -lXxf86vm -lXext -lm -g
	gcc -c invert_back.c -I /usr/X11R6/include -DXCALIB_VERSION="0.10" -g
	gcc -L /usr/X11R6/lib -lm -o invert_back invert_back.o -lX11 -lXrandr -lXxf86vm -lXext -lm -g

test: device_drivers/user/cdev_test.c
	gcc -static -o cdev_test device_drivers/user/cdev_test.c

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-rm -f *~ *.o cdev_test
	rm invert
	rm invert_back
