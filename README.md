# Message-Digest-5-C
An implementation of the Message Digest 5 algorithm in C.

## Project Brief

* Write a program in the C programming language that calculates the MD5 hash digest of an input. The algorithm is specified in the [Request For Comments 1321 document][RFC-1321] supplied by the [Internet Engineering Task Force][IETF].
* The program must perform the algorithm correctly  - it is up to the developer to decide which types of input the algorithm operates on, e.g. filename, string, url, etc.

## Background

The Message Digest (MD) algorithms are a family of [one way hash functions][One-Way-Hash-Functions]. A one way hash function, as described by [Professor Alan Kaminsky][One-Way-Hash-Functions]:

"maps an arbitrary-length input message M to a fixed-length output hash H(M) such that the following properties hold":

1. **One-way:** Given a hash H(M), it is difficult to find the message M.
2. **Second preimage resistant:** Given a message M1, it is difficult to find another message M2 such that H(M1) = H(M2).
3. **Collision resistant:** It is difficult to find two messages M1 and M2 such that H(M1) = H(M2).

Each of the algorithms in the MD family were designed by [Ronald Rivest][RonaldRivest], a cryptographer and professor at MIT. 

### History

| Year  | Description |
|:-     |:-|
| 1989 | [MD2][RFC-1319] developed and optimised for 8-bit computers. |
| 1990 | [MD4][RFC-1320] developed.
| 1991 | [MD5][RFC-1321] designed. designed Den Boer and Bosselaers demonstrate weaknesses of MD4 in a paper published in 1991.
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

## Uses of MD5

Originally designed as a [cryptographic hash function][CryptographicHashFunction], MD5 has been found to have extensive vulnerabilities as outlined above and as such it is no longer reliable for cryptographic purposes. The function still has its uses today and is often used as a checksum to verify data integrity. It should be noted that MD5 is unsuitable for use in the verification of tramsmitted data (as per [RFC-6151][RFC-6151]), but it is very useful for verifying that local data has not become corrupt.
    
* USE for verifying that local data has not become corrupt, e.g. hashing data before compression and recording its hash value means that after decompression, the hash of the decompressed data can then be compared against the original hash. If the hashes match, it is very likely that the data has not been altered during the compression or decompression operations.

* DO NOT USE for verifying the integrity of data after transmission or possible handling by a third party, e.g. via HTTP, stored on a USB device or network. Since collisions are now relatively easy to create, it is possible for a man in the middle to alter the data in such a way that the hash of the original and modified data match. This is a very serious consideration in relation to malware.

## The Algorithm

References:

[RonaldRivest]: https://en.wikipedia.org/wiki/Ronald_Rivest
* [Ronald Rivest - Wikipedia][RonaldRivest]

[RFC-1319]: https://tools.ietf.org/html/rfc1319
* [RFC-1319 - The MD2 Message-Digest Algorithm (ietf.org)][RFC-1319]

[RFC-1320]: https://tools.ietf.org/html/rfc1320
* [RFC-1320 - The MD4 Message-Digest Algorithm (ietf.org)][RFC-1320]

[RFC-1321]: https://www.ietf.org/rfc/rfc1321.txt
* [RFC-1321 - The MD5 Message-Digest Algorithm (ietf.org)][RFC-1321]

[RFC-6150]: https://tools.ietf.org/html/rfc6150
* [RFC-6150 - MD4 to Historic Status (ietf.org)][RFC-6150]

[RFC-6151]: https://tools.ietf.org/html/rfc6151
* [RFC-6151 - Updated Security Considerations for the MD5 Message-Digest and the HMAC-MD5 Algorithms (ietf.org)][RFC-6151]

[IETF]: https://www.ietf.org/
* [Internet Engineering Task Force][IETF]

[One-Way-Hash-Functions]: https://www.cs.rit.edu/~ark/lectures/onewayhash/onewayhash.shtml
* [Cryptographic One-Way Hash Functions - Prof. Alan Kaminsky, Department of Computer Science, Rochester Institute of Technology][One-Way-Hash-Functions]

[CryptographicHashFunction]: https://en.wikipedia.org/wiki/Cryptographic_hash_function
* [Cryptographic Hash Function][CryptographicHashFunction]


[OpenSSL]: https://www.openssl.org/
* [OpenSSL][OpenSSL]

[BirthdayAttack]: https://en.wikipedia.org/wiki/Birthday_attack
* [Birthday Attack][BirthdayAttack]

[MD5CRK]: https://en.wikipedia.org/wiki/MD5CRK
* [MD5CRK][MD5CRK]

[VlastimilKlima]: https://en.wikipedia.org/wiki/Vlastimil_Klima
* [Vlastimil Klima - The man who broke MD5 and SSL][VlastimilKlima]

[Marc_Stevens]: https://en.wikipedia.org/wiki/Marc_Stevens_(cryptology)
* [Marc Stevens - Cryptographer][Marc_Stevens]

[SEI]: https://en.wikipedia.org/wiki/CMU_Software_Engineering_Institute
* [Software Engineering Institute (SEI))][SEI]]

[FlameMalware]: https://en.wikipedia.org/wiki/Flame_(malware)
* [Flame (Malware)][FlameMalware]