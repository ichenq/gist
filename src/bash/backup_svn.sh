svn_repo='/data/storage/svn/bomberman'
backup_dir='/data/backups/svn'
timestamp=`date +%Y%m%d%H`
youngest=`svnlook youngest $svn_repo`
backup_file="backup_bbm_$timestamp"

#echo "backup $svn_repo with revision $youngest"
cd $backup_dir
svnadmin hotcopy $svn_repo $backup_dir/$backup_file &&
#svnadmin dump --revision 0:$youngest $svn_repo > $backup_file && 
#7z a -mhe=on -mx=9 $backup_file.lzma $backup_file &&
tar -czf $backup_file.tar.gz $backup_file &&
rm -rf $backup_file

echo 'Succeed'

find -mtime +7 -delete

