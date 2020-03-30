# ESP32 - BME280 data through HTTP POST with Deep Sleep

## Getting started
Complete the `WLAN_SSID` and `WLAN_PASS` variables with your network's data, and set the URL you want to send the request to on line `48`.

`TIME_TO_SLEEP` is the deep sleep timer, in seconds.

## Example JSON data
```JSON
{
    "sensor": "bme280",
    "data": [
        24.76,
        996.53,
        34.06
    ]
}