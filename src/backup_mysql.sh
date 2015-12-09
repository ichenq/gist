#!/bin/sh

# created at Jul 7, 2010

basename=`date +%Y%m%d%H%M`
sql_file=$HOME/backup/back_file/account_$basename.sql
mysqldump --opt -u root -phy4rmb account_db > $sql_file
7z a -mhe=on -mx=9  $sql_file.lzma $sql_file
rm -f $sql_file
#lftp -c open -e "mput $sql_file.lzma" ftps://hydbk:bk4hy@58.221.28.159:990
#lftp -c open -e "mput $sql_file.lzma" ftps://dbb:dbb@61.164.140.95:990
scp -P 12200 $sql_file.lzma hypr@58.221.28.159:/home/hypr/db_backup
cd /home/ubuntu/backup/back_file
find -mtime +6 -delete

#-p7817A38D3E77
