clear
# Added link flag for math.h
# gcc main.c sha256.c lib/md5-alg.c lib/message-info.c lib/message-block.c -lm -o app-main

programName=hashC

gcc main.c lib/md5.c lib/sha256.c lib/message-info.c lib/common-alg.c -lm -o $programName

# gcc md5-c.c sha256.c message-info.c -lm -o md5-c
# gcc md5-c.c -lm -o md5-c
# Run

# file="./res/no-pad-block.txt"
file="./res/abc.txt"
# file="./res/quick-brown-fox.txt"
# file="./res/two-pad-block.txt"
# file="./res/one-pad-block.txt"
# file="./res/full-block.txt"
# file="./res/lorem-ipsum.txt"
# file="./res/binary.bin"
# file="app-main"


./$programName $file --verbose --sha256 --output hash-output.txt
# ./$programName $file
# ./$programName $file --string "The quick brown fox jumps over the lazy dog" --output myfile.txt #--sha256  
# ./app-main --string "The quick brown fox jumps over the lazy dog" --output myfile.txt
# ./app-main $file --output hash-output.txt
# ./app-main --string "The quick brown fox jumps over the lazy dog"
# ./app-main --help

sha256sum=($(sha256sum $file))
md5sum=($(md5sum $file))

break="---------------------------------------------------------------------------------"

echo $break
echo -e "sha256sum:\t ${sha256sum}"
echo $break
echo -e "md5sum:\t\t ${md5sum}"
echo $break