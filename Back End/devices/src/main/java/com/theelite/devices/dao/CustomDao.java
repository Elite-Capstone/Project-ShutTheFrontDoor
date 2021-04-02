package com.theelite.devices.dao;

import com.theelite.devices.model.Device;
import com.theelite.devices.model.Status;

import java.util.List;

public interface CustomDao {

    boolean deviceExistsWithId(String id);

    void changeDeviceName(Device device);

    void registerDeviceToAccount(String device, String familyAccount);

    List<String> getListOfDevicesFromAccount(String account);

    void removeDeviceFromAccount(String device, String account);

    void testDBConnection();

    List<String> familyAccountDeleted(String acc);

    String getFamilyAccountForDevice(String device);

    List<Device> getDevicesWithId(List<String> deviceIdsForAccount);

    List<String> getDeviceIds();

    void updateDeviceStatus(String id, Status status);
}
