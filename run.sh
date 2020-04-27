clear
# Added link flag for math.h
# gcc main.c sha256.c lib/md5-alg.c lib/message-info.c lib/message-block.c -lm -o app-main

gcc main.c lib/md5.c lib/sha256.c lib/message-info.c -lm -o app-main

# gcc md5-c.c sha256.c message-info.c -lm -o md5-c
# gcc md5-c.c -lm -o md5-c
# Run

# file="./res/no-pad-block.txt"
# file="./res/abc.txt"
# file="./res/two-pad-block.txt"
# file="./res/lorem-ipsum.txt"
file="./res/binary.bin"
# file="app-main"


# ./app-main $file --verbose --both --test --output myfile.txt
./app-main $file --output myfile.txt
# ./app-main --help

echo "------------------------"
echo "SHA256:"
echo $(sha256sum $file)
echo "------------------------"
echo "MD5:"
echo $(md5sum $file)
echo "------------------------"