/*----------------------------------------------------------------------------
	Program : gencal.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Jun 2021
	Synopsis: Generate calendar date in CSV format.
	Return  : 

	MIT License
	-----------

	Copyright (c) 2021 Tom Stevelt (https://www.silverhammersoftware.com)
	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
			

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		06/10/2021	Added -prefix and -suffix options.

----------------------------------------------------------------------------*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>
#include	<time.h>

#define		DATEFMT_MM_DD_YY		11
#define		DATEFMT_MM_DD_YYYY		12
#define		DATEFMT_MMDDYY			13
#define		DATEFMT_YYYY_MM_DD		14
#define		DATEFMT_AAA_DD_YYYY		15
#define		DATEFMT_EPOCH_MILLISEC	16
#define		DATEFMT_SYSTEM_DAYS		17
#define		DATEFMT_TIMESTAMP		18

static	int		MyDayOfWeek, WhichWeek[5], WhichCount, Count;
static	int		DateOutputType = DATEFMT_YYYY_MM_DD;
static	char	*Prefix = "";
static	char	*Suffix = "";

static void MyUsage ( char *msg )
{
	if ( strlen ( msg ) > 0 )
	{
		printf ( "\n*** %s ***\n\n", msg );
	}

	printf ( "USAGE: gencal -dow dow -week week [-week week ...] -count count [options]\n" );
	printf ( "Options:\n" );
	printf ( "  -fmt    1=YYYY-MM-DD (default), 2=MM/DD/YY, 3=MM/DD/YYYY, 4=MMDDYY\n" );
	printf ( "  -prefix string\n" );
	printf ( "  -suffix string\n" );
	printf ( "Examples:\n" );
	printf ( "  First  Monday  of month:  gencal -dow 2 -week 1 -count 10\n" );
	printf ( "  Third  Monday  of month:  gencal -dow 2 -week 3 -count 20\n" );
	printf ( "  Second Tuesday of month:  gencal -dow 3 -week 2 -count 35\n" );
	printf ( "  First and Third Monday of month:  gencal -dow 2 -week 1 -week 3 -count 10\n" );
	exit ( 1 );
}

static void OutputFunction ( int DateFormat, int Month, int Day, int Year )
{
	if ( strlen ( Prefix ) > 0 )
	{
		printf ( "%s", Prefix );
	}

	switch ( DateFormat )
	{
		case DATEFMT_MM_DD_YY:
			printf ( "%02d/%02d/%02d", Month, Day, Year % 100 );
			break;
		case DATEFMT_MM_DD_YYYY:
			printf ( "%02d/%02d/%04d", Month, Day, Year );
			break;
		case DATEFMT_MMDDYY:
			printf ( "%02d%02d%02d", Month, Day, Year % 100 );
			break;
		case DATEFMT_YYYY_MM_DD:
			printf ( "%04d-%02d-%02d", Year, Month, Day );
			break;
		case DATEFMT_AAA_DD_YYYY:
		case DATEFMT_SYSTEM_DAYS:
		case DATEFMT_TIMESTAMP:
		case DATEFMT_EPOCH_MILLISEC:
		default:
			fprintf ( stderr, "date format %d not supported at this time\n", DateFormat );
			exit ( 1 );
			break;
	}

	if ( strlen ( Suffix ) > 0 )
	{
		printf ( "%s", Suffix );
	}

	printf ( "\n" );
}


static void getargs ( int argc, char *argv[] )
{
	int		xa;

	if ( argc < 6 )
	{
		MyUsage ( "" );
	}

	WhichCount = 0;

	for ( xa = 1; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && strcmp ( argv[xa], "-dow" ) == 0 )
		{
			xa++;
			MyDayOfWeek = atoi ( argv[xa] );
			if ( MyDayOfWeek < 1 || MyDayOfWeek > 7 )
			{
				MyUsage ( "dow 1 - 7" );
			}
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-week" ) == 0 )
		{
			xa++;
			WhichWeek[WhichCount] = atoi ( argv[xa] );
			if ( WhichWeek[WhichCount] < 1 || WhichWeek[WhichCount] > 4 )
			{
				MyUsage ( "week 1 - 4" );
			}
			WhichCount++;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-count" ) == 0 )
		{
			xa++;
			Count = atoi ( argv[xa] );
			if ( Count < 1 || Count > 100 )
			{
				MyUsage ( "count 1 - 100" );
			}
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-fmt" ) == 0 )
		{
			xa++;
			switch ( argv[xa][0] )
			{
				case '1':
					DateOutputType = DATEFMT_YYYY_MM_DD;
					break;
				case '2':
					DateOutputType = DATEFMT_MM_DD_YY;
					break;
				case '3':
					DateOutputType = DATEFMT_MM_DD_YYYY;
					break;
				case '4':
					DateOutputType = DATEFMT_MMDDYY;
					break;
				default:
					MyUsage ( "unknown -fmt" );
					break;
			}
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-prefix" ) == 0 )
		{
			xa++;
			Prefix = argv[xa];
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-suffix" ) == 0 )
		{
			xa++;
			Suffix = argv[xa];
		}
		else
		{
			MyUsage ( "" );
		}
	}
}

int main ( int argc, char *argv[] )
{
	int			Counter;
	struct tm	*ptrTime;
	time_t		tv;
	int			ThisMyDayOfWeek, ThisWeek;
	int			xw, KeepIt;

	getargs ( argc, argv );

	tv = time ( NULL );

	ptrTime = localtime ( &tv );

	printf ( "Today: %d/%d/%d %d:%d:%d %d\n",
		ptrTime->tm_mon + 1,
		ptrTime->tm_mday,
		ptrTime->tm_year + 1900,
		ptrTime->tm_hour,
		ptrTime->tm_min,
		ptrTime->tm_sec,
		ptrTime->tm_wday + 1 );

	if ( ptrTime->tm_mday > 1 )
	{
		tv = tv - ( 86400 * (ptrTime->tm_mday - 1) );

		ptrTime = localtime ( &tv );

		printf ( "First: %d/%d/%d %d:%d:%d %d\n",
			ptrTime->tm_mon + 1,
			ptrTime->tm_mday,
			ptrTime->tm_year + 1900,
			ptrTime->tm_hour,
			ptrTime->tm_min,
			ptrTime->tm_sec,
			ptrTime->tm_wday + 1 );

		ThisMyDayOfWeek = ptrTime->tm_wday + 1;

		if ( ThisMyDayOfWeek == MyDayOfWeek )
		{
			ThisWeek = 1;
		}
		else
		{
			ThisWeek = 0;
		}
	}

	for ( Counter = 0; Counter < Count; )
	{
		tv += 86400;

		ptrTime = localtime ( &tv );

		if ( ptrTime->tm_mday == 1 )
		{
			ThisWeek = 0;
		}

		ThisMyDayOfWeek = ptrTime->tm_wday + 1;

		if ( ThisMyDayOfWeek == MyDayOfWeek )
		{
			ThisWeek++;

			for ( xw = 0, KeepIt = 0; xw < WhichCount && KeepIt == 0; xw++ )
			{
				if ( ThisWeek == WhichWeek[xw] )
				{
					KeepIt = 1;
				}
			}

			if ( KeepIt )
			{
#ifdef DEBUG
				printf ( "%d/%d/%d\n", ptrTime->tm_mon + 1, ptrTime->tm_mday, ptrTime->tm_year + 1900 );
#endif
				
				OutputFunction ( DateOutputType,  
					ptrTime->tm_mon + 1, ptrTime->tm_mday, ptrTime->tm_year + 1900 );

				Counter++;
			}

		}

	}

	return ( 0 );
}
