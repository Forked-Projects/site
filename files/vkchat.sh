token=$1
head='https://api.vk.com/method'
v=5.103

update() {
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
}

getname() {
  echo "$profiles" |\
  sed -n "/$1/,/\"online\"/p" |\
  grep -v '^"id":' |\
  tr -d '\n' |\
  sed 's/"first_name"://;s/"last_name":/ /;s/"online":1/ * /;s/"online":0//' |\
  tr -d '"'
}

getids() {
  chatnum=0 
  while read line
  do
    if [ $(echo $line | grep local_id) ]
      then
        chatnum=$(($chatnum+1))
        id=$(echo $line | sed 's/"local_id"://')
	idnum=$(printf "id$chatnum")
        export $idnum=$id
    fi
  done <<END
  $(echo "$conversations")
END
}

getlist() {
  echo '********************************************************************************'
  chatnum=0 
  while read -r line
  do
    if [ $(echo $line | grep local_id) ]
      then
        chatnum=$(($chatnum+1))
	echo ""
        printf "$chatnum "
	eval getname $(printf $"id$chatnum")
      else  
        echo $line |\
	sed '/"unread_count":/s/$/!\]/; s/"unread_count":/ \[/' |\
	sed 's/"attachments":\[{"type":"wall/: @ Запись на стене/' |\
	sed 's/"attachments":\[{"type":"photo/: >> Изображение/' |\
	sed 's/"attachments":\[{"type":"sticker/: Стикер/' |\
	sed 's/"fwd_messages":\[{.*/: @ Сообщение/' |\
	sed 's/"text":"/:  /; s/"$//; s/\\n/ /g; s/\\"/"/g; s/  / /g' |\
        grep -v '^: *$' |\
	tr -d '\n'
    fi
  done <<END
  $(echo "$conversations")
END
}

gethistory() {
  fetch -q -o - $head/messages.getHistory?user_id=$1\&access_token=$token\&v=$v |\
  sed $'s/,"/\\\n"/g'
  echo ''
  echo '--------------------------------------------------------------------------------'
  echo -n '>> : '
}

getdialog() {
  curid=$1
  gethistory $curid
  while read message
  do
    if [ "$message" == r ] || [ "$message" == к ]
      then
	gethistory $curid
    elif [ "$message" == q ] || [ "$message" == й ]
      then
        mainlist
    else
      echo "$message"
      random=$(cat /dev/urandom | head -1 | od -D -N4 -vAn | tr -d ' ')
      fetch -q -o - $head/messages.send?user_id=$curid\&message="$message"\&random_id=$random\&access_token=$token\&v=$v |\
      grep -v "response"
      gethistory $curid
    fi
  done
} 

mainlist() {
  update
  getids
  getlist | cut -c -$(tput columns)
  echo ""
  echo ""
  while read char
  do
    if [ $(echo "$char" | egrep [0-9]) ]
      then
        curid=$(eval printf $"id$char")
        getdialog $curid
    fi
    if [ "$char" == r ] || [ "$char" == к ]
      then
	update
        getids
        getlist | cut -c -$(tput columns)
        echo ""
        echo ""
    fi
    if [ "$char" == q ] || [ "$char" == й ]
      then
        exit
    fi
  done
}

mainlist
