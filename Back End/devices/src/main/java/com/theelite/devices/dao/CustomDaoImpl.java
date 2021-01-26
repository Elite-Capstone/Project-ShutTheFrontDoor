package com.theelite.devices.dao;

import com.theelite.devices.model.Device;
import com.theelite.devices.model.Device;
import com.theelite.devices.model.DeviceType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.mongodb.core.MongoOperations;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;

import java.util.UUID;

public class CustomDaoImpl implements CustomDao {
    private final MongoOperations mongoOperations;

    @Autowired
    public CustomDaoImpl(MongoOperations mongoOperations) {
        this.mongoOperations = mongoOperations;
    }


    @Override
    public boolean deviceExistsWithId(String id) {
        Query query = new Query();
        query.addCriteria(Criteria.where("id").is(id)).limit(1);
        return mongoOperations.exists(query, Device.class);
    }


    public boolean changeDeviceName(Device device){
        return false;
    }


    public String getDeviceName(Device device){
        return null;
    }

}
