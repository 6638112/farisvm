git clone https://github.com/mongodb/mongo-c-driver.git
cd mongo-c-driver/
apt-get install -y dh-autoreconf
./autogen.sh
#cmake CMakeLists.txt
time make
time make install
