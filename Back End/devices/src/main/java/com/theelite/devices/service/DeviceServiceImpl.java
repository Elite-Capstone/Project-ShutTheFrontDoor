package com.theelite.devices.service;

import com.theelite.devices.communication.NotifService;
import com.theelite.devices.communication.UsersService;
import com.theelite.devices.dao.DeviceDao;

import com.theelite.devices.model.Device;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import retrofit2.Retrofit;

import java.io.IOException;
import java.util.List;

@Service
public class DeviceServiceImpl implements DeviceService {

    private final DeviceDao deviceDao;
    @Value("${users.url}")
    private String usersUrl;
    @Value("${notif.url}")
    private String notifUrl;
    private UsersService usersService;
    private NotifService notifService;

    public DeviceServiceImpl(DeviceDao deviceDao) {
        this.deviceDao = deviceDao;
        this.usersService = this.buildRetrofit(usersUrl, UsersService.class);
        this.notifService = this.buildRetrofit(notifUrl, NotifService.class);
    }

    @Override
    public boolean addNewDevice(Device device, String email, String token) {
        try {
            String familyAccount = getFamilyAccount(email, token);
            if (familyAccount == null || familyAccount.isEmpty() || familyAccount.isBlank()) return false;
            if (deviceDao.deviceExistsWithId(device.getDeviceId())) return false;
            deviceDao.save(device);
            deviceDao.registerDeviceToAccount(device.getDeviceId(), familyAccount);
            notifService.newDeviceAdded(device.getDeviceId()).execute();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
        return true;
    }

    @Override
    public boolean deleteDevice(Device device, String email, String token) {
        try {
            String familyAccount = getFamilyAccount(email, token);
            if (familyAccount == null || familyAccount.isEmpty() || familyAccount.isBlank()) return false;
            deviceDao.deleteById(device.getDeviceId());
            deviceDao.removeDeviceFromAccount(device.getDeviceId(), familyAccount);
            notifService.deviceDeleted(device.getDeviceId()).execute();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
        return true;
    }

    private String getFamilyAccount(String email, String token) throws IOException {
        Boolean userIsLegit = usersService.validateUser(email, token).execute().body();
        if (userIsLegit == null || !userIsLegit) return null;
        return usersService.getFamilyAccountFor(email).execute().body();
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


    @Override
    public List<String> getDeviceIdsForAccount(String accountId) {
        return deviceDao.getListOfDevicesFromAccount(accountId);
    }

    @Override
    public List<String> getDeviceIdsForUser(String email, String token) {
        List<String> deviceIds = null;
        try {
            String familyAccount = getFamilyAccount(email, token);
            if (familyAccount == null || familyAccount.isEmpty() || familyAccount.isBlank()) return null;
            deviceIds = getDeviceIdsForAccount(familyAccount);
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
        return deviceIds;
    }

    @Override
    public ResponseEntity<String> getHealth() {
        try {
            deviceDao.testDBConnection();
        } catch (Exception e) {
            return new ResponseEntity<>("DB Connection error " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return new ResponseEntity<>("App seems fine", HttpStatus.OK);
    }

    @Override
    public String getFamilyAccountForDevice(String device) {
        if (!deviceDao.deviceExistsWithId(device)) return null;
        return deviceDao.getFamilyAccountForDevice(device);
    }

    @Override
    public void familyAccountDeleted(String acc) {
        deviceDao.familyAccountDeleted(acc);
    }

    private <T> T buildRetrofit(String url, Class<T> tClass) {
        if (url == null || url.isBlank() || url.isEmpty()) return null;
        Retrofit retrofit = new Retrofit.Builder().baseUrl(url).build();
        return retrofit.create(tClass);
    }
}
