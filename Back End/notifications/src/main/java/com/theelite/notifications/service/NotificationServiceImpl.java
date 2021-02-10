package com.theelite.notifications.service;

import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.KafkaAdminClient;
import org.apache.kafka.clients.admin.NewTopic;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.config.TopicBuilder;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.ExecutionException;
import java.util.stream.Collectors;

@Service
public class NotificationServiceImpl implements NotificationService {

    @Autowired
    private AdminClient kafkaAdmin;

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
    public boolean addNewDoorIdAsTopic(UUID accountId) {
//        return new NewTopic(accountId.toString(), 5, (short) 1);
        kafkaAdmin.createTopics(Collections.singleton(TopicBuilder.name(accountId.toString())
                .partitions(5)
                .replicas((short) 1)
                .build()));
        return true;
    }

    @Override
    public boolean deleteDoorIdAsTopic(UUID accId) {
        return false;
    }

    @Override
    public List<String> getKafkaTopics() {
        List<String> topics = null;
        try {
            topics = new ArrayList<>(kafkaAdmin.listTopics().names().get());
        } catch (Exception e) {
            System.out.println(e);
        }
        return topics;
    }
}
