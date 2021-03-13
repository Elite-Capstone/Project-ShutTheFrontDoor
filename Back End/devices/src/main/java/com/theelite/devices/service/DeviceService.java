package com.theelite.devices.service;

import com.theelite.devices.model.Device;

public interface DeviceService {

    boolean addNewDevice(Device device);

    boolean changeDeviceName(Device device);

    String getDeviceName(Device device);

    boolean deleteDevice(Device device);


}
