# pushfile - an expect script to copy a file to a remote system

If you have Linux shell access to a remote Linux system and need to copy
a file to that system BUT the usual ways of doing this have been blocked
then the `pushfile` expect script could be the answer.

## Pre-requisites

Tcl and expect must be installed.

The file:

```
/usr/local/bin/expect
```

must be the expect executable or be a link to the expect executable.

Just about any version of Tcl and expect should work but if you encounter
problems then update to recent versions.

## Installing

Copy the `pushfile.exp` script to a directory in your PATH renaming
it to `pushfile` - something similar to:

```
cp pushfile.exp $HOME/bin
cd $HOME/bin
mv pushfile.exp pushfile
chmod a+x pushfile
```

## Example run

Below is an example to copy a file called `nile.csv` from a local system
called `swstore` to a remote system called `bridge`:

```
andyc@swstore:~$ cd $HOME/data
andyc@swstore:~/data$ ls -l nile.csv
-rw-r--r-- 1 andyc general 7996 Jul  5 13:04 nile.csv
andyc@swstore:~/data$ md5sum nile.csv
526463d766ed2c6dcca1b22020071f5e  nile.csv

andyc@swstore:~/data$ ssh andyc@bridge
andyc@bridge's password: 
Linux bridge 4.9.59-v7+ #1047 SMP Sun Oct 29 12:19:23 GMT 2017 armv7l
Last login: Thu Jul  5 13:11:00 2018 from 80.95.188.152
andyc@bridge:~$ cd $HOME/data
andyc@bridge:~/data$ ls -l nile.csv
ls: cannot access 'nile.csv': No such file or directory
andyc@bridge:~/data$ exit
Connection to bridge closed.

andyc@swstore:~/data$ pushfile nile.csv
spawn sh
$ PS1="sh$ "
sh$ PS2=""
sh$ stty erase "^H"
sh$ ssh andyc@bridge
andyc@bridge's password: 
Linux bridge 4.9.59-v7+ #1047 SMP Sun Oct 29 12:19:23 GMT 2017 armv7l
Last login: Thu Jul  5 13:12:08 2018 from 192.168.1.13
andyc@bridge:~$ cd $HOME/data
andyc@bridge:~/data$ 

Now sending file

    PS1="sh$ "
sh$ PS2=""
sh$ cp /dev/null nile.csv
sh$ echo '"Year", "Flood"' >>nile.csv
sh$ echo '   1,  9.9974' >>nile.csv
sh$ echo '   2, 10.5556' >>nile.csv
sh$ echo '   3,  9.9014' >>nile.csv
sh$ echo '   4, 11.4800' >>nile.csv
sh$ echo '   5, 12.8460' >>nile.csv
...
...
sh$ echo ' 566, 11.3840' >>nile.csv
sh$ echo ' 567,  9.8630' >>nile.csv
sh$ echo ' 568, 10.8450' >>nile.csv
sh$ echo ' 569, 10.4020' >>nile.csv
sh$ echo ' 570, 10.3060' >>nile.csv
sh$ 

File Sent - Back to you

sh$ exit
Connection to bridge closed.
sh$ exit
The /home/andyc/bin/pushfile expect script has finished

andyc@swstore:~/data$ ssh andyc@bridge
andyc@bridge's password: 
Linux bridge 4.9.59-v7+ #1047 SMP Sun Oct 29 12:19:23 GMT 2017 armv7l
Last login: Thu Jul  5 13:12:43 2018 from 192.168.1.13
andyc@bridge:~$ cd $HOME/data
andyc@bridge:~/data$ ls -l nile.csv
-rw-r--r-- 1 andyc general 7996 Jul  5 13:12 nile.csv
andyc@bridge:~/data$ md5sum nile.csv
526463d766ed2c6dcca1b22020071f5e  nile.csv
```

I first show the file details on the local system and generate
the MD5 checksum.

Next I login to the remote system and show the file does not (yet)
exist.  I logout back to my local system.

I then run the `pushfile` expect script to do the transfer (see
below for more details on how that works).

Finally I login to the remote system again and this time
show the file has been copied and has the correct MD5 checksum.

## How the `pushfile` expect script works

Change to the directory on the local system that contains the file to
be copied.

Invoke `pushfile` expect script with the name of the file to be copied.

The `pushfile` expect script starts a subshell and immediately hands
control back to you.

You now login to the remote system and change to the directory that the
file should be copied to.

Next you type the '+' (plus sign) character and this tells the `pushfile`
expect script to take back control.

The `pushfile` expect script then reconstructs the file line by line using
a series of shell echo commands.  Once this is complete then control is
passed back to you a second time.

You now issue the necessary shell commands to logout of the remote system
and finally issue an `exit` command to the subshell to finish the process.

The file is now transferred.

At this stage I recommend logging back into the remote system and performing
some sort of check that the file has been copied without error.  In
the example run above I used the `md5sum` command before and
after to do this verification.

## Isn't that very complicated?

A little complicated yes :-]  But if the only access you have
is a commnand line session with the remote system (in the above example
a secure shell session) then it might be your only option.

## Also, what about binary files?

If you have a binary file to copy (e.g. a compressed tar file) then
convert it to a text file first, transfer the text file using
the `pushfile` expect script and then convert it back on the remote
system to its original binary form.

If your local AND remote systems have the `uuencode` and `uudecode`
commands available then this is easy.  For example if the file
is a binary file called `release.tar.gz` then convert to a text
based format with:

```
uuencode < release.tar.gz > t.txt
```

Transfer the file `t.txt` to the remote system and then on the remote
system do:

```
uudecode < t.txt > release.tar.gz
```

Once you have confirmed the binary file has transferred successfully delete
the `t.txt` file from both ther local and remote system.

## uuencode and/or uudecode not available

If uuencode and/or uudecode are not available then one alternative is
to use a crude emulation of the technique.  The repository contains the
following files:

* binary2hex.c
* hex2binary.sh

On the local system compile the `binary2hex.c` C source and
place the exeutable in a directory in your path - something like:

```
gcc -o binary2hex binary2hex.c
mv binary2hex $HOME/bin
cd $HOME/bin
chmod a+z binary2hex
```

The binary2hex program reads binary content from standard input
and produces hexadecimal output on standard output.  For example:

```
cat /bin/date | binary2hex > /tmp/date.hex
head /tmp/date.hex
```

produces the following output:

```
7F454C46010101000000000000000000
0200280001000000F01A010034000000
ACC20000020400053400200009002800
1B001A000100007094BE000094BE0100
94BE0100080000000800000004000000
04000000060000003400000034000100
34000100200100002001000005000000
04000000030000005401000054010100
54010100190000001900000004000000
01000000010000000000000000000100
```

So if we transfer the date.hex file using the `pushfile` expect script
all we need do is convert this hexadecimal representaion back to binary.

Well the `hex2binary.sh` shell script can do this.  The script is very small
and reproduced below:

```
while read line
do
  n=`expr length $line`
  i=1
  while [ $i -lt $n ]
  do
    h1=`echo $line | cut -c$i`
    i=`expr $i + 1`
    h2=`echo $line | cut -c$i`
    i=`expr $i + 1`
    printf "\x$h1$h2"
  done
done
```

You could use the `pushfile` expect script to copy the `hex2binary.sh`
script to the remote system.  Or, as it is so short you could probably
cut and paste it from one terminal session to another.  Whatever is easier.

Once you have the script on the remote system you can
run something like:

```
./hex2binary.sh < date.hex > date
```

to recreate the binary file.  As before I recommend doing a MD5 checksum
before and after just to be sure.

## Warnings

Always do some form of data verification after the transfer.  I have used
the `md5sum` command but select whatever tool that is best for you.

The `hex2binary.sh` shell script is not very fast so be patient.

If you want faster conversion from hex to binary on the remote system
and have access on that system to a C compiler then look at the
`hex2binary.c` C program source in the repository.

## There must be a better way!

Congratulations on reading this far!  And yes there are always better ways.
One is to install C-Kermit on both the local and remote systems and
do the file transfer that way.  It's simpler and faster.

If the local and remote systems are running the same version of
operating system you could compile C-Kermit on your local machine
and then use the `pushfile` expect script to transfer the C-Kermit
binary executable to the remote system.  Try and do a statically linked
compile of C-Kermit to avoid library issues on the remote system.

---------------------------------------------------------------
