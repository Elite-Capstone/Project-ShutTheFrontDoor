package com.theelite.users.dao;

import com.theelite.users.model.*;
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
    public boolean userExistsWithEmail(String email) {
        Query query = new Query();
        query.addCriteria(Criteria.where("email").is(email)).limit(1);
        return mongoOperations.exists(query, User.class);
    }

    @Override
    public void updateUserInfo(User user) {
        Query query = new Query().addCriteria(Criteria.where("email").is(user.getEmail()));
        Update update = new Update().set("firstName", user.getFirstName()).set("lastName", user.getLastName());
        mongoOperations.findAndModify(query, update, User.class);
    }

    @Override
    public void updateUserPassword(String user, String password) {
        Query query = new Query();
        query.addCriteria(Criteria.where("email").is(user));
        Update update = new Update();
        update.set("password", password);
        mongoOperations.findAndModify(query, update, User.class);
    }

    @Override
    public void updateUserRole(String user, UserRole userRole) {
        Query query = new Query();
        query.addCriteria(Criteria.where("email").is(user));
        Update update = new Update();
        update.set("role", userRole);
        mongoOperations.findAndModify(query, update, User.class);
    }

    @Override
    public void updateUserAccount(String user, UUID accountId) {
        Query query = new Query().addCriteria(Criteria.where("email").is(user));
        Update update = new Update().set("accountId", accountId);
        mongoOperations.findAndModify(query, update, User.class);
    }

    @Override
    public void testDatabaseConnection() {
        Query query = new Query();
        query.addCriteria(Criteria.where("email").is("").not());
        mongoOperations.count(query, User.class);
    }

    @Override
    public void cancelInvitation(String user) {
        Query query = new Query();
        query.addCriteria(Criteria.where("receiverEmail").is(user));
        mongoOperations.findAndRemove(query, Invitation.class);
    }

    @Override
    public Invitation getInvitation(String user) {
        Query query = new Query();
        query.addCriteria(Criteria.where("receiverEmail").is(user));
        return mongoOperations.findOne(query, Invitation.class);
    }

    @Override
    public boolean invitationExists(String user) {
        Query query = new Query();
        query.addCriteria(Criteria.where("receiverEmail").is(user));
        return mongoOperations.exists(query, Invitation.class);
    }

    @Override
    public void saveNewInvitation(Invitation invitation) {
        mongoOperations.save(invitation);
    }

    @Override
    public boolean validateUser(String email, String token) {
        Query query = new Query();
        query.addCriteria(Criteria.where("email").is(email).and("tokens").elemMatch(Criteria.where("token").is(token))).limit(1);
        return mongoOperations.exists(query, User.class);
    }

    @Override
    public void addNewTokenToUser(String email, UserToken token) {
        Query query = new Query();
        query.addCriteria(Criteria.where("email").is(email));
        Update update = new Update();
        update.addToSet("tokens", token);
        mongoOperations.findAndModify(query, update, User.class);
    }

    @Override
    public void removeToken() {
        //TODO implement
    }

    @Override
    public long numberOfAdminsInFamilyAccount(UUID accountId) {
        Query query = new Query().addCriteria(Criteria.where("accountId").is(accountId).and("role").is(UserRole.Admin));

        return mongoOperations.count(query, User.class);
    }

    @Override
    public void saveNewFamilyAccount(Account account) {
        mongoOperations.save(account);
    }

    @Override
    public void deleteUserAccount(String email) {
        Query query = new Query();
        query.addCriteria(Criteria.where("email").is(email));
        mongoOperations.findAndRemove(query, User.class);
    }
}
