Blowfish CUI tool
===================

Blowfish CUI tool.
Fork from [Blowfish](https://github.com/h2so5/Blowfish).

[![image](http://i.creativecommons.org/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)

 * Tested on Mac OS X (10.9.4 / 64bit)


Install & Build
-------------------
```sh
$ git clone https://github.com/mitsuaki-n/Blowfish
$ cd Blowfish
$ make
>>> generated 'blowfish'
```

Synopsis
-------------------
blowfish file_path encription_key [-o output_file_path] [-e | -d]
 * file_path --- Path to data file you want to encrypted.
 * encription_key --- Encryption key.
 * -o flag --- If you want specify output-file-path than use this flag.
 * -e, -d flag --- -e is Encrypt Mode(Default). -d is Decrypt Mode.


Usage
-------------------

### Encryption Mode ###
```sh
$ blowfish datafile.dat encryption_key
Succeeded! "data.txt" --[Encryption(Key:encryption_key)]--> "data.txt.bfe"

$ blowfish datafile.dat encryption_key -o datafile.dat
Succeeded! "datafile.dat" --[Encryption(Key:encryption_key)]--> "datafile.dat"
```

### Description Mode ###
```sh
$ blowfish encryptedfile.bfe encryption_key
Succeeded! "encryptedfile.bfe" --[Decryption(Key:encryption_key)]--> "encryptedfile.bfe.bfd"

$ blowfish encryptedfile.dat encryption_key -o encryptedfile.dat -d
Succeeded! "encryptedfile.dat" --[Encryption(Key:encryption_key)]--> "encryptedfile.dat"
```
