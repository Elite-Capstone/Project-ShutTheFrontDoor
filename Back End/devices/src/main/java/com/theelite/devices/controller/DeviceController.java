package com.theelite.devices.controller;

import com.theelite.devices.model.Device;
import com.theelite.devices.service.DeviceServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

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
    public List<String> getDeviceIdsForUser(@PathVariable("email") String email, @PathVariable("token") String token) {
        return deviceService.getDeviceIdsForUser(email, token);
    }

    @GetMapping("/getFamilyAccountForDevice/{device}")
    public String getFamAccForDeviceWithId(@PathVariable String device) {
        return deviceService.getFamilyAccountForDevice(device);
    }

    @DeleteMapping("/familyAccountDeleted/{acc}")
    public void familyAccountDeleted(@PathVariable String acc) {
        deviceService.familyAccountDeleted(acc);
    }

    @GetMapping("/health")
    public ResponseEntity<String> getHealth() {
        return deviceService.getHealth();
    }

    @GetMapping("/{email}/{token}")
    public List<Device> getDevices(@PathVariable("email") String email, @PathVariable("token") String token) {
        return deviceService.getDevices(email, token);
    }

    @GetMapping("/{id}")
    public String getMyId(@PathVariable String id, HttpServletRequest request) {
        return deviceService.saveIpAddress(id, request);
    }

    @GetMapping("/{id}/address/{email}/{token}")
    public String getIdForDevice(@PathVariable("id")String deviceId, @PathVariable("email") String email, @PathVariable("token") String token){
        return deviceService.getIpAddressForDevice(deviceId, email, token);
    }
}
