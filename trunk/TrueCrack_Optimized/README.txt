TRUECRACK OPTIMIZED
TrueCrack is a bruteforce password cracker for TrueCrypt (Copyrigth) volume.
It is optimized with NVIDIA Cuda technology and even has multi-GPU support.
It works with PBKDF2 (defined in PKCS5 v2.0) based on RIPEMD160 Key derivation 
function and XTS block cipher mode of operation used for hard disk encryption 
based on AES.

LICENSE
TrueCrack is an Open Source Software under GNU Public License version 3.
This software is Based on TrueCrypt, freely available at http://www.truecrypt.org/

AUTHOR
The authors are Bastian Blankemeier, Henryk Jaskowiak and Benjamin Meis.
This program was made in a project at the Ruhr-University Bochum and is based 
on the open-source program TRUECRACK freely available at http://code.google.com/p/truecrack/


COMPILE
The software can work on CPU or GPU. 
If you want the Cuda optimization, you set the GPU variable on true:
   make GPU=true
Else if you want use only the CPU resource, you set the GPU variable on false:
   make GPU=false
