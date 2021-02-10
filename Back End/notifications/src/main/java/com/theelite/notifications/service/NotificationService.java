package com.theelite.notifications.service;

import com.theelite.notifications.model.Notification;

import java.util.List;

//@Service
public interface NotificationService {
    String getHealth();
    List<Notification> getRecentNotifications(String accountId, String userId);
    boolean publishNotification(Notification notification);
    boolean addNewDoorIdAsTopic(String doorId);
    boolean deleteDoorIdAsTopic(String doorId);
    boolean createConsumerGroup(String accId);
    boolean deleteConsumerGroup(String accId);
    List<String> getKafkaTopics();
}
