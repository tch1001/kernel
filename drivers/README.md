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

# Compile the kernel?
## Method 1
```
# download from kernel.org
apt install -y libncurses5 libncurses5-dev bison flex
make menuconfig
apt install -y libssl-dev libelf-dev
make -j2
make modules_install
make install
```
## Method 2
```
apt-get source linux-image-`uname -r`
cd folder
fakeroot debian/rules clean
```
