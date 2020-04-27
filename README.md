# Message-Digest-5-C
An implementation of the Message Digest 5 algorithm in C

## Introduction

The Message Digest 5 (MD5) algorithm is a hash function which produces a 128-bit value.

Originally designed as a cryptographic hash function, it has been found to have extensive vulnerabilities.  The hash still has its uses and is still widely used as a checksum to verify data integrity - but only against unintentional corruption. The hash should not be used to verify that a downloaded file is the expected file, since the algorithm is easily manipulated.