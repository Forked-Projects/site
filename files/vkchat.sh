token=$1
head='https://api.vk.com/method'
v=5.103

if [ $1 ]
  then

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
	sed 's/"attachments":\[{"type":"audio/: >> Аудиотрек/' |\
	sed 's/"attachments":\[{"type":"doc/: >> Документ/' |\
	sed 's/"attachments":\[{"type":"video/: >> Видео/' |\
	sed 's/"attachments":\[{"type":"sticker/: >> Стикер/' |\
	sed 's/"attachments":\[{"type":"link/: @ Ссылка/' |\
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
  echo ''
  echo "updating dialog..."
  history=$(
    fetch -q -o - $head/messages.getHistory?user_id=$1\&extended=1\&access_token=$token\&v=$v |\
    sed $'s/},{"date"/}\\\n\\\n{"date"/g' |\
    tail -r |\
    sed $'s/,"/\\\n"/g; s/}},{/\\\n/g; s/\"profiles\":\\[{/"profiles":\\\n/g' |\
    grep -v '"fwd_messages":\[\]' |\
    sed $'s/"fwd_messages":/"fwd_messages":\\\n/' |\
    grep -v '"attachments":\[\]' |\
    grep -v '"text":""'
  )

  friendname() {
    echo "$history" |\
    sed -n '/"profiles":/,/{"date"/p' |\
    egrep '^"id":|^"first_name":|^"last_name":' |\
    tr -d '\n' |\
    sed $'s/"id":/\\\n/g' |\
    grep $1 |\
    sed 's/.*"first_name":"//; s/""last_name":"/ /; s/"$/: /'
  }

  historyupd=$(
  echo "$history" | sed '/"profiles":/,/}\]}}/d' |\
  while read -r line
  do
    if [ $(echo $line | egrep '"type":"[mopqstuvwxyz]"') ]
      then
	read -r line
    elif [ $(echo $line | grep '"from_id"' | grep -v '-') ]
      then
        friendid=$(echo "$line" | sed 's/"from_id"://')
        friendname $friendid | tr -d '\n'
    elif [ $(echo $line | grep '{"date":') ]
      then
	echo $line | sed 's/.*"date"://' | xargs date -Iminutes -r |\
	sed 's/+.*//; s/T/ /'
    elif [ $(echo $line | egrep '^"height":64.*128"') ]
      then
	echo $line | sed 's/.*"url":"//; s/"$//'
    else
	echo $line |\
	egrep '^$|"date":|"text":|"url":|"fwd_messages":|"attachments":|"audio":{"artist"|"title":' |\
	sed 's/"fwd_messages":/@ Пересланное сообщение/' |\
	sed 's/"attachments":\[{"type":"wall/@ Запись на стене/' |\
	sed 's/"attachments":\[{"type":"photo/>> Изображение/' |\
	sed 's/"attachments":\[{"type":"audio/>> Аудиотрек/' |\
	sed 's/"attachments":\[{"type":"doc/>> Документ/' |\
	sed 's/"attachments":\[{"type":"video/>> Видео/' |\
	sed 's/"attachments":\[{"type":"sticker/>> Стикер/' |\
	sed 's/"attachments":\[{"type":"link/@ Ссылка/' |\
	sed 's/"audio":{"artist":"/Исполнитель: /' |\
	sed 's/"title":"/Название: /' |\
	egrep -v '"url":.*\\/sticker\\/' |\
	sed 's/"url":"//' |\
	grep -v '^"date":' |\
	sed 's/"$//; s/"text":"//; s/\\n\\n/\\n/g; s/\\n/\
/g'
    fi
  done
  )

  echo "$historyupd"
  echo ''
  echo '--------------------------------------------------------------------------------'
  echo -n '>> : '
}

getdialog() {
  curid=$1
  gethistory $curid
  while read input
  do
    if [ "$input" == q ] || [ "$input" == й ]
      then
        mainlist
    elif [ "$input" == r ] || [ "$input" == к ]
      then
	gethistory $curid
    else
        random=$(cat /dev/urandom | head -1 | od -D -N4 -vAn | tr -d ' ')
	message=$input
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

  else
    echo "Usage: sh vkchat.sh access_token
To get access_token please visit https://oauth.vk.com/authorize?client_id=2685278&scope=messages,friends,offline&redirect_uri=https://oauth.vk.com/blank.html&display=page&response_type=token&v=5.103"
fi
