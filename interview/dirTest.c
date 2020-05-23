#include <stdio.h>
#include <dirent.h>

int main(int argc, const char *argv[])
{
	char buff[1024] = {0};
	struct dirent *entptr = NULL;
	DIR *dirptr = opendir("/home/ljx/working/address_book");
	if (!dirptr) {
		goto error;
	}
	while ((entptr = readdir(dirptr))) {
		if (entptr->d_name[0] == '.') {
			continue;
		}
		snprintf(buff, 1024, "%s", entptr->d_name);
		break;
	}
	printf("%s\n", buff);
	closedir(dirptr);
	return 0;
error:
	buff[0] = '\0';
	if (dirptr) {
		closedir(dirptr);
		return -1;
	}
}
