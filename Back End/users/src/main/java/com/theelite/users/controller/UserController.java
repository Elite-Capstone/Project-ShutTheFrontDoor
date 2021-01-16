package com.theelite.users.controller;


import com.theelite.users.model.Account;
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

    @PostMapping("/auth")
    public boolean authenticateUser(@RequestBody User user) {
        return userService.authenticateUser(user);
    }

    @PostMapping("/role/{role}")
    public boolean updateUserRole(@RequestBody User user, @PathVariable int role) {
        return userService.modifyUserRole(user, role);
    }

    @DeleteMapping("/")
    public boolean deleteUser(@RequestBody User user) {
        return userService.deleteUser(user);
    }

    @GetMapping("/")
    public User getUserWithEmail(@RequestBody User user) {
        return userService.getUserWithEmail(user.getEmail());
    }
}
