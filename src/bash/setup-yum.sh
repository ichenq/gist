#!/bin/bash
#
# set working environment on CentOS 6.x
#

# enable eth0 on boot
sudo sed -i 's/ONBOOT=no/ONBOOT=yes/g' /etc/sysconfig/network-scripts/ifcfg-eth0

cd ~
if [ ! -d 'Downloads' ]; then
    mkdir Downloads
fi

# wandisco svn 1.9
sudo cat <<EOF > /etc/yum.repos.d/WANdisco-svn19.repo
[WANdisco-svn19]
name=WANdisco SVN Repo 1.9
enabled=1
baseurl=http://opensource.wandisco.com/centos/6/svn-1.9/RPMS/\$basearch/
gpgcheck=1
gpgkey=http://opensource.wandisco.com/RPM-GPG-KEY-WANdisco
EOF

sudo rpm -Uvh http://opensource.wandisco.com/git/wandisco-git-release-6-1.noarch.rpm
sudo wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo


sudo yum update -y &&
sudo yum install -y yum-utils epel-release zlib-devel openssl-devel freeglut-devel binutils gcc gcc-c++ subversion git


# setup devtoolset-2
if test -z `rpm -qa devtoolset-2-gcc-c++`; then
    sudo yum install -y devtoolset-2-gcc devtoolset-2-gcc-c++  devtoolset-2-binutils-devel devtoolset-2-libstdc++-devel devtoolset-2-gdb devtoolset-2-valgrind &&

    sudo sed -i '$a source /opt/rh/devtoolset-2/enable' /etc/profile &&
    sudo sed -i '$a export CC=/opt/rh/devtoolset-2/root/usr/bin/gcc' /etc/profile &&
    sudo sed -i '$a export CPP=/opt/rh/devtoolset-2/root/usr/bin/cpp' /etc/profile &&
    sudo sed -i '$a export CXX=/opt/rh/devtoolset-2/root/usr/bin/c++' /etc/profile

    if [ "$?" -ne "0" ]; then
        echo 'install devtoolset failed'
        exit 1
    fi
    source /etc/profile
fi


function build_python_cwd()
{
	sed -i "s/#SSL=\/usr\/local\/ssl/SSL=\/usr/g" Modules/Setup.dist &&
	sed -i '/_ssl _ssl.c/s/^#//' Modules/Setup.dist &&
	sed -i '/-DUSE_SSL -I$(SSL)\/include/s/^#//' Modules/Setup.dist &&
	sed -i '/-L$(SSL)\/lib -lssl -lcrypto/s/^#//' Modules/Setup.dist &&
	sed -i '/zlib zlibmodule.c/s/^#//' Modules/Setup.dist &&
	./configure && make && sudo make install || exit 1
}

# setup python 2.7
if ! command -v python2.7 > /dev/null 2>&1; then
	cd ~/Downloads
	if [ ! -f 'Python-2.7.12.tar.xz' ]; then
		wget ftp://192.168.0.102/Development/Python/Python-2.7.12.tar.xz
		# wget https://www.python.org/ftp/python/2.7.12/Python-2.7.12.tar.xz
	fi
	rm -rf Python-2.7.12
	xz -d Python-2.7.12.tar.xz && tar -xvf Python-2.7.12.tar
	cd Python-2.7.12
	install_python_cwd
	sudo ln -s /usr/local/bin/python2.7 /usr/bin/python2.7  &&
	sudo ln -s /usr/local/bin/python2.7 /usr/bin/python2 &&
	sudo ln -sf /usr/local/bin/python2.7 /usr/bin/python
fi

# setup python 3.5
if ! command -v python3.5 > /dev/null 2>&1; then
	cd ~/Downloads
	if [ ! -f 'Python-3.5.2.tar.xz' ]; then
		wget ftp://192.168.0.102/Development/Python/Python-3.5.2.tar.xz
		# wget https://www.python.org/ftp/python/3.5.2/Python-3.5.2.tar.xz
	fi
	rm -rf Python-3.5.2
	xz -d Python-3.5.2.tar.xz && tar -xvf Python-3.5.2.tar
	cd Python-3.5.2
	install_python_cwd
	sudo ln -s /usr/local/bin/python3.5 /usr/bin/python3.5  &&
	sudo ln -s /usr/local/bin/python3.5 /usr/bin/python3  &&
	sudo ln -s /usr/local/bin/pip3.5 /usr/bin/pip3
fi

cd ~/Downloads
wget https://bootstrap.pypa.io/get-pip.py
if [ "$?" -ne "0" ]; then
    echo 'download pip failed'
    exit 1
fi


# setup jdk 1.8
if ! command -v javac > /dev/null 2>&1; then
	cd ~/Downloads
	if [ ! -f 'jdk-8u111-linux-x64.rpm' ]; then
		wget ftp://192.168.0.102/Development/Java/jdk-8u111-linux-x64.rpm &&
		# wget http://download.oracle.com/otn-pub/java/jdk/8u111-b14/jdk-8u111-linux-x64.rpm?AuthParam=1479791167_7053f00405e9381ea0ffb74a978e7099
	fi
	sudo rpm -ivh jdk-8u111-linux-x64.rpm
	rm -r jdk-8u111-linux-x64.rpm
fi


# setup mysql
if ! command -v mysqld > /dev/null 2>&1; then
	cd ~/Downloads
	sudo yum remove -y mysql-libs
	wget ftp://192.168.0.102/Development/database/MySQL-5.5.52-1.el6.x86_64.rpm-bundle.tar &&
	tar -xvf MySQL-5.5.52-1.el6.x86_64.rpm-bundle.tar &&
	sudo rpm -ivh MySQL-server-5.5.52-1.el6.x86_64.rpm MySQL-client-5.5.52-1.el6.x86_64.rpm MySQL-shared-compat-5.5.52-1.el6.x86_64.rpm  MySQL-shared-5.5.52-1.el6.x86_64.rpm
	sudo chkconfig mysql on
	# /usr/bin/mysqladmin -u root password 'hellokitty'
	sudo yum install phpmyadmin
fi

# install boost 1.62
cd /usr/loca/include
if [ ! -d 'boost' ]; then
	cd ~/Downloads
	if [ ! -f 'boost_1_62_0.tar.gz' ]; then
		wget ftp://192.168.0.102/Development/SourceCode/boost_1_62_0.tar.gz
		# wget https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.tar.gz
	fi
	rm -rf boost_1_62_0
	tar -xzvf boost_1_62_0.tar.gz
	sh boost_1_62_0/bootstrap.sh && boost_1_62_0/bjam && sudo boost_1_62_0/bjam install
fi

# setup golang enviroment
if ! command -v go > /dev/null 2>&1; then
	if [ ! -f 'go1.7.3.linux-amd64.tar.gz' ]; then
		wget ftp://192.168.0.102/Development/Golang/go1.7.3.linux-amd64.tar.gz
		# wget https://storage.googleapis.com/golang/go1.7.3.linux-amd64.tar.gz
	fi
	tar -xzvf go1.7.3.linux-amd64.tar.gz && sudo mv go /usr/local
	sudo sed -i '$a export GOROOT=/usr/local/go' /etc/profile &&
	sudo sed -i '$a export PATH=$PATH:$GOROOT/bin' /etc/profile &&
	sudo sed -i '$a export GOPATH=$HOME/Projects/3body' /etc/profile &&
	sudo sed -i '$a export PATH=$PATH:$GOPATH/bin' /etc/profile &&
	sudo sed -i '$a export PROJECT_PATH=$HOME/Projects/BeyondClient' /etc/profile &&
	sudo sed -i '$a export PROJECT_DOC_PATH=$HOME/Projects/Documents' /etc/profile

	source /etc/profile
fi

# setup editor and vcs
cd ~
if [ ! -d 'App' ]; then
    mkdir App
fi

cd App
if [ ! -d 'sublime_text_3' ]; then
	wget ftp://192.168.0.102/Development/Editor/SublimeText/sublime_text_3_build_3126_x64.tar.bz2 &&
	# wget https://download.sublimetext.com/sublime_text_3_build_3126_x64.tar.bz2 &&
	tar -xjvf sublime_text_3_build_3126_x64.tar.bz2 &&
	rm -f sublime_text_3_build_3126_x64.tar.bz2
fi

if [ ! -d 'smartgit' ]; then
	wget ftp://192.168.0.102/Development/VCS/smartgit-linux-8_0_3.tar.gz &&
	# wget http://www.syntevo.com/static/smart/download/smartgit/smartgit-linux-8_0_3.tar.gz &&
	tar -xzvf smartgit-linux-8_0_3.tar.gz &&
	rm smartgit-linux-8_0_3.tar.gz
fi

if [ ! -d 'smartsvn' ]; then
	wget ftp://192.168.0.102/Development/VCS/smartsvn-linux-9_1_2.tar.gz &&
	# wget http://www.smartsvn.com/static/svn/download/smartsvn/smartsvn-linux-9_1_2.tar.gz &&
	tar -xzvf smartsvn-linux-9_1_2.tar.gz &&
	rm smartsvn-linux-9_1_2.tar.gz
fi


# setup source code tree
cd ~
if [ ! -d 'Projects' ]; then
    mkdir Projects
fi
cd ~/Projects
if [ ! -d '3body' ]; then
    git clone http://gitlab.chaoyuehudong.com/chenqiang/3body.git 3body
	cd 3body && go generate tools/lorisd && go install tools/lorisd
	cd ~/Projects
fi
if [ ! -d 'webgm' ]; then
    git clone http://gitlab.chaoyuehudong.com/chenqiang/webgm.git webgm
fi
if [ ! -d 'Documents' ]; then
    svn checkout https://192.168.0.102/svn/StarRaidersDoc Documents
fi
if [ ! -d 'BeyondClient' ]; then
    svn checkout https://192.168.0.102/svn/BeyondClient/trunk BeyondClient
    cd BeyondClient && sh Tools/deploy-battleai.sh
fi


echo 'configuration done, start reboot'
sudo reboot

