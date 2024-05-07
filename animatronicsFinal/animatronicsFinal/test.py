from nyct_gtfs import NYCTFeed
import datetime
import time
import serial
import struct

import time
# # Load the realtime feed from the MTA site
# f_feed = NYCTFeed("F", api_key="m0rrXh0G2Iafz8RwWBzFK94Xq3JtMo8maZmcUbzn")
# a_feed = NYCTFeed("A", api_key="m0rrXh0G2Iafz8RwWBzFK94Xq3JtMo8maZmcUbzn")
#
# trains = f_feed.filter_trips(headed_for_stop_id=["F16N", "F16S"], underway=True) + a_feed.filter_trips(headed_for_stop_id=["F16N", "F16S"], underway=True)
#
# current_datetime = datetime.datetime.now()
#
# for train in trains:
#     for stop in train.stop_time_updates:
#         if stop.stop_id == "F16S" or stop.stop_id == "F16N":
#             minutes_left = stop.arrival - current_datetime
#             difference_in_minutes = round(minutes_left.total_seconds() / 60)
#             print(train.direction, stop.stop_name, stop.stop_id, difference_in_minutes)
#             break

def get_trains_status():
    train_data = []

    # Load the realtime feed from the MTA site
    f_feed = NYCTFeed("F", api_key="m0rrXh0G2Iafz8RwWBzFK94Xq3JtMo8maZmcUbzn")
    a_feed = NYCTFeed("A", api_key="m0rrXh0G2Iafz8RwWBzFK94Xq3JtMo8maZmcUbzn")

    trains = f_feed.filter_trips(headed_for_stop_id=["F16N", "F16S"], underway=True) + a_feed.filter_trips(headed_for_stop_id=["F16N", "F16S"], underway=True)

    current_datetime = datetime.datetime.now()

    for train in trains:
        for stop in train.stop_time_updates:
            # york st is f18
            if stop.stop_id in ["F18S", "F18N"]:
                minutes_left = stop.arrival - current_datetime
                difference_in_minutes = round(minutes_left.total_seconds() / 60)

                train_data.append( {
                    "direction" : train.direction,
                    "stop" : stop.stop_name,
                    "stop_id" : stop.stop_id,
                    "countdown" : difference_in_minutes
                } )
                # print(train.direction, stop.stop_name, stop.stop_id, difference_in_minutes)
                break

    # print(train_data)
    return train_data

# serialObj = serial.Serial('com4')
# time.sleep(2)
# #
# print(serialObj)
# # print("high")
# serialObj.write(b'H')
# serialObj.write(b'100')
# data_to_write = struct.pack('B', 255)
# # #
#
# switch = True
# while True:
#     print("sending")
#     serialObj.write(data_to_write)
#     time.sleep(10)

# need to be able to handle two different kinds of serial commands.


# print("low")
# serialObj.close()
# #

def train_info(serialObj):
    while True:

        train_info = get_trains_status()
        sorted_trains = sorted(train_info, key=lambda x: x['countdown'])

        north_trains = []
        south_trains = []
        for train in sorted_trains:
            if train["direction"] == "N":
                north_trains.append(train)
            else:
                south_trains.append(train)
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        print(current_time)
        # print(sorted_trains)

        # next_2_north = north_trains[:2]
        #
        # for train in next_2_north:
        #     for key, value in train.items():
        #         print(key, value)

        next_north = north_trains[0]["countdown"]
        next_south = south_trains[0]["countdown"]

        print(next_north)
        print(next_south)
        data = f"{next_north},{next_south}\n"

        serialObj.write(data.encode())

        # if next_2_north[0]["countdown"] == 0 and next_2_north[1]["countdown"] > 10:
        #     print("push back into middle")
        # if next_2_north[0]["countdown"] == 0 and not next_2_north[1]["countdown"] > 10:
        #     spinTo = next_2_north[1]["countdown"]
        #     print(f"adjust turntable to {spinTo}")
        #
        # elif next_2_north[0]["countdown"] <= 10:
        #     spinTo = next_2_north[0]["countdown"]
        #     print(f"adjust turntable to {spinTo}")
        #


            # print("______")

        # for train in south_trains[:2]:
        #     for key, value in train.items():
        #         print(key, value)
        #     print("______")
        print("~~~")
        time.sleep(60)


serialObj = serial.Serial('com4')
time.sleep(2)

#
train_info(serialObj)
# # serialObj.write(struct.pack(b'0',b'180'))
# val1 = "0"
# val2 = "180"
#
# # print(val1.encode()+newline.encode())
# # serialObj.write(val1.encode()+newline.encode())
# # serialObj.write(val2.encode()+newline.encode())
# data = "0,180\n"
# print(data.encode())
# Send data
# serialObj.write(data.encode())



serialObj.close()
