
load data local infile 'event.csv'
into table event
fields terminated by ',' lines terminated by '\n' 
( Eascdate, Etime, Elocation, Edept );

show warnings ;
