# sudo wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
# sudo yum install devtoolset-2-gcc devtoolset-2-gcc-c++ devtoolset-2-gdb devtoolset-2-binutils devtoolset-2-libstdc++-devel devtoolset-2-libatomic-devel

source /opt/rh/devtoolset-2/enable
export CC=/opt/rh/devtoolset-2/root/usr/bin/gcc
export CPP=/opt/rh/devtoolset-2/root/usr/bin/cpp
export CXX=/opt/rh/devtoolset-2/root/usr/bin/c++
