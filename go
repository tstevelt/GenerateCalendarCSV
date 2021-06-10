#!/bin/sh

DATABASE=YourDatabase

gencal -dow 2 -week 1 -week 3 -count 16 -suffix ',19:00:00,1,1' > event.csv

cat event.csv

#mysql -D $DATABASE < LoadEvent.sql
#echo 'select * from event' | mysql -D $DATABASE

