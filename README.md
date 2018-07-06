# pushfile - an expect script to copy a file to a remote system

The `pushfile` expect scipt can be used to copy a file
from a local UNIX/Linux system to a remote UNIX/Linux system
provided the local system can establish a command line
shell on the remote system.

The way the command line shell is established could be as simple
as:

* Using ssh to login to the remote system

or it could be as convoluted as:

* Establish a VPN to a remote site
* Login to a bridge head system
* From the bridge head login to a second system
* From the second system run a cu serial session to login to a third system

Simple or convoluted the `pushfile` expect script will be able to copy
a file.

## Pre-requisites

Tcl and expect must be installed on the local system.

Just about any version of Tcl and expect should work but if you encounter
problems then update to recent versions.

The file:

```
/usr/local/bin/expect
```

on the local system must be the expect executable or be
a link to the expect executable.

## Installing

Copy the `pushfile.exp` script to a directory on your local system in
your PATH renaming it to `pushfile` - something similar to:

```
cp pushfile.exp $HOME/bin
cd $HOME/bin
mv pushfile.exp pushfile
chmod a+x pushfile
```

The `binary2hex.c` C program source must be compiled on the local system
and the resulting executable put in a directory in your PATH - something
similar to:

```
gcc -o $HOME/bin/binary2hex binary2hex.c
cd $HOME/bin
chmod a+x binary2hex
```

Finally login to the remote system and create a shell script called
`hex2binary` using commands similar to:

```
cd $HOME/bin
cp /dev/null hex2binary
echo 'while read line' >> hex2binary
echo 'do' >> hex2binary
echo '  n=`expr length $line`' >> hex2binary
echo '  i=1' >> hex2binary
echo '  while [ $i -lt $n ]' >> hex2binary
echo '  do' >> hex2binary
echo '    h1=`echo $line | cut -c$i`' >> hex2binary
echo '    i=`expr $i + 1`' >> hex2binary
echo '    h2=`echo $line | cut -c$i`' >> hex2binary
echo '    i=`expr $i + 1`' >> hex2binary
echo '    printf "\x$h1$h2"' >> hex2binary
echo '  done' >> hex2binary
echo 'done' >> hex2binary
chmod a+x hex2binary
```

## Example run

Below is an example run to copy a file called `software-1-2-3.tar.gz` from
a local system called `swstore` to a remote system called `bridge`:

```
andyc@swstore:~$ cd $HOME/data
andyc@swstore:~/data$ ls -l
total 12
-rw-r--r-- 1 andyc general 8970 Jul  5 17:52 software-1-2-3.tar.gz
andyc@swstore:~/data$ md5sum software-1-2-3.tar.gz
87e7d5fcbe3def4e1a14cd995c4cca4a  software-1-2-3.tar.gz
andyc@swstore:~/data$ ssh andy@bridge
andyc@bridge's password: 
Linux bridge 4.9.59-v7+ #1047 SMP Sun Oct 29 12:19:23 GMT 2017 armv7l
Last login: Fri Jul  6 16:01:11 2018 from 192.168.1.99
andyc@bridge:~$ cd $HOME/data
andyc@bridge:~/data$ ls -l
total 0
andyc@bridge:~/data$ exit
Connection to bridge closed.
andyc@swstore:~/data$ binary2hex < software-1-2-3.tar.gz > h.hex
andyc@swstore:~/data$ head h.hex
1F8B080841193D5B0003736F66747761
72652D312D322D332E74617200EC5B0B
781CD5759E952C3FD64696790463537C
F103E4C7AE66666767773106CDBEBC22
B2516C8938356235DA9DD50E5ECDA833
B396C423313526A88E89D390D47DD0BA
4943094D528736094D1330718221A5D4
24D0D2128AA121B579C514429C12AC9E
7BE7DED5EE7885C9F719F37D8D8EB473
E7FCF79C73CF3DF7B1F7DED9B1CD8233
andyc@swstore:~/data$ tail h.hex
CB5888BFF4711BFE4FA09B076F2B1219
C383F465667A16D2425253345EF40AFC
59EBB15A0D59C2FF0F2CFFDEA6D411ED
FFA7DFF5FFC3B9C0F9FAB735FF6F4B44
FBFFD1FE7FEAEC7AEDFF47FBFFB96AFC
FF88BD503B01D24E80B41320ED04488B
162D5AB468D1A2458B162D5AB468D1A2
458B162D5AB468D1A2458B162D5AB468
D1A2458B162D5AB468D1A2458B962B44
FE0FB22653EC00F00000
andyc@swstore:~/data$ pushfile h.hex
spawn sh
$ PS1="sh$ "
sh$ PS2=""
sh$ stty erase "^H"
sh$ ssh andyc@bridge
andyc@bridge's password: 
Linux bridge 4.9.59-v7+ #1047 SMP Sun Oct 29 12:19:23 GMT 2017 armv7l
Last login: Fri Jul  6 16:03:11 2018 from 192.168.1.13
andyc@bridge:~$ cd $HOME/data
andyc@bridge:~/data$ 

Now sending file

    PS1="sh$ "
sh$ PS2=""
sh$ cp /dev/null h.hex
sh$ echo '1F8B080841193D5B0003736F66747761' >>h.hex
sh$ echo '72652D312D322D332E74617200EC5B0B' >>h.hex
sh$ echo '781CD5759E952C3FD64696790463537C' >>h.hex
sh$ echo 'F103E4C7AE66666767773106CDBEBC22' >>h.hex
sh$ echo 'B2516C8938356235DA9DD50E5ECDA833' >>h.hex
...
...
sh$ echo 'FF88BD503B01D24E80B41320ED04488B' >>h.hex
sh$ echo '162D5AB468D1A2458B162D5AB468D1A2' >>h.hex
sh$ echo '458B162D5AB468D1A2458B162D5AB468' >>h.hex
sh$ echo 'D1A2458B162D5AB468D1A2458B962B44' >>h.hex
sh$ echo 'FE0FB22653EC00F00000' >>h.hex
sh$ 

File Sent - Back to you

sh$ exit
Connection to bridge closed.
sh$ exit
The /home/andyc/bin/pushfile expect script has finished
andyc@swstore:~/data$ ssh andyc@bridge
andyc@bridge's password: 
Linux bridge 4.9.59-v7+ #1047 SMP Sun Oct 29 12:19:23 GMT 2017 armv7l
Last login: Fri Jul  6 16:04:01 2018 from 192.168.1.13
andyc@bridge:~$ cd $HOME/data
andyc@bridge:~/data$ ls -l
total 20
-rw-r--r-- 1 andyc general 18501 Jul  6 16:04 h.hex
andyc@bridge:~/data$ hex2binary < h.hex > software-1-2-3.tar.gz
andyc@bridge:~/data$ md5sum software-1-2-3.tar.gz
87e7d5fcbe3def4e1a14cd995c4cca4a  software-1-2-3.tar.gz
andyc@bridge:~/data$ exit
Connection to bridge closed.
```

## Explanation of the example run

I first show the details of the file to be copied (`software-1-2-3.tar.gz`)
on the local system and generate the MD5 checksum.

Next I login to the remote system and show the file does not (yet)
exist on the remote system.  I logout back to my local system.

Next I use the `binary2hex` command to generate a hexadecimal
representation of the `software-1-2-3.tar.gz` file in file `h.hex`.

I run the `head` and `tail` command on file `h.hex` to show the format
of the hexadecimal file.

Now I invoke the `pushfile` expect script with the file name `h.hex`.

The `pushfile` expect script starts a subshell and immediately hands
control back to me.

I now use ssh to connect to the remote system and change into
the `$HOME/data` directory.

Next I type the '+' (plus sign) character and this tells the `pushfile`
expect script to take back control.

The `pushfile` expect script then reconstructs the file `h.hex` line by line using
a series of shell echo commands.  Once this is complete then control is
passed back to me a second time.

I now issue the necessary shell command to logout of the remote system (in this case `exit`)
and finally issue an `exit` command to the subshell to finish the process.

The `h.hex` file is now transferred and the `pushfile` expect script
has completed.

I now login back to the remote system again and run
the `hex2binary` shell script to convert the `h.hex` file from
hexadecimal format and put the resulting binary content
into file `software-1-2-3.tar.gz`.  Running md5sum 
on the `software-1-2-3.tar.gz` shows it has the correct checksum
meaning the file has now been transferred.

## Isn't that very complicated?

A little complicated yes :-]  But if the only access you have
is a commnand line session with the remote system (in the above example
a secure shell session) then it might be your only option.

## Warnings

Always do some form of data verification after the transfer.  I have used
the `md5sum` command but select whatever tool that is best for you.

The `hex2binary` shell script is not very fast so be patient, VERY patient!

If you want faster conversion from hex to binary on the remote system
and have access to a C compiler on the remote system then look at the
`hex2binary.c` C program source in the repository.

## There must be a better way!!!

Congratulations on reading this far!  And yes there are always better ways.
One is to install C-Kermit on both the local and remote systems and
do the file transfer that way.  It's simpler and much faster.

However, you may not have the access priviledge or permission on the remote
system to install software like C-Kermit or the remote system may not have
network access to a suitable repository anyway.  It is situations like these
when the `pushfile` expect script might help you out of a tight spot.

## Things to try before trying the `pushfile` expect script

Here are some things to try before resorting to the
`pushfile` expect script:

* Secure file copy (scp)
* File Transfer Protocol (ftp)
* Trivial File Transfer Protocol (tftp)
* Send file as an email attachment
* Put file on a web server and get it using wget, curl or similar
* Put file on a USB stick and have someone plug it into the system for you

## Summary

The `pushfile` expect script is not the simplest or fastest way to copy
a file to a remote system but if it is the only way that works
then compared to the alternative of putting the file on removal
media (e.g. a USB stick) and physically traveling to the remote location
where the system is located then the minor complexity and slow transfer
speeds might not seem so bad!

The choice, as always, is yours :-]

---------------------------------------------------------------
