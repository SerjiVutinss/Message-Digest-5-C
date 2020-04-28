clear
# Added link flag for math.h
# gcc main.c sha256.c lib/md5-alg.c lib/message-info.c lib/message-block.c -lm -o app-main

gcc main.c lib/md5.c lib/sha256.c lib/message-info.c lib/common-alg.c -lm -o app-main

# gcc md5-c.c sha256.c message-info.c -lm -o md5-c
# gcc md5-c.c -lm -o md5-c
# Run

# MD5 tests

# all files

## declare an array variable
declare -a files=(
    "./res/no-pad-block.txt"
    "./res/one-pad-block.txt"
    "./res/two-pad-block.txt"
    "./res/full-block.txt"
    "./res/abc.txt"
    "./res/quick-brown-fox.txt"
    "./res/lorem-ipsum.txt"
    "./res/binary.bin"
)



# ## now loop through the above array
# for i in "${files[@]}"
# do
#     file = $i
#     result=$(./app-main ${i})
#     (./app-main $file --verbose --md5)

#     hash=$(echo $result | rev | cut -d ' ' -f1 | rev)
#     echo "$hash"
#    # or do whatever with individual element of the array
# done



# file="./res/no-pad-block.txt"
file="./res/abc.txt"
# file="./res/quick-brown-fox.txt"
# file="./res/two-pad-block.txt"
# file="./res/no-pad-block.txt"
# file="./res/full-block.txt"
# file="./res/lorem-ipsum.txt"
# file="./res/binary.bin"
# file="app-main"


# ./app-main $file --verbose --sha256
result="$(./app-main res/abc.txt)"
# ./app-main $file --sha256  --string "The quick brown fox jumps over the lazy dog" --output myfile.txt
# ./app-main --string "The quick brown fox jumps over the lazy dog" --output myfile.txt --sha256  
# result=$(./app-main --string "The quick brown fox jumps over the lazy dog" --output myfile.txt)
# ./app-main --string "The quick brown fox jumps over the lazy dog"
# ./app-main --help

echo "$result"

hash=$(echo $result | rev | cut -d ' ' -f1 | rev)

echo $hash


sha256sum=($(sha256sum $file))
md5sum=($(md5sum $file))

if [ "$hash" = "$md5sum" ]; then
    echo "PASS"
else
    echo "FAIL"
fi

echo "------------------------"
echo "SHA256: ${sha256sum}"
echo "------------------------"
echo "   MD5: ${md5sum}"
echo "------------------------"