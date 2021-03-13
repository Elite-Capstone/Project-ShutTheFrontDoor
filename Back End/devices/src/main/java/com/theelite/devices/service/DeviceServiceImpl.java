package com.theelite.devices.service;

import com.theelite.devices.dao.DeviceDao;

import com.theelite.devices.model.Device;
import com.theelite.devices.model.DeviceType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.UUID;

@Service
public class DeviceServiceImpl implements DeviceService {

    @Autowired
    private DeviceDao deviceDao;

    @Override
    public boolean addNewDevice(Device device) {
        if (deviceDao.deviceExistsWithId(device.getDeviceId()))
            return false;
        deviceDao.save(device);
        return true;
    }

    @Override
    public boolean deleteDevice(Device device) {
        deviceDao.deleteById(device.getDeviceId());
        return true;
    }


    @Override
    public boolean changeDeviceName(Device device) {
        deviceDao.changeDeviceName(device);
        return true;
    }

    @Override
    public String getDeviceName(Device device) {
        return null;
    }


}
