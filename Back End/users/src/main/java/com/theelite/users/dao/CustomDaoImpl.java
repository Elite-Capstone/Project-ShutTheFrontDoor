package com.theelite.users.dao;

import com.theelite.users.model.User;
import com.theelite.users.model.UserRole;
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
}
