token=$1
head='https://api.vk.com/method'
v=5.103

response=$(
  fetch -q -o - $head/messages.getConversations?count=8\&extended=1\&access_token=$token\&v=$v |\
  sed $'s/,"/\\\n"/g; s/:{/\\\n/g; s/},{/\\\n/g; s/\"profiles\":\\[{/\"profiles\":\\\n/g'
)

profiles=$(
  echo "$response" |\
  sed '1,/"profiles":/d; /"groups":/,/$/d' |\
  egrep '^"id":|^"first_name":|^"last_name":|^"online":'
)

conversations=$(
  echo "$response" |\
  sed '/"attachments":\[\]/,/"conversation"/d; s/}$//' |\
  egrep '^"text":|^"local_id":|^"attachments":|^"unread_count":|^"fwd_messages":\[{'
)

getname() {
  echo "$profiles" |\
  sed -n "/$1/,/\"online\"/p" |\
  grep -v '^"id":' |\
  tr -d '\n' |\
  sed 's/"first_name"://;s/"last_name":/ /;s/"online":1/ * /;s/"online":0//' |\
  tr -d '"'
}

getlist() {
  echo -e '**************************************************\n'
  chatnum=0 
  while read -r line
  do
    if [ $(echo $line | grep local_id) ]
      then
        chatnum=$(($chatnum+1))
	echo ""
        printf "$chatnum "
        id=$(echo $line | sed 's/"local_id"://')
        getname "$id"
	idnum=$(printf "id$chatnum")
        export $idnum=$id
     else
        echo $line |\
	sed '/"unread_count":/s/$/!\]/; s/"unread_count":/ \[/' |\
	sed 's/"attachments":\[{"type":"wall/: @ Запись на стене/' |\
	sed 's/"attachments":\[{"type":"photo/: >> Изображение/' |\
	sed 's/"attachments":\[{"type":"sticker/: Стикер/' |\
	sed 's/"fwd_messages":\[{.*/: @ Сообщение/' |\
	sed 's/"text":"/:  /; s/"$//; s/\\n.*//g' |\
        grep -v '^: *$' |\
	tr -d '\n'
    fi
  done <<END
  $(echo "$conversations")
END
}

getchat() {
  fetch -q -o - $head/messages.getHistory?user_id=$1\&access_token=$token\&v=$v |\
  sed $'s/,"/\\\n"/g'
}

getlist | cut -c -$(tput columns)
echo ""
while read char
do
  if [ $(echo $char | egrep [0-9]) ]
    then
      getchat $(eval echo $(printf $"id$char"))
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
