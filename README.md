# how to use

Build and flash ESP32. The location of spiffs partition in flash should be the same as a Duo so you should be able to just flash a Duo and look through its spiffs.

# pull spiffs off of esp32 with command
```
./parttool.py -p /dev/cu.SLAB_USBtoUART read_partition --partition-name <'spiffs' or 'storage'> --output path/to/spiffs.img
```
or 
```
./parttool.py -p /dev/cu.SLAB_USBtoUART read_partition --partition-type data --partition-subtype spiffs --output path/to/spiffs.img
```

# put spiffs on this esp32 with command
```
./parttool.py -p /dev/cu.SLAB_USBtoUART read_partition --partition-name <'spiffs' or 'storage'> --input path/to/spiffs.img
```
or 
```
./parttool.py -p /dev/cu.SLAB_USBtoUART read_partition --partition-type data --partition-subtype spiffs --input path/to/spiffs.img
```