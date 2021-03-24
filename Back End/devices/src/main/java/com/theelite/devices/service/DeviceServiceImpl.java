package com.theelite.devices.service;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.theelite.devices.communication.NotifService;
import com.theelite.devices.communication.UsersService;
import com.theelite.devices.dao.DeviceDao;

import com.theelite.devices.dao.IpAddressesDao;
import com.theelite.devices.model.Device;
import com.theelite.devices.model.DeviceIp;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.env.Environment;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import javax.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.net.InetAddress;
import java.util.Arrays;
import java.util.List;

@Service
public class DeviceServiceImpl implements DeviceService {

    private final DeviceDao deviceDao;
    private final IpAddressesDao ipDao;
    private final UsersService usersService;
    private final NotifService notifService;

    public DeviceServiceImpl(DeviceDao deviceDao, IpAddressesDao ipDao, Environment environment) {
        this.deviceDao = deviceDao;
        this.ipDao = ipDao;
        this.usersService = this.buildRetrofit(environment.getProperty("user.url"), UsersService.class);
        this.notifService = this.buildRetrofit(environment.getProperty("notif.url"), NotifService.class);
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
        } catch (IOException | NullPointerException e) {
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
        } catch (IOException | NullPointerException e) {
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
        } catch (IOException | NullPointerException e) {
            System.out.println(e.getMessage());
        }
        return deviceIds;
    }

    @Override
    public ResponseEntity<String> getHealth() {
        try {
            deviceDao.testDBConnection();
            if (usersService == null)
                return new ResponseEntity<>("userService is null.", HttpStatus.INTERNAL_SERVER_ERROR);
            else if (notifService == null)
                return new ResponseEntity<>("notifService is null.", HttpStatus.INTERNAL_SERVER_ERROR);
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
    public List<Device> getDevices(String email, String token) {
        List<Device> devices = null;
        try {
            String familyAccount = getFamilyAccount(email, token);
            if (familyAccount == null || familyAccount.isBlank() || familyAccount.isEmpty()) return null;
            devices = deviceDao.getDevicesWithId(getDeviceIdsForAccount(familyAccount));

        } catch (IOException | NullPointerException e) {
            e.printStackTrace();
        }
        return devices;
    }

    @Override
    public void familyAccountDeleted(String acc) {
        List<String> deviceIds = deviceDao.familyAccountDeleted(acc);
        try {
            notifService.deviceDeleted(deviceIds).execute();
            ipDao.deleteDevices(deviceIds);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    @Override
    public String saveIpAddress(String deviceId, HttpServletRequest request) {
        if (request == null) {
            return "0.0.0.0";
        }
        if (deviceDao.deviceExistsWithId(deviceId)) {
            // Update IP Address
            if (ipDao.deviceIdSaved(deviceId)) ipDao.updateDeviceIpAddress(deviceId, request.getRemoteAddr());
            else ipDao.save(new DeviceIp(deviceId, request.getRemoteAddr()));
        }
        return request.getRemoteAddr();
    }

    private <T> T buildRetrofit(String url, Class<T> tClass) {
        if (url == null || url.isBlank() || url.isEmpty()) {
            System.out.println("Url for " + tClass.getName() + " is null.");
            return null;
        }
        Gson gson = new GsonBuilder()
                .setLenient()
                .create();
        Retrofit retrofit = new Retrofit.Builder().baseUrl(url).addConverterFactory(GsonConverterFactory.create(gson)).build();
        return retrofit.create(tClass);
    }

    public String getIpAddressForDevice(String deviceId, String email, String token) {
        try {
            String familyAccount = getFamilyAccount(email, token);
            if (familyAccount == null || familyAccount.isEmpty() || familyAccount.isBlank()) return null;
            if (ipDao.deviceIdSaved(deviceId)) return ipDao.findById(deviceId).get().getIpAddress();
        } catch (IOException | NullPointerException e) {
            System.out.println(e.getMessage());
        }
        return null;
    }
}
