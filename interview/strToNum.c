int str2int(char *str)
{
	int num = 0;
	while (*str != '\0') {
		int digital = *str - 48;
		num = num * 10 + digital;
		str ++;
	}
	return num;
}

void int2str(int num, char *val)
{
	char str_temp[100];
	int i, j;
	int temp1 = numl;
	int temp2

	for (i = 0; i < 100; i++) {
		temp2 = temp1 % 10;
		temp1 /= 10;
		str_temp[i] = temp2 + 48;

		if (temp1 < 10) {
			i++;
			str_temp[i] = temp1 + 48;
			break;
		}
	}
	for (j = 0; j < i; j++) {
		val[j] = str_temp[i-j];
	}

	val[j] = '\0';
}
