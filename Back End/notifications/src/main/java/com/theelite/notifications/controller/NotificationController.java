package com.theelite.notifications.controller;

import com.theelite.notifications.model.Notification;
import com.theelite.notifications.service.NotificationService;
import com.theelite.notifications.service.NotificationServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.UUID;

@RestController
@RequestMapping("/notif")
public class NotificationController {

    @Autowired
    private NotificationServiceImpl service;

    @GetMapping("/")
    public ResponseEntity getMicroServiceHealth() {
        return service.getHealth();
    }

    @GetMapping("newTopic/{doorId}")
    public boolean addDoorIdAsTopic(@PathVariable String doorId) {
        System.out.println("Received Request to create topic " + doorId);
        return service.addNewDoorIdAsTopic(doorId);
    }

    @GetMapping("deleteTopic/{doorId}")
    public boolean deleteDoorIdAsTopic(@PathVariable String doorId) {
        System.out.println("Received Request to delete topic " + doorId);
        return service.deleteDoorIdAsTopic(doorId);
    }

    @GetMapping("newConsumerGroup/{consuGroup}")
    public boolean addAccountAsNewConsumerGroup(@PathVariable String consuGroup) {
        System.out.println("Received Request to create ConsumerGroup " + consuGroup);
        return service.createConsumerGroup(consuGroup);
    }

    @GetMapping("deleteConsumerGroup/{consuGroup}")
    public boolean deleteAccountAsConsumerGroup(@PathVariable String consuGroup) {
        System.out.println("Received Request to delete ConsumerGroup " + consuGroup);
        return service.deleteConsumerGroup(consuGroup);
    }

    @GetMapping("/getTopics")
    public List<String> getKafkaTopics() {
        return service.getKafkaTopics();
    }

    @GetMapping("/getRecent/{accId}/{userId}")
    public List<Notification> getRecentNotifications(@PathVariable String accId, @PathVariable String userId) {
        return service.getRecentNotifications(accId, userId);
    }

    @PutMapping("/newNotif")
    public void publishNotification(@RequestBody Notification notification) {
        service.publishNotification(notification);
    }

    @PutMapping("/newNotif/{doorId}/{event}")
    public void publishNotification(@PathVariable("doorId") String doorId, @PathVariable("event") String event) {
        Notification notification = new Notification();
        notification.setNotification(event);
        notification.setDoorId(UUID.fromString(doorId));
        service.publishNotification(notification);
    }
}
