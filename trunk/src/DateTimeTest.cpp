#include <gtest/gtest.h>

#include <Poco/Timestamp.h>
#include <Poco/DateTime.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timespan.h>
#include <Poco/Timezone.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeParser.h>
using Poco::Timestamp;
using Poco::DateTime;
using Poco::LocalDateTime;
using Poco::Timespan;
using Poco::Timezone;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::DateTimeParser;

TEST(DateTimeTest, TimestampTest)
{
	Timestamp now; // the current date and time
	std::time_t t1 = now.epochTime(); // convert to time_t ...
	Timestamp ts1(Timestamp::fromEpochTime(t1)); // ... and back again
	for (int i = 0; i < 100000; ++i) ; // wait a bit
	Timestamp::TimeDiff diff = now.elapsed(); // how long did it take?
	Timestamp start(now); // save start time
	now.update(); // update with current time
	diff = now - start; // again, how long?
}

TEST(DateTimeTest, DateTimeTest)
{
	DateTime now; // the current date and time in UTC
	int year = now.year();
	int month = now.month();
	int day = now.day();
	int dow = now.dayOfWeek();
	int doy = now.dayOfYear();
	int hour = now.hour();
	int hour12 = now.hourAMPM();
	int min = now.minute();
	int sec = now.second();
	int ms = now.millisecond();
	int us = now.microsecond();
	double jd = now.julianDay();
	Poco::Timestamp ts = now.timestamp();
	DateTime xmas(2006, 12, 25); // 2006-12-25 00:00:00
	Poco::Timespan timeToXmas = xmas - now;
	DateTime dt(1973, 9, 12, 2, 30, 45); // 1973-09-12 02:30:45
	dt.assign(2006, 10, 13, 13, 45, 12, 345); // 2006-10-13 12:45:12.345
	bool isAM = dt.isAM(); // false
	bool isPM = dt.isPM(); // true
	bool isLeap = DateTime::isLeapYear(2006); // false
	int days = DateTime::daysOfMonth(2006, 2); // 28
	bool isValid = DateTime::isValid(2006, 02, 29); // false
	dt.assign(2006, DateTime::OCTOBER, 22); // 2006-10-22 00:00:00
	if (dt.dayOfWeek() == DateTime::SUNDAY)
	{
		// ...
	}
}

TEST(DateTimeTest, LocalDateTimeTest)
{
	LocalDateTime now; // the current date and local time
	int year = now.year();
	int month = now.month();
	int day = now.day();
	int dow = now.dayOfWeek();
	int doy = now.dayOfYear();
	int hour = now.hour();
	int hour12 = now.hourAMPM();
	int min = now.minute();
	int sec = now.second();
	int ms = now.millisecond();
	int us = now.microsecond();
	int tzd = now.tzd();
	double jd = now.julianDay();
	Poco::Timestamp ts = now.timestamp();
	LocalDateTime dt1(1973, 9, 12, 2, 30, 45); // 1973-09-12 02:30:45
	dt1.assign(2006, 10, 13, 13, 45, 12, 345); // 2006-10-13 12:45:12.345
	LocalDateTime dt2(3600, 1973, 9, 12, 2, 30, 45, 0, 0); // UTC +1 hour
	dt2.assign(3600, 2006, 10, 13, 13, 45, 12, 345, 0);
	Poco::Timestamp nowTS;
	LocalDateTime dt3(3600, nowTS); // construct from Timestamp
}

TEST(DateTimeTest, TimespanTest)
{
	Timespan ts1(1, 11, 45, 22, 123433); // 1d 11h 45m 22.123433s
	Timespan ts2(33*Timespan::SECONDS); // 33s
	Timespan ts3(2*Timespan::DAYS + 33*Timespan::HOURS); // 3d 33h
	EXPECT_EQ(1, ts1.days()); // 1
	EXPECT_EQ(11, ts1.hours()); // 11
	EXPECT_EQ(35, ts1.totalHours()); // 35
	EXPECT_EQ(45, ts1.minutes()); // 45
	EXPECT_EQ(2145, ts1.totalMinutes()); // 2145
	EXPECT_EQ(22, ts1.seconds()); // 22
	EXPECT_EQ(128722, ts1.totalSeconds()); // 128722
}

TEST(DateTimeTest, DateTimeTimespanTest)
{
	// what is my age?
	DateTime birthdate(1985, 10, 11, 2, 30);
	DateTime now(2011, 8, 18, 14, 30);
	Timespan age = now - birthdate;
	int days = age.days(); // in days
	int hours = age.totalHours(); // in hours
	int secs = age.totalSeconds(); // in seconds
	// when was I 10000 days old?
	Timespan span(10000*Timespan::DAYS);
	DateTime dt = birthdate + span;
}

TEST(DateTimeTest, TimezoneTest)
{
	int utcOffset = Timezone::utcOffset();
	int dst = Timezone::dst();
	bool isDst = Timezone::isDst(Timestamp());
	int tzd = Timezone::tzd();
	std::string name = Timezone::name();
	std::string stdName = Timezone::standardName();
	std::string dstName = Timezone::dstName();
}

TEST(DateTimeTest, DateTimeFormatterTest)
{
	Poco::DateTime dt(2006, 10, 22, 15, 22, 34);
	std::string s(DateTimeFormatter::format(dt, "%e %b %Y %H:%M"));
	EXPECT_EQ("22 Oct 2006 15:22", s);
	// "22 Oct 2006 15:22"
	Poco::Timestamp now;
	s = DateTimeFormatter::format(now, DateTimeFormat::SORTABLE_FORMAT);
	std::string ssss = DateTimeFormatter::format(now, DateTimeFormat::SORTABLE_FORMAT);
	EXPECT_EQ(ssss, s);
	// "2006-10-30 09:27:44"
	Poco::Timespan span(5, 11, 33, 0, 0);
	s = DateTimeFormatter::format(span, "%d days, %H hours, %M minutes");
	EXPECT_EQ("5 days, 11 hours, 33 minutes", s);
	// "5 days, 11 hours, 33 minutes"
}

TEST(DateTimeTest, DateTimeParserTest)
{
	std::string s("Sat, 1 Jan 2005 12:00:00 GMT");
	int tzd;
	DateTime dt;
	DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, s, dt, tzd);
	Poco::Timestamp ts = dt.timestamp();
	Poco::LocalDateTime ldt(tzd, dt);
	bool ok = DateTimeParser::tryParse("2006-10-22", dt, tzd);
	ok = DateTimeParser::tryParse("%e.%n.%Y", "22.10.2006", dt, tzd);
}
