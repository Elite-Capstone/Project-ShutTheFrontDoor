package com.theelite.devices.dao;

import com.theelite.devices.model.DeviceIp;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import javax.transaction.Transactional;
import java.util.List;

public interface IpAddressesDao extends CrudRepository<DeviceIp, String> {

    @Query("select case when count (ip.deviceId) > 0 then true else false end from DeviceIp ip where ip.deviceId = :id")
    boolean deviceIdSaved(@Value("id") String id);

    @Transactional
    @Modifying
    @Query(value = "update DeviceIp d set d.ipAddress = :ip where d.deviceId = :id")
    void updateDeviceIpAddress(@Value("id") String id, @Value("ip") String ip);

    @Transactional
    @Modifying
    @Query(value = "delete from DeviceIp dip where dip.deviceId in :ids")
    void deleteDevices(@Value("ids") List<String> ids);
}
