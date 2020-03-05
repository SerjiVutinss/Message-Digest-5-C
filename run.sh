clear
# Added link flag for math.h
gcc md5-c.c sha256.c message-info.c message-block.c -lm -o md5-c
# gcc md5-c.c sha256.c message-info.c -lm -o md5-c
# gcc md5-c.c -lm -o md5-c
# Run

# file="./res/one-pad-block.txt"
file="md5-c"


./md5-c $file


echo $(sha256sum $file)