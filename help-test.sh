clear
# Added link flag for math.h
# gcc main.c sha256.c lib/md5-alg.c lib/message-info.c lib/message-block.c -lm -o app-main

gcc main.c lib/md5.c lib/sha256.c lib/message-info.c lib/common-alg.c -lm -o app-main

./app-main --help