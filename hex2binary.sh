
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
