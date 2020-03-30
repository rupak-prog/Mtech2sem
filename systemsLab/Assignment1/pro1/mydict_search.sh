#!bin/bash

echo -e "Enter the new word: \c"
read word

#check if word is not empty
while [ -z "$word" ]
do 
    echo -e "Enter the new word: \c"
    read word
    if [ -z "$word" ]
    then
      echo "word is empty"
    else
        break
    fi
done

#find the word in dictionay
count=`grep -c "word=$word" mydictionary.txt`
if [ $count -eq 1 ];
then 
    echo -e "$word : \c"
    meaning=`grep -n "word=$word" mydictionary.txt | cut -d "=" -f3`
    echo $meaning
else
    echo "$word does not exist in your dictionary."
fi