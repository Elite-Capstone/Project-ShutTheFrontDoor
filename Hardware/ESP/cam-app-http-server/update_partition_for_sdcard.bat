REM This modifies the file <ESP-IDF>/components/partition_table/partitions_singleapp.csv
REM It modifies the value 1M of "factory, app, factory" to 0x180000
(
echo # Name,   Type, SubType, Offset,  Size, Flags
echo # Note: if you have increased the bootloader size, make sure to update the offsets to avoid overlap
echo nvs,      data, nvs,     ,        0x6000,
echo phy_init, data, phy,     ,        0x1000,
echo factory,  app,  factory, ,        0x180000,
) > ../esp-idf/components/partition_table/partitions_singleapp.csv