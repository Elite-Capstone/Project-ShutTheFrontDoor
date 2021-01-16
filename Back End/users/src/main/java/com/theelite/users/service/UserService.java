package com.theelite.users.service;

import com.theelite.users.model.User;

public interface UserService {

    boolean addUser(User user);
    boolean authenticateUser(User user);
    boolean deleteUser(User user);
    User getUserWithEmail(String email);
    boolean modifyUserRole(User user, int role);

}
