package com.theelite.users.service;

import com.theelite.users.model.User;
import org.springframework.stereotype.Service;

@Service
public class UserServiceImpl implements UserService {


    @Override
    public boolean addUser(User user) {
        return false;
    }

    @Override
    public boolean authenticateUser(User user) {
        return false;
    }

    @Override
    public boolean deleteUser(User user) {
        return false;
    }
}
