package com.theelite.users.service;

import com.theelite.users.dao.UserDao;
import com.theelite.users.model.User;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

@Service
public class UserServiceImpl implements UserService {

    @Autowired
    private PasswordEncoder passwordEncoder;

    @Autowired
    private UserDao userDao;

    @Override
    public boolean addUser(User user) {
        if (userDao.userExistsWithEmail(user.getEmail())) return false;
        user.setPassword(passwordEncoder.encode(user.getPassword()));
        userDao.save(user);
        return true;
    }

    @Override
    public boolean authenticateUser(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
        User savedUserInfo = userDao.findById(user.getEmail()).get();
        return passwordEncoder.matches(user.getPassword(), savedUserInfo.getPassword());
    }

    @Override
    public boolean deleteUser(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
        userDao.deleteById(user.getEmail());
        return true;
    }

    @Override
    public User getUserWithEmail(String email) {
        if (userDao.userExistsWithEmail(email)) return userDao.findById(email).get();
        else return null;
    }
}
