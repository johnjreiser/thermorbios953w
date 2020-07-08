#!/usr/bin/env python3

import os
import logging
import traceback
import csv
import datetime
import pytz
import ujson as json
import requests

logging.basicConfig(level=os.getenv("LOGLEVEL", logging.INFO))


def cel_faren(cel):
    return float(cel) * 1.8 + 32


def mm_inch(mm):
    return float(mm) / 25.4


def km_mile(km):
    return float(km) / 1.609344


def mb_inch(mb):
    return float(mb) * 0.0295301


class ws9xxd(object):
    def __init__(self, data=None, tzoffset=None):
        self.wxdata = {}
        self.tzoffset = (
            tzoffset
            or datetime.datetime.now(datetime.timezone(datetime.timedelta(0)))
            .astimezone()
            .tzinfo
        )
        if data:
            self.loadData(data)

    def loadData(self, data):
        logging.debug(type(data))
        if isinstance(data, (bytes, bytearray)):
            data = data.decode().split("\n")
        try:
            self.processData(data)
        except Exception as e:
            logging.error(traceback.print_exc())
            return False

    def processTime(self, date, time, tzoffset=None):
        """
        Take the date and time from the log (columns 1,2)
        and process them into a single time-based identifier.
        Converts the time to UTC. 
        """
        tzoffset = tzoffset or self.tzoffset
        datestr = f"{date} {time} {tzoffset}"
        dateobj = datetime.datetime.strptime(datestr, "%Y-%m-%d %H:%M:%S %Z")
        return dateobj.astimezone(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M:00")

    def processData(self, data):
        csvdata = csv.reader(data)
        for row in csvdata:
            if row:
                logging.debug(row)
                dateuniq = self.processTime(row[0], row[1])
                itemkey = "{0} {1}".format(row[4], row[5]) if row[4] else row[5]
                itemvalue = row[6]

                if dateuniq not in self.wxdata:
                    self.wxdata[dateuniq] = {}
                self.wxdata[dateuniq][itemkey] = itemvalue

    def formatData(self, dataformat="json"):
        if dataformat == "json":
            return json.dumps(self.wxdata, indent=2)
        if dataformat == "wu" or dataformat == "wunderground":
            wukeys = {
                "winddir": "Wind Direction",
                "windspeedmph": "Current Wind Speed",
                "windgustmph": "Current Wind Gust",
                "indoorhumidity": "Current Humidity",
                "tempf": "Current Outside Temperature",
                "indoortempf": "Current Inside Temperature",
                "baromin": "Current Pressure",
                "rainin": "Current Rain",
            }
            conversions = {
                "Current Wind Speed": km_mile,
                "Current Wind Gust": km_mile,
                "Current Rain": mm_inch,
                "Current Outside Temperature": cel_faren,
                "Current Inside Temperature": cel_faren,
                "Current Pressure": mb_inch,
            }

            data = []
            for datekey, obs in self.wxdata.items():
                logging.debug(datekey, obs)
                record = {"dateutc": datekey}
                for key in wukeys.keys():
                    if wukeys[key] in obs.keys():
                        logging.debug(key, obs[wukeys[key]])
                        if wukeys[key] in conversions:
                            record[key] = conversions[wukeys[key]](obs[wukeys[key]])
                        else:
                            record[key] = obs[wukeys[key]]
                data.append(record)
            return data
        else:
            return self.wxdata


class wunderground(object):
    """
    API Documentation: https://support.weather.com/s/article/PWS-Upload-Protocol?language=en_US
    """

    def __init__(self, pwsid=None, pwskey=None):
        self.pwsid = pwsid or os.getenv("PWSID")
        self.pwskey = pwskey or os.getenv("PWSKEY")
        self.url = "https://weatherstation.wunderground.com/weatherstation/updateweatherstation.php"

    def postObservations(self, data):
        try:
            data["ID"] = self.pwsid
            data["PASSWORD"] = self.pwskey
            data["action"] = "updateraw"
            logging.debug(data)
            r = requests.get(self.url, params=data)
            return r
        except Exception as e:
            logging.error(traceback.print_exc())
            return False


if __name__ == "__main__":
    import sys

    if len(sys.argv) == 1:
        wxdata = ws9xxd(sys.stdin)
    else:
        with open(sys.argv[1], "rb") as fh:
            wxdata = ws9xxd(fh.read())
    observations = wxdata.formatData("wu")
    wu = wunderground()  # get credentials from environment
    for obs in observations:
        r = wu.postObservations(obs)
        print(r.status_code, r.content)
