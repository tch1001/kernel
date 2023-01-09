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
```
