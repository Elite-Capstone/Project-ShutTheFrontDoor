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
    private final String deviceIds = "deviceIds";
    private final String deviceId = "deviceId";
    private final String deviceName = "deviceName";
    private final String accountId = "accountId";

    @Autowired
    public CustomDaoImpl(MongoOperations mongoOperations) {
        this.mongoOperations = mongoOperations;
    }


    @Override
    public boolean deviceExistsWithId(String id) {
        Query query = new Query();
        query.addCriteria(Criteria.where(deviceId).is(id)).limit(1);
        return mongoOperations.exists(query, Device.class);
    }


    public void changeDeviceName(Device device) {
        Query query = new Query().addCriteria(Criteria.where(deviceId).is(device.getDeviceId()));
        Update update = new Update().set(deviceName, device.getDeviceName());
        mongoOperations.findAndModify(query, update, Device.class);
    }

    @Override
    public void registerDeviceToAccount(String device, String familyAccount) {
        Query query = new Query().addCriteria(Criteria.where(accountId).is(UUID.fromString(familyAccount)));
        Update update = new Update().addToSet(deviceIds, device);
        mongoOperations.findAndModify(query, update, Account.class);
    }

    @Override
    public List<String> getListOfDevicesFromAccount(String famAccountId) {
        Query query = new Query().addCriteria(Criteria.where(accountId).is(UUID.fromString(famAccountId))).limit(1);
        query.fields().include(deviceIds);
        Account account = mongoOperations.find(query, Account.class).get(0);
        String[] deviceIds = account.getDeviceIds();
        if (deviceIds != null) return Arrays.asList(account.getDeviceIds());
        else return null;
    }

    @Override
    public List<Device> getDevicesWithId(List<String> deviceIdsForAccount) {
        Query query = new Query().addCriteria(Criteria.where(deviceId).in(deviceIdsForAccount));
        return mongoOperations.find(query, Device.class);
    }

    @Override
    public void removeDeviceFromAccount(String device, String account) {
        Query query = new Query().addCriteria(Criteria.where(accountId).is(UUID.fromString(account)).and(deviceIds));
        Update update = new Update().pull(deviceIds, new Query().addCriteria(Criteria.where(deviceIds).is(device)));
        FindAndModifyOptions options = FindAndModifyOptions.options().returnNew(true);
        mongoOperations.findAndModify(query, update, options, Account.class);
    }

    @Override
    public List<String> familyAccountDeleted(String acc) {
        Query deleteAccountQuery = new Query().addCriteria(Criteria.where(accountId).is(UUID.fromString(acc)));
        Account account = mongoOperations.find(deleteAccountQuery, Account.class).get(0);
        if (account != null) {
            Query deleteDevicesQuery = new Query().addCriteria(Criteria.where(deviceIds).in((Object[]) account.getDeviceIds()));
            mongoOperations.findAndRemove(deleteDevicesQuery, Device.class);
            mongoOperations.findAndRemove(deleteAccountQuery, Account.class);
            return Arrays.asList(account.getDeviceIds());
        } else return null;
    }

    @Override
    public String getFamilyAccountForDevice(String device) {
        Query query = new Query().addCriteria(Criteria.where(deviceIds).is(device)).limit(1);
        query.fields().include(accountId);
        Account account = mongoOperations.findOne(query, Account.class);
        if (account == null) return null;
        else return account.getAccountId().toString();
    }

    @Override
    public void testDBConnection() {
        mongoOperations.count(new Query().limit(1), Account.class);
    }
}
