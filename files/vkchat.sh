token=$1
head='https://api.vk.com/method'
v=5.103

getlist() {
  echo -e '**************************************************\n'
  chatnum=0
  list=$(
  fetch -q -o - $head/messages.getConversations?count=8\&access_token=$token\&v=$v |\
  sed $'s/,"/\\\n"/g;s/:{/\\\n/g' |\
  sed '/attachments":/,/"conversation"/d; s/}$//' |\
  egrep '"text":|"local_id":')
  while read -r line
  do
    if [ $(echo $line | grep local_id) ]
      then
        chatnum=$(($chatnum+1))
        printf "$chatnum "
        id=$(echo $line | sed 's/"local_id"://')
        getname "$id"
	idnum=$(printf "id$chatnum")
        export $idnum=$id
     else
        echo -e $line | sed 's/"text"://'
        echo -e '\n--------------------------------------------------\n'
    fi
  done <<END
  $(echo "$list")
END
}

getname() {
      fetch -q -o - $head/users.get?user_ids=$id\&access_token=$token\&v=$v |\
      sed 's/^.*"first_name":"//;s/","last_name":"/ /; s/",".*$/:/'
      echo ""
}

getchat() {
  fetch -q -o - $head/messages.getHistory?user_id=25450017\&access_token=$token\&v=$v
}

getlist
echo "choose chat number or press r for reload or q for quit:"
while read char
do
  if [ $(echo $char | egrep [0-9]) ]
    then
      getchat $(printf "id$char")
  fi
  if [ $char == r ]
    then
      getlist
  fi
  if [ $char == q ]
    then
      exit
  fi
done
