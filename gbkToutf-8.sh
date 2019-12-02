#bin/bash
#同一个文件夹下批量转换文件格式 GBK to UTF-8
PWD=`pwd`
for i in `find $PWD -type f`;
do
	echo $i;
	iconv -f GBK -t UTF-8 $i > tmpfile;
	cp tmpfile $i;
	rm tmpfile;
done
	
	
