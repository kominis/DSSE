# Dynamic SSE Scheme
This repository contains implementation of a DSSE scheme named Mara. This project was developed for my bachelor thesis at Harokopio University of Athens.

# Pre-requisites
For implementing the scheme a compiler supporting C++14 is needed. It has been successfully built and tested on Ubuntu 18.04 LTS. The Linux build commands are as follows.

## Linux
```
$ sudo apt-get update
$ sudo apt-get install build-essential openssl scons make
$ sudo apt-get install gcc-5 g++-5
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 1
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 1
```

## Installing libsodium
[Libsodium](https://libsodium.gitbook.io/doc/) cryptographic library. The code has been compiled and tested using Libsodium 1.0.18.
```
$ wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.18-stable.tar.gz
$ tar -xvf libsodium-1.0.18-stable.tar.gz
$ cd libsodium-stable
$ ./configure
$ make && make check
$ sudo make install
```

# Building
Building is done through [SConstruct](http://www.scons.org).
To build the code just enter in your terminal
```
$ scons
```

# Usage
This repository provides implementation of DSSE as a proof of concept, and cannot be used for real sensitive applications.

After compiling the project, a binary file will be generated named mara. This executable is designed  to be run on a single system. It executes client and server operation as a local command and no communication cost is being considered.
