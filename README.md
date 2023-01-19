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
git am update.patch
patch -p1 < update.patch # do this cos git apply doesnt work (insert 1h of vulgarities)
git am --continue
```

