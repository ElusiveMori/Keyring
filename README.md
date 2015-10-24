Keyring
-------

Purpose
=======

**Keyring** is a simple CUI (console user interface) cryptographic password manager for Windows, 
designed to securely store passwords in a file, using a passphrase to encode/decode them.

How it works
=======
All your data is stored in a pw.db file and encoded using the AES algorithm.  
The key for AES is generated using PBKDF2 from a passphrase with 10000 iterations, which, hopefully,
is fairly brute force resistant.
Passwords are grouped into categories. Each password entry consists of a username and a password attached to it.

Dependencies
=======
**Keyring** is written in C++11, and is meant to be ran on Windows.
It uses [CryptoPP](https://www.cryptopp.com/) for cryptography and [Boost](http://www.boost.org/) for file I/O.
Everything is statically compiled into the executable, so the executable is self-sufficient.
