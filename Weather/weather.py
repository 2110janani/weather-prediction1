import requests
import serial
import time

# OpenWeatherMap API details
API_KEY = "YOUR_API_KEY"
CITY = "YOUR_CITY_NAME"
COUNTRY_CODE = "YOUR_COUNTRY_CODE"

# Set up serial communication with Arduino (Adjust port and baud rate)
arduino = serial.Serial(port='COM3', baudrate=9600, timeout=1)

def get_weather():
    try:
        response = requests.get(URL)
        data = response.json()

        # Extract weather condition from the JSON response
        weather = data['weather'][0]['main']
        return weather
    except Exception as e:
        print("Error fetching weather:", e)
        return None

def send_signal_to_arduino(signal):
    arduino.write(signal.encode())
    time.sleep(1)  # Give Arduino some time to process

def main():
    while True:
        weather = get_weather()

        if weather:
            print(f"Weather: {weather}")

            if weather == "Clear":
                print("Sending Sunny signal to Arduino...")
                send_signal_to_arduino('S')  # 'S' for Sunny
            elif weather == "Clouds":
                print("Sending Cloudy signal to Arduino...")
                send_signal_to_arduino('C')  # 'C' for Cloudy
            elif weather == "Rain":
                print("Sending Rain signal to Arduino...")
                send_signal_to_arduino('R')  # 'R' for Rain
            else:
                print("Unknown weather condition")
        else:
            print("Could not retrieve weather data")

        # Wait for 60 seconds before the next request
        time.sleep(60)

if __name__ == "__main__":
    main()
