package com.theelite.devices.controller;

import com.theelite.devices.model.Device;
import com.theelite.devices.service.DeviceServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("device")
public class DeviceController {

    @Autowired
    private DeviceServiceImpl DeviceService;

    @PutMapping("/new")
    public boolean addNewDevice(@RequestBody Device device) { return DeviceService.addNewDevice(device); }

    @PostMapping("/updateName")
    public boolean changeDeviceName(@RequestBody Device device) {
        return DeviceService.changeDeviceName(device);
    }

    @GetMapping("/getName")
    public String getDeviceName(@RequestBody Device device) { return DeviceService.getDeviceName(device); }

    @DeleteMapping("/")
    public boolean deleteDevice(@RequestBody Device device) {
        return DeviceService.deleteDevice(device);
    }


}
