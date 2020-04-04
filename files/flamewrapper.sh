scrdir=~/Pictures

check="ls -t $scrdir | head -1"
before=$(eval $check)
flameshot gui -p $scrdir
count=0
while [ $before == $(eval $check) ]
do
  sleep 1
  if [ $count == 60 ]
    then
      break
  fi
  count=$(($count + 1))
done
scrname=$(eval $check)
if [ $before != $scrname ]
  then
    dolphin --select $scrdir/$scrname &
fi
