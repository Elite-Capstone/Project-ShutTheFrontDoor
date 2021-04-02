package com.theelite.devices.dao;

import com.theelite.devices.model.Device;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface DeviceDao extends MongoRepository<Device, String>, CustomDao {
}
