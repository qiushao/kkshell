
# kkshell

A simple terminal for serial, ssh, telnet, local shell。

## build
### install dependency
for debian base system, just apt install:
```
sudo apt install sudo apt install build-essential cmake qt5-default qtcreator
sudo apt install qt5-doc qt5-doc-html qtbase5-doc-html qtbase5-examples
sudo apt install libqt5serialport5-dev qt5serialport-examples
sudo apt install libutf8proc-dev
sudo apt install libssh2-1-dev
sudo apt install libssl-dev
``` 

### build and run
```
git clone https://github.com/qiushao/kkshell.git
cd kkshell
mkdir build && cd build
cmake ..
make
./kkshell 
```

## Dependencies
- [QT](https://www.qt.io/developers)
- [qtermwidget](https://github.com/lxqt/qtermwidget)
- [deepin-terminal](https://github.com/linuxdeepin/deepin-terminal)
- [ssh2](https://github.com/libssh2/libssh2)
- [openssl](https://github.com/openssl/openssl)

