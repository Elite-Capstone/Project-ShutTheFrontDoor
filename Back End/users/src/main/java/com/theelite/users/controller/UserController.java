package com.theelite.users.controller;


import com.theelite.users.service.UserServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("user")
public class UserController {

    @Autowired
    private UserServiceImpl userService;


    public boolean addNewUser() {
        return false;
    }




}
