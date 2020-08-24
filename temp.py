import sys
import Adafruit_DHT
import time
import datetime
from time import sleep
from gpiozero import Buzzer, InputDevice
from firebase.firebase import FirebaseApplication, FirebaseAuthentication

no_rain = InputDevice(18)
DSN = 'https://sensor-19569.firebaseio.com/'
firebase = FirebaseApplication(DSN, None)

if __name__ == '__main__':
    while True:
        flag = False
        humidity, temperature = Adafruit_DHT.read_retry(11, 4)
        print 'Temperature: {0:0.1f} Humidity: {1:0.1f} %'.format(temperature, humidity)
        if not no_rain.is_active:
            flag = True
            print("It's raining!!")
        
        data = {'Temperature': temperature, 'Humidity': humidity, 'Rain': flag}
        snapshot = firebase.post('/Sensor', data)
        sleep(1)
