#!/usr/bin/env bash
if [[ "$#" -lt 5 ]] ; then
  echo "Wrong number of parameters"
  exit
fi
if ! [[ "$4" =~ ^[0-9]+$ ]]
    then
        echo "Sorry number of files must be an integer"
        exit
fi
if ! [[ "$5" =~ ^[0-9]+$ ]]
    then
        echo "Sorry number of files must be an integer"
        exit
fi

diseasesFile=$1
countriesFile=$2
input_dir=$3
numFilesPerDirectory=$4
numRecordsPerFile=$5

mkdir -p $input_dir

country=()
disease=()

#Create array of diseases from file & count them
d=0
while read line; do
  disease[$d]=${line}
  let d=d+1
done < $diseasesFile

#Create array of countries from file & count them
c=0
while read line; do
  country[$c]=${line}
  let c=c+1
done < countriesFile

i=0
path=$3
while [[ ${i} -lt ${c} ]]; do
  country_folder=${path}
  country_folder="${country_folder}/${country[i]}"
  mkdir -p "${country_folder}"
  j=0
  while [[ ${j} -lt ${numFilesPerDirectory} ]]; do
    day=$(($RANDOM%30+1))
    month=$(($RANDOM%12+1))
    year=$(($RANDOM%101+1920))
    filename="${day}-${month}-${year}"
    file="${country_folder}/${filename}"
    touch "${file}"
    k=0
    while [[ ${k} -lt ${numRecordsPerFile} ]]; do
      #create rand data
      id=$RANDOM
      rand=$(($RANDOM % 2))
      if ([ $rand == 0 ])
      then
        status='EXIT'
      else
        status='ENTER'
      fi
      name=$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 13 | head -n 1)
      lastname=$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 13 | head -n 1)
      virus=${disease[$RANDOM%$d]}
      age=$(($RANDOM % 121))

      #print it in file
      echo "$id $status $name $lastname $virus $age" >> "${file}"

      k=$((k+1))
    done
    j=$((j+1))
  done
  i=$((i+1))
done



























