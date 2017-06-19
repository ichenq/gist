#!/bin/bash
#
# set working environment on Ubuntu, Mint
#

cd ~
if [ ! -d 'Downloads' ]; then
    mkdir Downloads
fi

cd ~/Downloads

sudo apt-get update && upgrade
sudo apt-get install build-essential git subversion python2.7 python3 python-pip python3-pip sublime-text mysql-server mysql-client phpmyadmin

# we use py3 to write script tools
sudo -H pip3 install toml pymysql pytz openpyxl requests bottle tornado  &&

# but supervisor needs py2
sudo -H pip install supervisor

if [ "$?" -ne "0" ]; then
    echo 'install python module failed'
    exit 1
fi


# setup golang enviroment
if ! command -v go > /dev/null 2>&1; then
	if [ ! -f 'go1.7.4.linux-amd64.tar.gz' ]; then
		wget ftp://192.168.0.102/Development/Golang/go1.7.4.linux-amd64.tar.gz
		# wget https://storage.googleapis.com/golang/go1.7.3.linux-amd64.tar.gz
	fi
	tar -xzvf go1.7.4.linux-amd64.tar.gz && sudo mv go /usr/local
	sudo sed -i '$a export GOROOT=/usr/local/go' /etc/profile &&
	sudo sed -i '$a export PATH=$PATH:$GOROOT/bin' /etc/profile &&
	sudo sed -i '$a export GOPATH=$HOME/Projects/3body' /etc/profile &&
	sudo sed -i '$a export PATH=$PATH:$GOPATH/bin' /etc/profile &&
	sudo sed -i '$a export PROJECT_PATH=$HOME/Projects/BeyondClient' /etc/profile &&
	sudo sed -i '$a export PROJECT_DOC_PATH=$HOME/Projects/Documents' /etc/profile

	source /etc/profile
fi

# install jdk 8
sudo add-apt-repository ppa:webupd8team/java
sudo apt-get update
sudo apt-get install oracle-java8-installer

# setup smartgit
if test -z `dpkg -l|grep smartgit`; then
	if [ ! -f 'smartgit-8_0_2.deb' ]; then
		wget ftp://192.168.0.102/Development/VCS/smartgit-8_0_2.deb
		# wget http://www.syntevo.com/smartgit/download?file=smartgit/smartgit-8_0_2.deb
	fi
	sudo dpkg -i smartgit-8_0_2.deb
fi



# setup source code tree
cd ~
if [ ! -d 'Projects' ]; then
    mkdir Projects
fi
cd ~/Projects
if [ ! -d '3body' ]; then
    git clone http://gitlab.chaoyuehudong.com/beyondtech/3body.git 3body
	cd 3body && go generate tools/lorisd && go install tools/lorisd
	cd ~/Projects
fi
if [ ! -d 'webgm' ]; then
    git clone http://gitlab.chaoyuehudong.com/beyondtech/webgm.git webgm
fi
if [ ! -d 'Documents' ]; then
    svn checkout https://192.168.0.102/svn/StarRaidersDoc Documents
fi
if [ ! -d 'BeyondClient' ]; then
    svn checkout https://192.168.0.102/svn/BeyondClient/trunk BeyondClient
fi

echo 'configuration done, start reboot'
sudo reboot
