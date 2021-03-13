package com.theelite.devices.dao;

import com.theelite.devices.model.Device;
import com.theelite.devices.model.DeviceType;

import java.util.UUID;

public interface CustomDao {

    boolean deviceExistsWithId(String id);
    void changeDeviceName(Device device);
}
