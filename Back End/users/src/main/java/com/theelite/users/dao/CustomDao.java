package com.theelite.users.dao;

import com.theelite.users.model.User;
import com.theelite.users.model.UserRole;

import java.util.UUID;

public interface CustomDao {
    boolean userExistsWithEmail(String email);
    void updateUserAccount(String user, UUID accountId);
    void updateUserInfo(User user);
    void updateUserPassword(String user, String password);
    void updateUserRole(String user, UserRole userRole);
    long testDatabaseConnection();
}
