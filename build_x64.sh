apt-get install python3-dev python3-pip
pip3 install Flask 
cp libs/x64/libmsc.so ./
make
cd record
make 
cd ..
