clear
programName=hashC
gcc main.c lib/md5.c lib/sha256.c lib/message-info.c lib/common-alg.c -lm -o $programName