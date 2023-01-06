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
