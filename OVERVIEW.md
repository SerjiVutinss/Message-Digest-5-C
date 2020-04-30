# Message-Digest-5-C
An implementation of the Message Digest 5 algorithm in C.

## Project Brief

* Write a program in the C programming language that calculates the MD5 hash digest of an input. The algorithm is specified in the [Request For Comments 1321 document][RFC-1321] supplied by the [Internet Engineering Task Force][IETF].
* The program must perform the algorithm correctly  - it is up to the developer to decide which types of input the algorithm operates on, e.g. filename, string, url, etc.

---

## Downloading and Running the Program

### **Prerequisities**
* **Windows** - The best option is likely to use [Cygwin](https://www.cygwin.com/) or [MinGW](http://www.mingw.org/) to provide GNU tools for Windows. Alternatively, if Visual Studio or CLion are present, the repo may be imported to these IDEs.

* **Linux** - The GNU C Compiler (GCC) and associated libraries are required. These packages vary by Linux distribution but can generally be installed by using the following command depending on distribution type:
    * Debian/Ubuntu based: `$ sudo apt update && sudo apt install build-essential`
    * Red Hat/Fedora based: `# yum groupinstall 'Development Tools'`
    * Arch based:  `$ pacman -S base-devel`

### **Clone and Build**

1. Clone this repo: 
    * SSH: `$ git clone git@github.com:SerjiVutinss/Message-Digest-5-C.git`
    * HTTPS: `$ git clone https://github.com/SerjiVutinss/Message-Digest-5-C.git`

2. Build:
    * Use the provided script: `$ chmod a+x build.sh && ./build.sh`
    * Build manually: `$ gcc main.c lib/md5.c lib/sha256.c lib/message-info.c lib/common-alg.c -lm -o hashC`

### **Run the Program**
Execute the program with the `--help` to display the following which includes some examples of how to run the program.

    Usage:
        File:   hashC [FILE] [OPTION]...
        String: hashC --string [STRING] [OPTION]

    Calculate and print out the MD5(default) or SHA256 hash of the input FILE or STRING.
    NOTE: If string hashing is to be used, --string MUST be the first argument

        --string [STRING]   Use the next argument as input. --string MUST be the first argument passed if it is used.
                            If the string includes whitespace, it should be surrounded by quotes.
    
        --sha256            Use the SHA256 algorithm to calculate the hash.
    
        --output [OUTPUT]   Write the hash to the file, [OUTPUT]. MUST be followed by the output filename.
    
        --verbose           Print detailed hash information to the screen.
    

    Example usage:
        $ hashC input.txt - hash the contents of input.txt using MD5 and print the hash to the screen. 
        $ hashC --string \"hello world\" - hash the string \"hello world\" and print the hash to the screen. 

        $ hashC input.txt --sha256 - hash the contents of input.txt using SHA256 and print the hash to the screen. 
        $ hashC input.txt --output output.txt - hash the contents of input.txt, print the hash to the screen and also write it to the file output.txt.

---

### Example Outputs

In this section I have provided two basic examples of verification of the program's output against the Linux command line tool `md5sum`. This section is only for demonstration purposes and I have provided far more detail of the overall program testing in the next section.

Command: `$ ./hashC ./res/two-pad-block.txt --output hash-output.txt`


    Output:

    Opening file: ./res/two-pad-block.txt
    Successfully opened file: ./res/two-pad-block.txt
    Using algorithm: MD5
    Verbose mode: Disabled
    Output mode: Enabled
    Output file: hash-output.txt
    STARTING MD5 HASH

    HASH: f3bab8014e99a33546458a7dd088c396

Verification command: `$ md5sum ./res/two-pad-block.txt`

    Verification Output: f3bab8014e99a33546458a7dd088c396

---
Command: `./hashC --string "The quick brown fox jumps over the lazy dog" --output hash-output.txt`

    Output:

    Using algorithm: MD5
    Verbose mode: Disabled
    Output mode: Enabled
    Output file: myfile.txt
    STARTING MD5 HASH

    HASH: 9e107d9d372bb6826bd81d3542a419d6

Verification command: `echo -n "The quick brown fox jumps over the lazy dog" | md5sum`

    Verification Output: 9e107d9d372bb6826bd81d3542a419d6
---

## Tests - [tests.sh]

Rather than building the tests into my program in C, I opted to use a script to automate the testing of the programs's output against known good values for the messages provided as inputs to the program. As this program was developed in a Linux environment, I created a Bash script to compare the hash results. A similar script could easily be written for Windows in PowerShell should the need arise.

I have broken the test script into two distinct sections - Files and Strings. Files uses filenames as input and Strings uses just string values as input. 

The `/res` folder of this repository contains a number of files used for testing. A number of these files (e.g. `no-pad-block.txt`, `full-block.txt`, etc ) are designed to test the algorithm on the boundaries of its determination and creation of block sizes. For example, `full-block.txt` is 64 bytes in size and `one-pad-block.txt` is 56 bytes in size. I have also included a small binary file (`binary.bin`) which is a very early build of this program.

The flow of the test script is as follows:

### Files

1. Create an array of filenames.
2. For each `filename` in filenames, run this program using the filename: '`hashC filename`'.
3. Store the output of this program as a variable `output`.
4. Echo `output` to stdout and pipe it to '`rev`', '`cut -d ' ' -f1`' and '`rev`' respectively:
    1. `rev` - reverses the output, meaning that the hash result text is now at the beginning of the string, although reversed.
    2. `cut -d ' ' -f1` - splits the output on the space character and then takes the first element, which is the reversed hash result.
    3. `rev` - since the hash result is reversed, reverse it again to restore it to its original ordering.
5. Store the result of the previous step as the variable `hashResult`.
6. Run the md5sum Linux command line tool: '`md5sum filename`' and store its output as `md5sum`.
7. Compare `hashResult` to `md5sum` - if they are equal, the test has passed, otherwise it has failed.

### Strings

Similar to testing files, although the inputs to both this program and the md5sum command line tool are slightly different:

1. Create an array of strings.
2. For each `str` in strings, run this program using the string: '`hashC --string "str"`'.
3. Store the output of this program as a variable `output`.
4. Echo `output` to stdout and pipe it to '`rev`', '`cut -d ' ' -f1`' and '`rev`' respectively:
    1. `rev` - reverses the output, meaning that the hash result text is now at the beginning of the string, although reversed.
    2. `cut -d ' ' -f1` - splits the output on the space character and then takes the first element, which is the reversed hash result.
    3. `rev` - since the hash result is reversed, reverse it again to restore it to its original ordering.
5. Store the result of the previous step as the variable `hashResult`.
6. Echo the string to stdout and pipe it to the md5sum Linux command line tool: '`echo -n str | md5sum`' and store its output as `md5sum`.
7. Compare `hashResult` to `md5sum` - if they are equal, the test has passed, otherwise it has failed.





## Introduction and Background of MD5 Algorithm

The Message Digest (MD) algorithms are a family of [one way hash functions][One-Way-Hash-Functions]. A one way hash function, as described by [Professor Alan Kaminsky][One-Way-Hash-Functions]:

"maps an arbitrary-length input message M to a fixed-length output hash H(M) such that the following properties hold":

1. **One-way:** Given a hash H(M), it is difficult to find the message M.
2. **Second preimage resistant:** Given a message M1, it is difficult to find another message M2 such that H(M1) = H(M2).
3. **Collision resistant:** It is difficult to find two messages M1 and M2 such that H(M1) = H(M2).

Each of the algorithms in the MD family were designed by [Ronald Rivest][RonaldRivest], a cryptographer and professor at MIT. 

### History of Message Digest Algorithms

| Year  | Description |
|:-     |:-|
| 1989 | [MD2][RFC-1319] developed and optimised for 8-bit computers. |
| 1990 | [MD4][RFC-1320] developed.
| 1991 | [MD5][RFC-1321] designed. Den Boer and Bosselaers demonstrate weaknesses of MD4 in a paper published in 1991.
| 1992 | [RFC-1321][RFC-1321] published by Rivest, specifying the MD5 algorithm.
| 1995 | The first full-round MD4 collision attack was found by Hans Dobbertin, which took only seconds to carry out at that time. A preimage attack is theorised as being possible.
| 1996 | Collision of MD5 compression function announced. While not an attack on the complete MD5 function, cryptographers recommend switching to an alternative.
| 2004 | MD2 shown to be vulnerable to a preimage attack with time complexity equivalent to 2^104 applications of the compression function (Muller, 2004). The author concludes, "MD2 can no longer be considered a secure one-way hash function". The MD5CRK distributed project is started with the aim of demonstrating that MD5 is practically insecure by finding a collision using a [birthday attack][BirthdayAttack]. [MD5CRK][MD5CRK] later announces collisions and their analytical attack reportedly took one hour on their hardware cluster.
| 2005/06 | Further exploits found in MD5. [Vlastimil Klima][VlastimilKlima] publishes an algorithm that can find an MD5 collision in one minute on a laptop, using a method he calls tunneling.
| 2008 | Preimage resistance of MD4 broken by Gaëtan Leurent, with a 2^102 attack. Time complexity later improved to 2^99.7 in 2010.
| 2009 | Time complexity of MD2's vulnerability is reduced to 2^63.3 compression function evaluations. Security updates were issued disabling MD2 in many software applications, most notably [OpenSSL][OpenSSL]
| 2010 | Tao Xie and Dengguo Feng announce the first published single-block (512-bit) MD5 collision. Not disclosing the attack method, they issue a challenge to the cryptographic community, offering a USD$10,000 reward to the first finder of a different 512-bit collision before 1/1/2013. Marc Stevens responds with numerous single-block messages along with the algorithm he used.
|| The [Software Engineering Institute (SEI)][SEI] considers MD5 "cryptographically broken and unsuitable for further use"
| 2011 | [RFC-6150][RFC-6150] published by the IETF stating that MD4 is historic and obsolete.
|  | [RFC-6151][RFC-6151] published by the IETF updating security considerations for the use of MD5 - "The published attacks against MD5 show that it is not prudent to use MD5 when collision resistance is required."
| 2012 | [Flame malware][FlameMalware] exploits the weaknesses in MD5 to fake a Microsoft digital signature.


### Comparison of Message Digest Algorithms (SHA256 also included)
##### (All figures represent sizes in bits unless otherwise stated)

| Algorithm | Output    | Internal State | Block | Word | # Rounds
|:-         |:-         |:-              |:-     |:-    |:-
| MD2       | 128       | 384            | 128   | 32   | 18
| MD4       | 128       | 128            | 512   | 32   | 3
| MD5       | 128       | 128            | 512   | 32   | 64
| SHA256    | 256       | 256            | 512   | 32   | 64

#### Notes
* MD2 was considered a slow algorithm which can be attributed mainly to the fact that it used a large internal state relative to the hardware available at the time.
* MD4 was generally faster than MD2 but its vulnerabilities were quickly uncovered by researchers and members of the cryptographic community.
* In response to MD4's apparent weaknesses, Rivest designs MD5 which is more complex and difficult to break. The rest is history.

---

## Uses of MD5

Originally designed as a [cryptographic hash function][CryptographicHashFunction], MD5 has been found to have extensive vulnerabilities as outlined above and as such it is no longer reliable for cryptographic purposes. The function still has its uses today and is often used as a checksum to verify data integrity. It should be noted that MD5 is unsuitable for use in the verification of tramsmitted data (as per [RFC-6151][RFC-6151]), but it is very useful for verifying that local data has not become corrupt.
    
* USE for verifying that local data has not become corrupt, e.g. hashing data before compression and recording its hash value means that after decompression, the hash of the decompressed data can then be compared against the original hash. If the hashes match, it is very likely that the data has not been altered during the compression or decompression operations.

* DO NOT USE for verifying the integrity of data after transmission or possible handling by a third party, e.g. via HTTP, stored on a USB device or network. Since collisions are now relatively easy to create, it is possible for a man in the middle to alter the data in such a way that the hash of the original and modified data match. This is a very serious consideration in relation to malware.

---
---

# The MD5 Algorithm

## Terminology

In the previous comparison of a number of hashing algorithms, it could be seen that MD5 uses certain bit sizes for the core components of its algorithm.

| Algorithm | Output    | Internal State | Block | Word | # Rounds
|:-         |:-         |:-              |:-     |:-    |:-
| MD5       | 128       | 128            | 512   | 32   | 64

**Output:**
The hash value produced by the algorithm which will always be 128 bits in length. In MD5 this is actually the modified initial internal state which is returned once all operations have been performed on it.

**Internal State:**
This four-word buffer, composed of four 32-bit registers (A, B, C and D), should be initialised to the following hexadecimal values ([md5.h] line 63):

<center>

|Word   |Little Endian  | Big Endian    |
|:-:    |:-             |:-             |
|A      |01 23 45 67    |67 45 23 01    |
|B      |89 ab cd ef    |ef cd ab 89    |
|C      |fe dc ba 98    |98 ba dc fe    |
|D      |76 54 32 10    |10 32 54 76    |

</center>

**Block**
The input message is broken into chunks (blocks) of 512 bits in size. More on this in the Padding section.

**Word**
As outlined above, the internal state is maintained as a four-word buffer, i.e. 4 x 32-bit words to give an internal state size of 128 bits.

**Rounds**
The number of rounds of calculations to be peformed in the algorithm. 64 rounds are used here although it can also be thought of as 4 stages of 16 similar operations.

---

## Algorithm Steps

### **Step 1 and Step 2 - Append Padding Bits and Length ([RFC-1321][RFC-1321] Section 3.1 & 3.2)**

Since each block must be 512 bits in length, the original message may need to be padded so that its length is congruent to 448, modulo 512, i.e the entire length of the message should be padded to 64 bits short of being a multiple of 512 bits. Depending on the original length of the message, this may result in the need to add a new, fully padded block to the message.

Different approaches to padding could be taken, e.g. the entire message could be pre-processed to determine total number of blocks and how they should be padded. In my implementation, I have performed padding on-the-fly to reduce the space complexity of the algorithm with no real impact on the time complexity.

(md5.c processNextMD5Block())

```vb
1. "Record the length of the original message."

    var messageLength = number of bits in original message represented as a 64-bit number**

2. "Append a single bit (one-bit) to the message."

    append one-bit (1) to the message

3. "Break the message up into blocks of 512 bits."

4. "Record the length of the last 512-bit block."

    var lastBlockLength = number of bits used in the last 512-bit block.

5. "Depending on the length of 'lastBlockLength', do one of the following:"

    If lastBlockLength < 448 bits Then 
        
        pad with zero-bits until its length == 448 bits

        append `b` to the last block 
        
    End If

    If lastBlockLength == 448 bits Then append `b` to the last block End If
    
    If lastBlockLength > 448 AND messageLength < 512 bits Then
        
        append zero-bits (0) to last block until its length is 512 bits
        
        create a new block and pad with zero-bits until its length is 448 bits

        append 'b'

    End If
```
**In the unlikely event that b is greater than 2^64, then only
   the low-order 64 bits of b are used. These bits are appended as two
   32-bit words and appended low-order word first in accordance with the
   previous conventions. ([RFC-1321][RFC-1321] Section 3.2)

### **Step 3 - Initialise the Message Digest Buffer ([RFC-1321][RFC-1321] Section 3.3)**

As described earlier in the [Internal State section](#Internal-State), each word in this four-word buffer should be initialised to constant values.

In my implementation of the algorithm I have used an alias for `uint32_t` which I have named `WORD`. Using this alias, I have created a `WORD` array named `H_MD5` to initialise and hold the internal state.

I have initialised `H_MD5` as follows (according to [RFC-1321][RFC-1321] Section 3.3):
    
        WORD H_MD5[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

### **Step 4 - Process Message in 16-Word Blocks ([RFC-1321][RFC-1321] Section 3.4)**

**Functions F, G, H and I:**
These auxiliary functions, each taking three 32-bit words as inputs and producing one 32-bit word as their outputs must be defined. RFC-1321 defines these functions as:

    F(X,Y,Z) = XY v not(X) Z
    G(X,Y,Z) = XZ v Y not(Z)
    H(X,Y,Z) = X xor Y xor Z
    I(X,Y,Z) = Y xor (X v not(Z))

I have implemented these methods using the standard C bit operators and using WORD as an alias for the numeric `uint32_t` type available in the `inttypes.h` library. (For actual implementations, please refer to lines 99-120 of [md5.h].

    WORD F(WORD x, WORD y, WORD z) => ((x & y) | ((~x) & z));
    WORD G(WORD x, WORD y, WORD z) => ((x & z) | (y & (~z)));
    WORD H(WORD x, WORD y, WORD z) => (x ^ y ^ z);
    WORD I(WORD x, WORD y, WORD z) => (y ^ (x | (~z)));

**Functions FF, GG, HH and II:**
Additionally, not specified in RFC-1321, it is useful to create the functions FF, GG, HH and II which provide a means for calling each of the above 4 functions in a consistent fashion.  I have implemented each of these methods and call them from the function to process each block, rather than calling F, G, H or I directly. In my opinion this greatly increases the readability of the code, debugging and resilience to errors. The functions could each be reduced to one line of code but I have intentionally left the code verbose for increased readability.

**It should be noted that the first argument should be a pointer to a WORD, rather than an actual WORD.

    FF(WORD *a, WORD b, WORD c, WORD d, WORD k, WORD s, WORD ac)
    {
        WORD sum = (*a + F(b, c, d) + k + ac);
        *a = b + ROTL(sum, s);
    }

**Rotate Left:** Not specified in RFC-1321, this is a useful method which rotates the bits in the input Word x, left by c bits. Note that c does not need to be a word, but for consistency I have used the same type as x. ([md5.h], lines 128-130)

    WORD ROTL(WORD x, WORD c) =>  (((x) << (c)) | ((x) >> (32 - (c))));

**Round-Shift Amounts (S):** Specified in the description of each round in RFC-1321, I have used an array for these values. They determine how many bits are to be shifted in each of the algorithm's rounds. They are used solely as an input argument to the Rotate Left function.

#### Process Message in 16-word (512-bit) Blocks

Using the functions and values outlined above, RFC-1321 states to do the following with each message block:

* Break each block into 16 32-bit chunks (Words), e.g. M[16];

* Initialise values for this hash using the values described for H_MD5 in the Internal State in the [Terminology section](#Terminology) of this document.  For the first block, I have initialised the variables A, B, C and D to the initial values of the H_MD5 array. This is done via the nextMD5Hash() ([md5.c] line 104) method. This method is first called by passing in the H_MD5 array, which results in this:

        WORD A = H_MD5[0];
        WORD B = H_MD5[1];
        WORD C = H_MD5[2];
        WORD D = H_MD5[3];

* Now perform the 4 rounds of 16 calculations. I have implemented this as a single block of 64 method calls, but it is also possible to perform these calculations within a loop if so desired and can be done very nicely with function pointers. For this implementation, I have left the code intentionally verbose to increase readability. Also, RFC-1321 describes this portion of the algorithm in 64 distinct operations so I have kept with that methodology.  Rather than adding all of the code here, it can be seen in this repo in [md5.c] lines 125-194.

* Once all round calculations have been performed, the initial values of H_MD5 are now incremented by the values A, B, C and D respectively. The value of H_MD5 at this point is the current hash value. If the message consists of multiple blocks, this new value of H_MD5 is then used as an initialiser to calculate the next block's hash which is again added to H_MD5, and so on until no blocks remain. If this is a single-block hash, this is the result of the hash.

---

## Complexity

---
##### References:

[RonaldRivest]: https://en.wikipedia.org/wiki/Ronald_Rivest
###### [Ronald Rivest - Wikipedia][RonaldRivest]

[RFC-1319]: https://tools.ietf.org/html/rfc1319
###### [RFC-1319 - The MD2 Message-Digest Algorithm (ietf.org)][RFC-1319]

[RFC-1320]: https://tools.ietf.org/html/rfc1320
###### [RFC-1320 - The MD4 Message-Digest Algorithm (ietf.org)][RFC-1320]

[RFC-1321]: https://www.ietf.org/rfc/rfc1321.txt
###### [RFC-1321 - The MD5 Message-Digest Algorithm (ietf.org)][RFC-1321]

[RFC-6150]: https://tools.ietf.org/html/rfc6150
###### [RFC-6150 - MD4 to Historic Status (ietf.org)][RFC-6150]

[RFC-6151]: https://tools.ietf.org/html/rfc6151
###### [RFC-6151 - Updated Security Considerations for the MD5 Message-Digest and the HMAC-MD5 Algorithms (ietf.org)][RFC-6151]

[IETF]: https://www.ietf.org/
###### [Internet Engineering Task Force][IETF]

[One-Way-Hash-Functions]: https://www.cs.rit.edu/~ark/lectures/onewayhash/onewayhash.shtml
###### [Cryptographic One-Way Hash Functions - Prof. Alan Kaminsky, Department of Computer Science, Rochester Institute of Technology][One-Way-Hash-Functions]

[CryptographicHashFunction]: https://en.wikipedia.org/wiki/Cryptographic_hash_function
###### [Cryptographic Hash Function][CryptographicHashFunction]


[OpenSSL]: https://www.openssl.org/
###### [OpenSSL][OpenSSL]

[BirthdayAttack]: https://en.wikipedia.org/wiki/Birthday_attack
###### [Birthday Attack][BirthdayAttack]

[MD5CRK]: https://en.wikipedia.org/wiki/MD5CRK
###### [MD5CRK][MD5CRK]

[VlastimilKlima]: https://en.wikipedia.org/wiki/Vlastimil_Klima
###### [Vlastimil Klima - The man who broke MD5 and SSL][VlastimilKlima]

[Marc_Stevens]: https://en.wikipedia.org/wiki/Marc_Stevens_(cryptology)
###### [Marc Stevens - Cryptographer][Marc_Stevens]

[SEI]: https://en.wikipedia.org/wiki/CMU_Software_Engineering_Institute
###### [Software Engineering Institute (SEI)][SEI]

[FlameMalware]: https://en.wikipedia.org/wiki/Flame_(malware)
###### [Flame (Malware)][FlameMalware]

[md5.h]: https://github.com/SerjiVutinss/Message-Digest-5-C/blob/master/lib/md5.h
[md5.c]: https://github.com/SerjiVutinss/Message-Digest-5-C/blob/master/lib/md5.c
[tests.sh]: https://github.com/SerjiVutinss/Message-Digest-5-C/blob/master/tests.sh