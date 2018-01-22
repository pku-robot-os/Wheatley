apt-get install python3-dev python3-pip
pip install Flask webbrowser
cp libs/x86/libmsc.so ./
make
cd record
make 
cd ..
