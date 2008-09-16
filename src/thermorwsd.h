#ifndef _THERMOSWSD_H
#define _THERMOSWSD_H

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#define RAW_DATA			1

#define NUM_DATA			8

#define UNIX_PATH	"/tmp/wsd"

/* Data Types */
#define DATA_TYPE_DATE			0x02
#define DATA_TYPE_TIME			0x03
#define DATA_TYPE_PRESSURE		0x10
#define DATA_TYPE_RAIN			0x11
#define DATA_TYPE_WIND_CHILL	0x12
#define DATA_TYPE_OUT_TEMP		0x13
#define DATA_TYPE_IN_TEMP		0x14
#define DATA_TYPE_WIND_DIR		0x15
#define DATA_TYPE_WIND_SPEED	0x16
#define DATA_TYPE_WIND_GUST		0x17
#define DATA_TYPE_HUMIDITY		0x18
#define DATA_TYPE_FORECAST		0x19
#define DATA_TYPE_TREND			0x1a
#define DATA_TYPE_UNKNOWN1		0x1b
#define DATA_TYPE_MAX			0x1c

/* Data Categories */
#define DATA_MIN				0x01
#define DATA_MAX				0x02
#define DATA_CURRENT			0x03

/* Default Text */
#define DEF_MAX_TXT					"Maximum "
#define DEF_MIN_TXT					"Minimum "
#define DEF_CUR_TXT					"Current "

#define DEF_DATA_PREFIX_TXT			"DATA: "
#define DEF_DATA_SEPARATOR_TXT		": "
#define DEF_UNIT_SEPARATOR_TXT		" "
#define DEF_NO_READING_TXT			"-"

#define DEF_TIME_TXT				"Time"
#define DEF_TIME_SUFFIX_TXT			""

#define DEF_DATE_TXT				"Date"
#define DEF_DATE_SUFFIX_TXT			""

#define DEF_IN_TEMP_TXT				"Inside Temperature"
#define DEF_IN_TEMP_SUFFIX_TXT		"C"

#define DEF_OUT_TEMP_TXT			"Outside Temperature"
#define DEF_OUT_TEMP_SUFFIX_TXT		"C"

#define DEF_RAIN_TXT				"Rain"
#define DEF_RAIN_SUFFIX_TXT			"mm"

#define DEF_HUMIDITY_TXT			"Humidity"
#define DEF_HUMIDITY_SUFFIX_TXT		"%"

#define DEF_PRESSURE_TXT			"Pressure"
#define DEF_PRESSURE_SUFFIX_TXT		"mb"

#define DEF_WIND_DIR_TXT			"Wind Direction"
#define DEF_WIND_DIR_SUFFIX_TXT		"degrees"

#define DEF_WIND_SPEED_TXT			"Wind Speed"
#define DEF_WIND_SPEED_SUFFIX_TXT	"kmh"

#define DEF_WIND_GUST_TXT			"Wind Gust"
#define DEF_WIND_GUST_SUFFIX_TXT	"kmh"

#define DEF_FORECAST_TXT			"Forecast"
#define DEF_FORECAST_SUFFIX_TXT		""

#define DEF_TREND_TXT				"Trend"
#define DEF_TREND_SUFFIX_TXT		""

#define DEF_WIND_CHILL_TXT			"Wind Chill"
#define DEF_WIND_CHILL_SUFFIX_TXT	"C"

#define DEF_UNKNOWN1_TXT			"Unknown"
#define DEF_UNKNOWN1_SUFFIX_TXT		"?"

enum DEF_OPTIONS_TXT
	{
	FLD_MAX,
	FLD_MIN,
	FLD_CUR,

	FLD_DATA_PREFIX,
	FLD_DATA_SEPARATOR,
	FLD_UNIT_SEPARATOR,
	FLD_NO_READING,

	FLD_TIME,
	FLD_TIME_SUFFIX,

	FLD_DATE,
	FLD_DATE_SUFFIX,

	FLD_IN_TEMP,
	FLD_IN_TEMP_SUFFIX,

	FLD_OUT_TEMP,
	FLD_OUT_TEMP_SUFFIX,

	FLD_RAIN,
	FLD_RAIN_SUFFIX,

	FLD_HUMIDITY,
	FLD_HUMIDITY_SUFFIX,

	FLD_PRESSURE,
	FLD_PRESSURE_SUFFIX,

	FLD_WIND_DIR,
	FLD_WIND_DIR_SUFFIX,

	FLD_WIND_SPEED,
	FLD_WIND_SPEED_SUFFIX,

	FLD_WIND_GUST,
	FLD_WIND_GUST_SUFFIX,

	FLD_FORECAST,
	FLD_FORECAST_SUFFIX,

	FLD_TREND,
	FLD_TREND_SUFFIX,

	FLD_WIND_CHILL,
	FLD_WIND_CHILL_SUFFIX,

	FLD_UNKNOWN1,
	FLD_UNKNOWN1_SUFFIX,

	FLD_MAX_DEF_OPTION
	};

struct ws_prog_options
	{
	char *vendor;
	char *product;

	char *device;
	char *logfile;

	char *unix_path;

	int foreground;
	int fuzzy;
	int playback_rate;

	char *output_filename;
	FILE *output_fs;

	char *record_data_file;
	char *play_data_file;

	int debug_lvl;
	int data_csv;

	int out_temp_adj;
	int in_temp_adj;
	int pressure_adj;
	
	int rain_clicks_per_inch;

	char *output_txt[FLD_MAX_DEF_OPTION];
	char *default_txt[FLD_MAX_DEF_OPTION];
	};

struct datum_handler
	{
	int (*data_handler)(int *);
	int (*display_handler)();
	};

#endif
