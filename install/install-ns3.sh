sudo apt-get update
yes|sudo apt-get install gcc g++
yes|sudo apt-get install python python-dev
yes|sudo apt-get install qt4-dev-tools libqt4-dev
yes|sudo apt-get install mercurial
yes|sudo apt-get install bzr
yes|sudo apt-get install cmake libc6-dev libc6-dev-i386 g++-multilib
yes|sudo apt-get install gdb valgrind
yes|sudo apt-get install flex bison libfl-dev
yes|sudo apt-get install tcpdump
yes|sudo apt-get install sqlite sqlite3 libsqlite3-dev 
yes|sudo apt-get install libxml2 libxml2-dev
yes|sudo apt-get install libgtk2.0-0 libgtk2.0-dev
yes|sudo apt-get install vtun lxc
yes|sudo apt-get install uncrustify
yes|sudo apt-get install python-sphinx dia
yes|sudo apt-get install python-pygraphviz python-kiwi python-pygoocanvas libgoocanvas-dev
yes|sudo apt-get install libboost-signals-dev libboost-filesystem-dev
yes|sudo apt-get install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev

wget https://www.nsnam.org/release/ns-allinone-3.26.tar.bz2

tar jxvf ns-allinone-3.26.tar.bz2

cd ns-allinone-3.26
./build.py
cd ns-3.26
sudo ./waf -d optimized configure
sudo ./waf
