# Message-Digest-5-C
An implementation of the Message Digest 5 algorithm in C.

This document only relates to the downloading, compilation and running of the program. For more detailed documentation concerning the MD5 algorithm, its history, uses and implementation in this program, please see the !!OVERVIEW.md document!!

### Project Brief

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

### **Run**
Execute the program with the `--help` to display the following which includes some examples of how to run the program.

    Usage:
        File:   hashC [FILE] [OPTION]...
        String: hashC --string [STRING] [OPTION]

    Calculate and print out the MD5(default) or SHA256 hash of the input FILE or STRING.
    NOTE: If string hashing is to be used, --string MUST be the first argument

        --string [STRING]   Use the next argument as input. --string MUST be the first argument passed if it used.
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

## Example Outputs

In this section I have provided two basic examples of verification of the program's output against the Linux command line tool `md5sum`. This section is only for demonstration purposes and I have provided far more detail of the overall program testing in the OVERVIEW.md document.

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