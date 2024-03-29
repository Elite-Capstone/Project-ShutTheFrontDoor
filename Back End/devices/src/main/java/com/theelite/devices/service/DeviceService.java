package com.theelite.devices.service;

import com.theelite.devices.model.Command;
import com.theelite.devices.model.Device;
import com.theelite.devices.model.Status;
import org.springframework.http.ResponseEntity;

import javax.servlet.http.HttpServletRequest;
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

    String saveIpAddress(String deviceId, HttpServletRequest request);

    ResponseEntity<String> publishCommand(Command command, String email, String token);

    boolean updateDeviceStatus(String id, Status status);
}
