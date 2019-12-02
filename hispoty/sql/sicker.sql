create table sicker
(
	ID int not null auto_increment primary key,
	name varchar(20) not null,
	passwd varchar(20) not null,
	sex varchar(10) not null,
	age int not null,
	doctor varchar(20)not null,
	nures varchar(20) not null,
	atTime varchar(20) not null,
	CBZD varchar(50) not null
);
