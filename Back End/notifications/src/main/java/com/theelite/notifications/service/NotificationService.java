package com.theelite.notifications.service;

import com.theelite.notifications.model.Notification;
import org.springframework.http.ResponseEntity;

import java.util.List;

//@Service
public interface NotificationService {
    ResponseEntity<String> getHealth();
    List<Notification> getRecentNotifications(String accountId, String userId);
    boolean publishNotification(Notification notification);
    boolean addNewDoorIdAsTopic(String doorId);
    boolean deleteDoorIdAsTopic(String doorId);
    boolean createConsumerGroup(String accId);
    boolean deleteConsumerGroup(String accId);
    List<String> getKafkaTopics();

    void deleteTopics(List<String> topics);
}
