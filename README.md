# GenerateCalendarCSV
Generate list of dates based on Day Of Week and Week of Month

USAGE: gencal -dow dow -week week [-week week ...] -count count [options]  
Options:  
  -fmt    1=YYYY-MM-DD (default), 2=MM/DD/YY, 3=MM/DD/YYYY, 4=MMDDYY  
  -prefix string  
  -suffix string  
Examples:  
  First  Monday  of month:  gencal -dow 2 -week 1 -count 10  
  Third  Monday  of month:  gencal -dow 2 -week 3 -count 20  
  Second Tuesday of month:  gencal -dow 3 -week 2 -count 35  
  First and Third Monday of month:  gencal -dow 2 -week 1 -week 3 -count 10  
AWS </home/tms/src/gencal>$ cat event.csv   
Today: 6/10/2021 11:22:7 5  
First: 6/1/2021 11:22:7 3  
2021-06-07,19:00:00,1,1  
2021-06-21,19:00:00,1,1  
2021-07-05,19:00:00,1,1  
2021-07-19,19:00:00,1,1  
2021-08-02,19:00:00,1,1  
2021-08-16,19:00:00,1,1  
2021-09-06,19:00:00,1,1  
2021-09-20,19:00:00,1,1  
2021-10-04,19:00:00,1,1  
2021-10-18,19:00:00,1,1  
2021-11-01,19:00:00,1,1  
2021-11-15,19:00:00,1,1  
2021-12-06,19:00:00,1,1  
2021-12-20,19:00:00,1,1  
2022-01-03,19:00:00,1,1  
2022-01-17,19:00:00,1,1  
AWS </home/tms/src/gencal>$ cat LoadEvent.sql  

load data local infile 'event.csv'  
into table event  
fields terminated by ',' lines terminated by '\n'   
( Eascdate, Etime, Elocation, Edept );  

