package com.theelite.users.dao;

import com.theelite.users.model.UserRole;

public interface CustomDao {
    boolean userExistsWithEmail(String email);
    void updateUserEmail(String user, String newEmail);
    void updateUserPassword(String user, String password);
    void updateUserRole(String user, UserRole userRole);
}
