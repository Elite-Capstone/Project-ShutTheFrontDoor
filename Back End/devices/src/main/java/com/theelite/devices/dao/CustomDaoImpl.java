package com.theelite.devices.dao;

import com.theelite.devices.model.Account;
import com.theelite.devices.model.Device;
import com.theelite.devices.model.DeviceType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.mongodb.core.FindAndModifyOptions;
import org.springframework.data.mongodb.core.MongoOperations;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;

import java.util.Arrays;
import java.util.List;
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


    public void changeDeviceName(Device device) {
        Query query = new Query().addCriteria(Criteria.where("deviceId").is(device.getDeviceId()));
        Update update = new Update().set("deviceName", device.getDeviceName());
        mongoOperations.findAndModify(query, update, Device.class);
    }

    @Override
    public void registerDeviceToAccount(String device, String familyAccount) {
        Query query = new Query().addCriteria(Criteria.where("accountId").is(familyAccount));
        Update update = new Update().addToSet("deviceIds", device);
        mongoOperations.findAndModify(query, update, Account.class);
    }

    @Override
    public List<String> getListOfDevicesFromAccount(String accountId) {
        Query query = new Query().addCriteria(Criteria.where("accountId").is(accountId)).limit(1);
        Account account = mongoOperations.find(query, Account.class).get(0);
        return Arrays.asList(account.getDeviceIds());
    }

    @Override
    public void removeDeviceFromAccount(String device, String account) {
        Query query = new Query().addCriteria(Criteria.where("accountId").is(account).and("deviceIds").elemMatch(Criteria.where("id").is(device)));
        Update update = new Update().pull("deviceIds", new Query().addCriteria(Criteria.where("id").is(device)));
        FindAndModifyOptions options = FindAndModifyOptions.options().returnNew(true);
        mongoOperations.findAndModify(query, update, options, Account.class);
    }

    @Override
    public void familyAccountDeleted(String acc) {

        Query deleteAccountQuery = new Query().addCriteria(Criteria.where("accountId").is(acc));
        Account account = mongoOperations.find(deleteAccountQuery, Account.class).get(0);
        Query deleteDevicesQuery = new Query().addCriteria(Criteria.where("id").in(account.getDeviceIds()));
        mongoOperations.findAndRemove(deleteDevicesQuery, Device.class);
        mongoOperations.findAndRemove(deleteAccountQuery, Account.class);


    }

    @Override
    public void testDBConnection() {
        mongoOperations.count(new Query().limit(1), Account.class);
    }
}
