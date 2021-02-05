package com.theelite.notifications.service;

import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.admin.NewTopic;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.UUID;

//@Service
public interface NotificationService {
    String getHealth();
    List<Notification> getRecentNotifications(UUID accountId, UUID userId);
    boolean publishNotification(Notification notification);
    NewTopic addNewDoorIdAsTopic(UUID doorId);
    boolean deleteDoorIdAsTopic(UUID doorId);
}
