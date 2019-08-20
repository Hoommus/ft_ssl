*Just gonna pretend anyone will ever need this*

*Yeah, I'm aware that you shouldn't implement cryptographic algorithms on your own from scratch*

## Overview
**ft_ssl** is a project at School 42 the goal of which is actually to 
implement some cryptographic... things — hashing, symmetric encoding, 
encryption and private-public key pairs generation.

At this point the project contains just three **hashing** algorithms:
**MD5**, **SHA256** and **Whirlpool**.

## Usage
After the compilation you'll get a binary named **ft_ssl**. It works somewhat
similar to the [OpenSSL Toolkit ](https://www.openssl.org/). The supported
commands are: `md5`, `sha256` and `whirlpool`

Each of which supports the following options: 
* `-r` — reverse output. Puts the name of the file that was hashed after
the digest separated by single space and removes the name of the hashing
 algorithm.
* `-q` — quiet output. Suppresses any messages regarding the name 
of the algorithm and the name of file that was hashed.
* `-p` — read input from stdin and echo it to stdout with the hash separated
by single newline. 
* `-s [string]` — uses `string` as an argument for the command instead of file.

If program was launched without arguments, it will read commands from
stdin in shell mode. And if command does not have any parameters provided,
it will read data from stdin.

If the command was provided, the program will read
input data from — you guessed it — stdin and use it as data for the 
specified command.

## Build
Just clone the repo somewhere and `make` it. **ft_ssl** does not depend on
anything except C standard library. It was developed specifically for macOS
and uses `clang` compiler, so it probably won't compile on Linux or 
virtually anything else.

<br>
<br>

*...why did I even push this to Github, this project is just disgusting...*
