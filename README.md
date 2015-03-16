# truecrack-extended
## What is this?

TrueCrack Extended is a client/server application for dictionary attacks on TrueCrypt volumes encrypted with AES-XTS and RIPEMD-160 hash algorithm. It was developed using C, CUDA and C++ with QT for the client/server part.

It is based on the open-source program "TrueCrack", developed by Luca Vaccaro. (freely available here: http://code.google.com/p/truecrack/)

TrueCrack Extended was developed with a focus on performance increase in comparision to the original TrueCrack. The program consists of the following 2 parts:

TrueCrack Optimized: This is basically the original TrueCrack, but with optimized CUDA-code and multi-GPU support. So in case the system has multiple CUDA-supported GPUs installed, TrueCrack optimized equally spreads the list of passwords over the GPUs.

TrueCrack Extended: This is the part that gave the project its name. It is a client/server program that gives TrueCrack scalability and thus a huge performance increase when used with multiple clients. You can easily set up a whole network to start calculating dictionary files on TrueCrypt containers with the combined computing power of all the clients. TrueCrack Extended uses the compiled binary of TrueCrack Optimized for the actual calculations.

## Working setup:
* Linux Kernel 2.6.x (e.g. Ubuntu 10.04 LTS)
* GCC-Compiler <= Version 4.4
* CUDA Toolkit 4.0 (May 2011)
* CUDA GPU Computing SDK
* NVIDIA Developer Driver 270.41.19
* Qt SDK Version 1.2

## Instructions:
1. First you have to compile TrueCrack Optimized, you can either compile it to run on CPU or GPU, though CPU-mode is very slow compared to GPU-mode!

  1. Compile for GPU: make GPU=true
  2. Compile for CPU: make GPU=false

2. Though TrueCrack Optimized can run on its own via terminal, we reccomend using it with the TrueCrack Extended. For that reason copy the compiled binary "truecrack" into the "TrueCrack"-folder you find in the TrueCrack_Extended root.

3. Now compile TrueCrack Extended by just using the QT-Creator included in the SDK. (Load the .pro file)

4. Put any TrueCrypt volume files under "HeaderFiles" and password files under "PasswordFiles". There should be an example file for both, the "ExampleHeader" is a usual TrueCrypt volume encrypted with AES and RIPEMD-160 using the password "12345". That is why the last password in the example_dictionary_1000k.txt is "12345".

5. TrueCrack Extended itself is pretty straight forward, you can setup the server using several terminal commands, or just start without any, it will ask wether to start client or server mode.

  1. Example start without commands: ./TrueCrack_Extended
  2. Example start as server: ./TrueCrack_Extended -s

6. When the server is configured and started, start a client with the parameter -c and input the server IP and ports. When compiled in GPU-Mode, make sure to start the client only from full terminal view (e.g. STRG+ALT+F1), otherwise you will get some unexpected errors.

  1. Example start as client: ./TrueCrack_Extended -c

#### Full terminal commands:
```
Terminal commands when using server-mode (-s):
Command    Description               Default
-hf PATH   PATH to Header-Folder     "HeaderFiles/"
-pf PATH   PATH to Password-Folder   "PasswordFiles/"
-cp INT    ControlPort               2000
-dp INT    DataPort                  2001
-b INT	   Blocksize              	 100000
-o FILE	   FILE for result output    result.txt
```
Example: ./TrueCrack_Extended -s -hf Data/Header/ -pf Data/Passwords/ -cp 23579 -dp 22485 -o outputFile.txt -b 100000

```
Terminal commands when using client-mode (-c):
Command          Description
-ip IP-Adress    IP-Adress of the server
-cp INT	         ControlPort of the server
-dp INT	         DataPort of the server
```
Example: ./TrueCrack_Extended -c -ip 192.168.10.168 -cp 23579 -dp 22485

7. Get crackin' :-)

For some test results check this Google Docs sheet: 
[Google Docs Sheet](https://docs.google.com/spreadsheet/ccc?key=0AlEw8DH466uzdFhaRlR6c1FDNkczMS11UHBGVXpjNUE&usp=sharing)

P.S.: Due to missing equipment the multi-GPU support is still untested, we would appreciate if anybody with a proper PC could send us his test results.
