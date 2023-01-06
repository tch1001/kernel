obj-m += keycapture.o 
obj-m += hello-1.o 
obj-m += 1-mem.o

PWD := $(CURDIR) 

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
