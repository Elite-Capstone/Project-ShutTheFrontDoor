package com.theelite.users.dao;

import com.theelite.users.model.User;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.config.EnableMongoRepositories;
import org.springframework.stereotype.Repository;

@Repository
public interface UserDao extends MongoRepository<User, String>, CustomDao {
}
