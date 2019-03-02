echo '== PUSH, PULL AND COMPILE =='
cd ~/frangitron/frangitron-raspberry

echo '--         Commit         --'
git add -A
git commit -m "$1"

echo '--     Push to Origin     --'
git push origin master

echo '--  SSH Pull and Compile  --'
ssh pi@192.168.1.14 '. ~/frangitron-raspberry/scripts/build.sh'

echo '== DONE =='

