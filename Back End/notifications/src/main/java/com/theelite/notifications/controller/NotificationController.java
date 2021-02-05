package com.theelite.notifications.controller;

import com.theelite.notifications.service.NotificationService;
import com.theelite.notifications.service.NotificationServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/notif")
public class NotificationController {

    @Autowired
    private NotificationServiceImpl service;

    @GetMapping("/")
    public String getMicroServiceHealth(){
        return service.getHealth();
    }
}
