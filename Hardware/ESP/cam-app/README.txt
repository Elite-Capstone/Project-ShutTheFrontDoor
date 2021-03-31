In order to be able to use the SD card, the batch script "update_partition_for_sdcard.bat" must be executed. Details of the modifications are given in the batch file.

GPIO List of the ESP32-CAM in ascending order
1:  TXD0 (Reserved)
2:  
3:  RXT0 (Reserved)
4:
5:
6:  
7:  
8:  
9:  
10: 
11: 
12: 
13: 
14: 
15: 
16:

Command Format JSON:
{
  "type": "record",
  "name": "Door_Command_Request",
  "fields": [
    {
      "name": "TimeOfRequest",
      "type": "record",
      "fields": [
        {
        "name": "Year",
        "type": "int"
        },
        {
        "name": "Month",
        "type": "int"
        },
        {
        "name": "Day",
        "type": "int"
        },
        {
        "name": "Hour",
        "type": "int"
        },
        {
        "name": "Minute",
        "type": "int"
        },
        {
        "name": "Second",
        "type": "int"
        }
      ]
    },
    {
      "name": "Target_Device",
      "type": "string"
    },
    {
      "name": "Command_Request",
      "type": "string"
    },
    {
      "name": "Request_Flag",
      "type": "int"
    },
    {
      "name": "Command_Delay_ms",
      "type": "int"
    }
  ]
}