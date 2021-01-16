package com.theelite.users.controller;


import com.theelite.users.model.User;
import com.theelite.users.service.UserServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("user")
public class UserController {

    @Autowired
    private UserServiceImpl userService;

    @PutMapping("/new")
    public boolean addNewUser(@RequestBody User user) {
        return userService.addUser(user);
    }

    @GetMapping("/auth")
    public boolean authenticateUser(@RequestBody User user) {
        return userService.authenticateUser(user);
    }

    @DeleteMapping("/")
    public boolean deleteUser(@RequestBody User user) {
        return userService.deleteUser(user);
    }


}
