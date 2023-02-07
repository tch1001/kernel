# my kernel exploration
Run the following as root
```
make
insmod hello-1.ko
dmesg -w # preferably in another window
rmmod hello-1.ko
make clean
git status
```

# for the char device testing
```
make test
./cdev_test
make clean
```

# For inverting screen colors
```
apt install libx11-dev libxxf86vm-dev libxrandr-dev
make invert
```

If things don't work, try
```
sudo systemctl restart display-manager
xhost +
```

# Writing assembly
```
gcc -S helloworld.c # compiler
vim helloworld.s # modify stuff
gcc helloworld.s # assembler and linker
./a.out
```

# Compile the kernel
## Method 1 (worked for me)
```
# download from kernel.org
apt install -y libncurses5 libncurses5-dev bison flex
make menuconfig
vim .config # remove debian things
apt install -y libssl-dev libelf-dev
make -j2
make modules_install
make install
```
## Method 2 (didn't work for me)
```
apt-get source linux-image-`uname -r`
cd folder
fakeroot debian/rules clean
```

# Cross compiling the kernel

## Getting binutils and gcc from apt
Do this if you're a noob
```
apt-get install binutils-riscv64-linux-gnu gcc-riscv64-linux-gnu
riscv64-linux-gnu-ld --version
riscv64-linux-gnu-gcc --version
```
## Building binutils from source
```
export TARGET=aarch64-unknown-linux-gnu     # replace with your intended target
export PREFIX="$HOME/opt/cross/aarch64"     # replace with your intended path
export PATH="$PREFIX/bin:$PATH"
wget ftp://ftp.gnu.org/gnu/binutils/binutils-2.29.1.tar.xz 
tar -xf binutils-2.29.1.tar.xz
cd binutils-2.29.1
./configure \
  --target=$TARGET \
  --prefix=$PREFIX \
  --with-sysroot \
  --disable-nls \
  --disable-werror
make 
make install
$TARGET-ld --version
```

## Cross compiling the kernel
```
make ARCH=riscv O=kernel_build defconfig
export TARGET=aarch64-unknown-linux-gnu-     
make ARCH=riscv CROSS_COMPILE=$TARGET modules
```


## Contributing to mailing list
```
git clone --depth=1 https://git.kernel.org/pub/scm/linux/kernel/git/bluetooth/bluetooth-next.git/
cd bluetooth-next
# compile kernel as per usual
git checkout <for-next>
# obtain patch from email
```
git am update.patch
patch -p1 < update.patch # do this cos git apply doesnt work (insert 1h of vulgarities)
git am --continue
```

# Parts from the blog

---
layout: post
title: Compiling a Minimal Kernel
---
The exploration logs are found at [here](https://github.com/tch1001/kernel).
Let's try to compile a kernel that is as small as possible.

## Measuring Size
The kernel size can be determined using 
```bash
$ ls -lh vmlinux
-rw-r--r--  1 tanchienhao  staff   2.2K Jan 31 12:12 	README.md
```
In this case, I will measure the size of `vmlinux` instead of `vmlinuz` since compression is cheating.

## Building the Kernel 
Of course the small kernel must still be functional, so we will test it using [qemu](https://www.qemu.org/). One can also use virtualbox but I chose qemu because I will be working remotely using SSH and VNC takes too much mobile data lol.

```bash
$ git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git --depth 1 --branch v6.1.4
$ cd linux
$ git log
commit 2cb8e624295ffa0c4d659fcec7d9e7a6c48de156 (grafted, HEAD, tag: v6.1.4)
    Author: Greg Kroah-Hartman <gregkh@linuxfoundation.org>
        Date:   Sat Jan 7 11:12:04 2023 +0100
	Linux 6.1.4
$ apt install -y libncurses5 libncurses5-dev bison flex
$ make menuconfig 
# go enable PVH if u want to run in QEMU
$ vim .config # remove "debian" things, and the "certs/signing_key.pem"
$ apt install -y libssl-dev libelf-dev
$ make -j$(nproc) # change -jN to your liking
# if they prompt something involving certs, just press enter (default choices)
```

## Running the Kernel in QEMU
Important: Don't forget to enable `CONFIG_PVH=y` in `.config` if you want to run in QEMU.
```
$ ls -lh vmlinux
```
`vmlinux` should exist, if it doesn't, something went wrong and go back to [build](#building-the-kernel).

Otherwise, let's run our kernel!
### Make our initramfs
Either use
```
$ mkinitramfs -o ramdisk.img # doesn't work right now
```
OR make your own
```
$ mkdir vfs && cd vfs
$ cat << EOF > hello-kernel.c
#include <stdio.h>

int main(){
        printf("Hello, kernel!\n");
        sleep(9999999999999);
}
EOF
$ gcc --static hello-kernel.c -o init
$ find . | cpio -o -H newc | gzip > root.cpio.gz
```

### Making a hard disk (for root)
```
$ dd if=/dev/zero of=roorfs.ext2 bs=1024k count=256
$ mkfs.ext2 rootfs.ext2
```
### Busybox
Compile Busybox
```
$ git clone --depth=1 https://github.com/mirror/busybox.git && cd busybox
$ make defconfig 
$ vi .config # set CONFIG_STATIC=y
$ make -j$(nproc)
$ make CONFIG_PREFIX=$PWD/BUSYBOX install
```
Then we make out initrd with busybox 
```
$ mkdir etc proc sys
$ ls              
$ cat << EOF > init                              
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs none /sys
                                                      
mknod /dev/ttyS0 c 4 64
setsid sh -c 'exec sh </dev/ttyS0 >/dev/ttyS0 2>&1'
EOF                                                   
$ chmod +x init 
$ cp ../../busybox/BUSYBOX/* .
$ find . | cpio -o -H newc | gzip > root.cpio.gz
```

### Booting
```
# for booting hello-kernel
$ qemu-system-x86_64 -kernel arch/x86/boot/bzImage -nographic --append "console=tty0 console=ttyS0 panic=1 root=/dev/sda rootfstype=ext2" -hda rootfs.ext2 -m 1024 -vga none -display none -serial mon:stdio -no-reboot -initrd vfs/root.cpio.gz 
# for booting busybox
$ qemu-system-x86_64 -kernel arch/x86/boot/bzImage -nographic --append "console=tty0 console=ttyS0 panic=1 root=/dev/sda rootfstype=ext2" -hda rootfs.ext2 -m 1024 -vga none -display none -serial mon:stdio -no-reboot -initrd initrd/root.cpio.gz 
# qemu-system-x86_64 -kernel arch/x86_64/boot/bzImage -initrd vfs/root.cpio.gz -nographic --append "console=tty0 console=ttyS0 panic=1" -m 512 -vga none -d isplay none -serial mon:stdio -no-reboot
# qemu-system-x86_64 -kernel arch/x86/boot/bzImage -initrd vfs/root.cpio.gz -nographic --append "console=tty0 console=ttyS0 panic=1 root=/dev/sda" -hda rootfs.ext2 -m 512 -vga none -display none -serial mon:stdio -no-reboot
```
My own init works now

## Running the Kernel on Hardware
If you're on ubuntu, you can do 
```
$ update-grub2 # make sure you're using the grub bootloader
$ make modules_install
$ make install
```
When booting up, press and hold `<Shift>` to go to the grub menu, then go to advanced options to select the kernel you want to boot with.
#Debugging the Kernel using GDB
Using instructions from [here](https://www.starlab.io/blog/using-gdb-to-debug-the-linux-kernel)
```bash
vim .config # edit some stuff
make -j32
```
Running time!
```
qemu-system-x86_64 -gdb tcp::1234 -S # the rest of the stuff
gdb ./vmlinux
target remote :1234
continue
```
Use `^C` to SIGINT the kernel.
## Interrupting Startup
```
target remote :1234
hbreak start_kernel
i b # info breakpoints
c
```
Looking at the proces struct
```
(gdb) lx-ps                                               
      TASK          PID    COMM    
0xffffffff828169c0   0   swapper
(gdb) p ((struct task_struct *) 0xffffffff828169c0)
$11 = (struct task_struct *) 0xffffffff828169c0 <init_task>
(gdb) p $11->pid
$13 = 0
```
## Interrupts
The first impt thing to do is learn how to interrupts work
```
(gdb) monitor info registers
(gdb) set $idtr=0xfffffe0000000000
(gdb) print /x *(uint64_t*)$idtr
$15 = 0x81e08e00001008f0
(gdb) define idt_entry
>set $tmp = *(uint64_t*)($idtr + 8 * $arg0)
>print (void *)(($tmp>>48<<16)|($tmp&0xffff))
>end
(gdb) set $i=0
(gdb) idt_entry $i++
```

# System calls
Let's pay some attention to 
`syscalls_64.h` and `arch/x86/entry/syscall_64.c`. 
The `syscall_64.c` file generates code using `extern`.

The definition for the function is provided by the 
`fs/read_write.c`. Namely,
```c
SYSCALL_DEFINE3(lseek, unsigned int, fd, off_t, offset, unsigned int, whence)
{
	return ksys_lseek(fd, offset, whence);
}
```


## Pwn stuff
