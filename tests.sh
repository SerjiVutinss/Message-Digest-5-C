./run.sh


# clear

outputFile=hashC

# gcc main.c lib/md5.c lib/sha256.c lib/message-info.c lib/common-alg.c -lm -o $outputFile

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

declare -a strings=(
    "helloworld"
    "LoremIpsumissimplydummytextoftheprinti ngandtypesettingindustry.LoremIpsumhasbeentheindustry'sstandarddummytexteversincethe1500s,whenanunknownprintertookagalleyoftypeandscrambledittomakeatypespecimenbook.Ithassurvivednotonlyfivecenturies,butalsotheleapintoelectronictypesetting,remainingessentiallyunchanged.Itwaspopularisedinthe1960swiththereleaseofLetrasetsheetscontainingLoremIpsumpassages,andmorerecentlywithdesktoppublishingsoftwarelikeAldusPageMakerincludingversionsofLoremIpsum."
    "TherearemanyvariationsofpassagesofLoremIpsumavailable,butthemajorityhavesufferedalterationinsomeform,byinjectedhumour,orrandomisedwordswhichdon'tlookevenslightlybelievable.IfyouaregoingtouseapassageofLoremIpsum,youneedtobesurethereisn'tanythingembarrassinghiddeninthemiddleoftext.AlltheLoremIpsumgeneratorsontheInternettendtorepeatpredefinedchunksasnecessary,makingthisthefirsttruegeneratorontheInternet.Itusesadictionaryofover200Latinwords,combinedwithahandfulofmodelsentencestructures,togenerateLoremIpsumwhichlooksreasonable.ThegeneratedLoremIpsumisthereforealwaysfreefromrepetition,injectedhumour,ornon-characteristicwordsetc."
)

# MD5 Files
for input in "${files[@]}"; do

    output=$(./$outputFile $input)
    hashResult=$(echo $output | rev | cut -d ' ' -f1 | rev)
    md5sum=($(md5sum $input))

    if [ "$hashResult" = "$md5sum" ]; then
        echo "   MD5 PASS $input"
    else
        echo "   MD5 FAIL $input"
    fi
done

# SHA256 Files
for input in "${files[@]}"; do

    output=$(./$outputFile $input --sha256)
    hashResult=$(echo $output | rev | cut -d ' ' -f1 | rev)
    sha256sum=($(sha256sum $input))

    if [ "$hashResult" = "$sha256sum" ]; then
        echo "SHA256 PASS $input"
    else
        echo "SHA256 FAIL $input"
    fi
done

# MD5 Strings
for input in "${strings[@]}"; do

    output=$(./$outputFile --string "$input")
    hashResult=$(echo $output | rev | cut -d ' ' -f1 | rev)
    md5sum=($(echo -n ${input} | md5sum))

    if [ "$hashResult" = "$md5sum" ]; then
        echo "   MD5 PASS '${input}'"
    else
        echo "   MD5 FAIL '${input}'"
    fi
done

# # SHA256 Strings
for input in "${strings[@]}"; do

    output=$(./$outputFile --string "$input" --sha256)
    hashResult=$(echo $output | rev | cut -d ' ' -f1 | rev)
    sha256sum=($(echo -n ${input} | sha256sum))

    if [ "$hashResult" = "$sha256sum" ]; then
        echo "SHA256 PASS '${input}'"
    else
        echo "SHA256 FAIL '${input}'"
    fi
done
