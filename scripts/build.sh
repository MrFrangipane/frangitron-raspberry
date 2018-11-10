cd ~/frangitron-raspberry
git pull
chmod +x ~/frangitron-raspberry/scripts/build.sh

cd ~/frangitron
qmake ~/frangitron-raspberry/src/project.pro
make -j4
