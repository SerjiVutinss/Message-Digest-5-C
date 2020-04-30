clear

programName=hashC

gcc main.c lib/md5.c lib/sha256.c lib/message-info.c lib/common-alg.c -lm -o $programName

file="./res/abc.txt"
./$programName $file --string "The quick brown fox jumps over the lazy dog" --output myfile.txt

sha256sum=($(sha256sum $file))
md5sum=($(md5sum $file))

break="---------------------------------------------------------------------------------"

echo $break
echo -e "sha256sum:\t ${sha256sum}"
echo $break
echo -e "md5sum:\t\t ${md5sum}"
echo $break