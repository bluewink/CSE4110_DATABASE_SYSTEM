create table customer(
ID varchar(20),
 customer_name varchar(20),
 address	varchar(20),
 phone_number numeric(15,0),
 primary key (ID) );
 
create table recipient(
recipient_name	varchar(20),
 address		varchar(30),
 phone_number	numeric(15,0),
 primary key (recipient_name, address) );
 
create table package (
package_ID		varchar(20),
 package_type		varchar(20),
 weight		numeric(10,0),
 timeliness varchar(20),
 hazardous varchar(20),
 international varchar(20),
 customer_ID varchar(20),
 recipient_name varchar(20),
 recipient_address varchar(30),
 scheduled_time numeric(10,0),
 primary key (package_ID),
 foreign key (customer_ID) references customer(ID) 
 on delete set null,
 foreign key (recipient_name,recipient_address) references recipient(recipient_name,address) 
 on delete set null );
 
create table bill(
bill_ID			varchar(20),
 package_ID 	varchar(20),
 amount			numeric(10,2),
 pay_type		varchar(20),
 pay_year 		numeric(10),
 pay_date 		numeric(20),
 pay_time 		numeric(20),
 credit_card_num numeric(20),
 account_num 	numeric(20),
 customer_ID	varchar(20),
 primary key (bill_ID),
 foreign key (customer_ID) references customer (ID) 
 on delete set null,
 foreign key (package_ID) references package (package_ID) 
 on delete set null);
 
create table location(
package_ID	varchar(20),
 tracking_year numeric(10,0),
 tracking_date		numeric(15,0),
 tracking_time      numeric(15,0),
 cur_position 		varchar(20),
 heading_to 		varchar(20),
 status 			varchar(20),
 warehouse_ID		varchar(20),
 primary key (package_ID, tracking_year,
 tracking_date, tracking_time),
 foreign key (package_ID) references package(package_ID) 
 on delete cascade);
 
create table transportation(
transportation_ID			varchar(20),
 type						varchar(20),
 primary key (transportation_ID) );
 
create table delivers(
package_ID				varchar(20),
transportation_ID		varchar(20),
primary key (package_ID, transportation_ID),
foreign key (package_ID) references package (package_ID) 
on delete cascade,
foreign key (transportation_ID) references transportation (transportation_ID)
on delete cascade);