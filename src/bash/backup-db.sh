#!/bin/sh
#
# backup mysql

cd ~
if [ ! -d 'backups' ]; then
    mkdir backups
fi
cd backups

basename=`date +%Y%m%d-%H%M%S`
dbargs=`cat $GOPATH/bin/deploy/server.conf`
alias dump-database='mysqldump --opt ${dbargs}'

dump-database loris_localization > loris_localization.sql &&
dump-database loris_character_01 > character_01.sql &&
dump-database loris_character_02 > character_02.sql &&
dump-database loris_character_03 > character_03.sql &&
dump-database loris_battlematch > loris_battlematch.sql &&
dump-database loris_guild > loris_guild.sql &&
dump-database loris_mail > loris_mail.sql &&
dump-database loris_world > loris_world.sql &&
dump-database loris_battle > loris_battle.sql &&
dump-database loris_battle > loris_battle.sql &&
dump-database loris_recharge > loris_battle.sql &&
dump-database loris_log > loris_log.sql &&

tar -zcvf backup_$basename.tar.gz *.sql && rm -f *.sql || exit 1

# delete old files
find -mtime +6 -delete
