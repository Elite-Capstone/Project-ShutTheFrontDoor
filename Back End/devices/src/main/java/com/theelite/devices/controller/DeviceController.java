package com.theelite.devices.controller;

import com.theelite.devices.model.Device;
import com.theelite.devices.service.DeviceServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.UUID;

@RestController
@RequestMapping("device")
public class DeviceController {

    @Autowired
    private DeviceServiceImpl deviceService;

    @PutMapping("/new/{email}/{token}")
    public boolean addNewDevice(@RequestBody Device device, @PathVariable("email") String email, @PathVariable("token") String token) {
        return deviceService.addNewDevice(device, email, token);
    }

    @PostMapping("/updateName")
    public boolean changeDeviceName(@RequestBody Device device) {
        return deviceService.changeDeviceName(device);
    }

    @GetMapping("/getName")
    public String getDeviceName(@RequestBody Device device) {
        return deviceService.getDeviceName(device);
    }

    @DeleteMapping("/{email}/{token}")
    public boolean deleteDevice(@RequestBody Device device, @PathVariable("email") String email, @PathVariable("token") String token) {
        return deviceService.deleteDevice(device, email, token);
    }

    @GetMapping("/getDeviceIdsForAccount/{accId}")
    public List<String> getDeviceIdsForAccount(@PathVariable String accId) {
        return deviceService.getDeviceIdsForAccount(accId);
    }

    @GetMapping("/getDeviceIdsForUser/{email}/{token}")
    public List<String> getDeviceName(@PathVariable("email") String email, @PathVariable("token") String token) {
        return deviceService.getDeviceIdsForUser(email, token);
    }

    @GetMapping("/health")
    public ResponseEntity<String> getHealth() {
        return deviceService.getHealth();
    }

}
