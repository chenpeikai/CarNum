#!/usr/bin/python
import re
import json
import time

pattern = re.compile(r"\D+:\d+")
car_dict = {"time":time.asctime( time.localtime(time.time()) )}
file = open("car.json","wr")
tick = time.time()
while 1:
    print time.time()-tick
    if (time.time()-tick) > 60:
        tick = time.time()
        print "write to json"
        car_dict["time"] = time.asctime( time.localtime(time.time()) )
        jsonstr = json.dumps(car_dict)
        file.write(jsonstr)
    data = raw_input()
    match = pattern.match(data)
    if match:
        car = data.split(':')
        if car_dict.has_key(car[0]):
            if car_dict[car[0]] != car[1]:
                car_dict[car[0]] = car[1]
        else:
            car_dict[car[0]] = car[1]
            print "add new item"
    else:
        print "not match"
