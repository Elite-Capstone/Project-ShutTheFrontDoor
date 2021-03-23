package com.theelite.devices.service;

import com.theelite.devices.model.Device;
import org.springframework.http.ResponseEntity;

import java.util.List;

public interface DeviceService {

    boolean addNewDevice(Device device, String email, String token);

    boolean changeDeviceName(Device device);

    String getDeviceName(Device device);

    boolean deleteDevice(Device device, String email, String token);

    ResponseEntity<String> getHealth();

    List<String> getDeviceIdsForAccount(String accountId);

    List<String> getDeviceIdsForUser(String email, String token);

    void familyAccountDeleted(String acc);

    String getFamilyAccountForDevice(String device);

    List<Device> getDevices(String email, String token);
}
