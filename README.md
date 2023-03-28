# how to use

build and flash esp32

# pull spiffs of esp32 with command
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