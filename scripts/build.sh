echo "-- pull --"
cd ~/frangitron-raspberry
git pull
chmod +x ~/frangitron-raspberry/scripts/build.sh

echo "-- make --"
cd ~/frangitron
qmake ~/frangitron-raspberry/src/frangitron.pro
make clean
make -j4

echo "-- run  --"
./frangitron --platform linuxfb
