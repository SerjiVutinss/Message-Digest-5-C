clear
# Added link flag for math.h
gcc main.c sha256.c message-info.c message-block.c -lm -o app-main
# gcc md5-c.c sha256.c message-info.c -lm -o md5-c
# gcc md5-c.c -lm -o md5-c
# Run

# file="./res/one-pad-block.txt"
file="app-main"


./app-main $file


echo $(sha256sum $file)