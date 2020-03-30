#!bin/bash

#check word if empty and should not contain nunbers and spetial characters
while [ 1 ]
do 
    echo -e "Enter new word: \c"
    read word
    if [ -z "$word" ];then
      echo "word is empty"
    elif [[ $word =~ [0-9] ]];then
      echo "word should not contains numbers please enter again!!!"
    elif [[ ! $word =~ ^[[:alnum:]]+$ ]];then
      echo "word should not contain special character please enter again!!!"
    else
        break
    fi
done

#check meaning if empty and should not contain numbers
while [ 1 ]
do 
    echo -e "Enter the meaning: \c"
    read meaning
    if [ -z "$meaning" ];then
      echo "word should have some meaning it should not be meaning less"
    elif [[ $meaning =~ [0-9] ]];then
      echo "Meaning should not contains numbers please enter again!!!"
    else
      break
    fi
done

#store the word and its meaning into the dictionary named mydictionary.txt
count=`grep -c "word=$word" mydictionary.txt`
if [ $count -eq 0 ];
then 
     echo "word=$word meaning=$meaning" | cat >> mydictionary.txt
     echo "$word has been successfully added to your dictionary!!"
else
    echo "$word already exist in your dictionary."
fi

#sorting the dictionary
sort -o mydictionary.txt mydictionary.txt