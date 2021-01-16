package com.theelite.users.dao;

public interface CustomDao {
    boolean userExistsWithEmail(String email);
    void updateUserEmail(String user, String newEmail);
    void updateUserPassword(String user, String password);
}
