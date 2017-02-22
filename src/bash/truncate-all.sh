#!/bin/bash

echo 'this will truncate all database tables, sure?[y/n]'
read sure
if [ $sure != 'y' ]; then
    exit
fi

dbargs=`cat server.conf`
filename='schemas.txt'

# truncate all tables in db schema
truncate_tables () {
    output=`echo "use $1; show tables;" | mysql ${dbargs}`
    read -r -a array <<< $output
    #remove first array element
    schemas=(${array[@]:1})
    for name in "${schemas[@]}"; do
        #echo "truncate $1.$name"
        echo "use $1; TRUNCATE TABLE $name;" | mysql ${dbargs}
    done
}

# read all schema from file
readarray -t schemas < $filename
for name in "${schemas[@]}"; do
    echo "truncate $name"
    truncate_tables $name
done

echo 'OK'
