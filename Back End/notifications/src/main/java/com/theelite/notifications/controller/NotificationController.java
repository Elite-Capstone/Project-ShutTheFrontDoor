package com.theelite.notifications.controller;

import com.theelite.notifications.service.NotificationService;
import com.theelite.notifications.service.NotificationServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;
import java.util.UUID;

@RestController
@RequestMapping("/notif")
public class NotificationController {

    @Autowired
    private NotificationServiceImpl service;

    @GetMapping("/")
    public String getMicroServiceHealth(){
        return service.getHealth();
    }

    @GetMapping("/{doorId}")
    public boolean addDoorIdAsTopic(@PathVariable String doorId){
        System.out.println("Received Request to create topic " + doorId);
        service.addNewDoorIdAsTopic(UUID.fromString(doorId));
        return true;
    }

    @GetMapping("/getTopics")
    public List<String> getKafkaTopics(){
        return service.getKafkaTopics();
    }
}
