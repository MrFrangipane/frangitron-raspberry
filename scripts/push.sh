echo '== PUSH, PULL AND COMPILE =='
cd ~/frangitron/frangitron-raspberry

echo '--     Push to Origin     --'
git push origin master

echo '--  SSH Pull and Compile  --'
ssh pi@192.168.1.12 '. ~/frangitron-raspberry/scripts/build.sh'

echo '== DONE =='

