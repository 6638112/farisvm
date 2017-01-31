git clone https://github.com/mongodb/libbson.git
cd libbson/
cmake CMakeLists.txt
time make
make install

######

git clone https://github.com/mongodb/mongo-c-driver.git
cd mongo-c-driver/
apt-get install -y dh-autoreconf
./autogen.sh
#cmake CMakeLists.txt
time make
time make install

######

apt-get install -y scons
git clone https://github.com/mongodb/mongo-cxx-driver.git
cd mongo-cxx-driver/
git checkout legacy
scons --disable-warnings-as-errors --prefix=/usr/local
scons --disable-warnings-as-errors --prefix=/usr/local install
