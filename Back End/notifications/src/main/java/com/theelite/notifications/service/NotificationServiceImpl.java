package com.theelite.notifications.service;

import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.admin.NewTopic;
import org.apache.kafka.common.internals.Topic;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.core.KafkaAdmin;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.UUID;

@Service
public class NotificationServiceImpl implements NotificationService {

    @Autowired
    private KafkaAdmin kafkaAdmin;

    @Override
    public String getHealth() {
        return "Everything seems to be fine\n";
    }

    @Override
    public List<Notification> getRecentNotifications(UUID accountId, UUID userId) {
        return null;
    }

    @Override
    public boolean publishNotification(Notification notification) {
        return false;
    }

    @Override
    public NewTopic addNewDoorIdAsTopic(UUID accountId) {
        return new NewTopic(accountId.toString(), 5, (short) 1);
    }

    @Override
    public boolean deleteDoorIdAsTopic(UUID accId) {
        return false;
    }
}
