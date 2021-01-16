package com.theelite.users.dao;

import com.theelite.users.model.User;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.mongodb.core.MongoOperations;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;

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
    public void updateUserEmail(String user, String newEmail) {

    }

    @Override
    public void updateUserPassword(String user, String password) {

    }
}
