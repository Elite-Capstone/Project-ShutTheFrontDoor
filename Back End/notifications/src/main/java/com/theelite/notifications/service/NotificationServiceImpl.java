package com.theelite.notifications.service;

import com.theelite.notifications.configuration.NotificationConfigurations;
import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.KafkaClient;
import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.ConsumerGroupListing;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.kafka.config.TopicBuilder;
import org.springframework.stereotype.Service;

import java.time.Duration;
import java.util.*;
import java.util.concurrent.ExecutionException;


@Service
public class NotificationServiceImpl implements NotificationService {

    @Autowired
    private AdminClient kafkaAdmin;

    @Value("${spring.kafka.bootstrap-servers}")
    private String bootStrapServer;

    @Override
    public String getHealth() {
        return "Everything seems to be fine\n";
    }

    @Override
    public List<Notification> getRecentNotifications(String accountId, String userId) {
        //TODO retrieve notification as pojo
        String topic = "00b288a8-3db1-40b5-b30f-532af4e12f4b";
        KafkaConsumer<String, String> consumer = new KafkaConsumer<>(NotificationConfigurations.getConsumerProps(accountId, userId, bootStrapServer));
        consumer.subscribe(Collections.singleton(topic));
        ConsumerRecords<String, String> records = consumer.poll(Duration.ofSeconds(2));
        consumer.commitAsync();
        List<Notification> notifications = new ArrayList<>();
        for (ConsumerRecord<String, String> r : records) {
            notifications.add(new Notification(r.value(), new Date(), UUID.fromString(topic), UUID.fromString(accountId)));
        }
        consumer.close();
        return notifications;
    }

    @Override
    public boolean publishNotification(Notification notification) {
        // TODO publish notification as pojo
        KafkaProducer<String, String> producer = new KafkaProducer<>(NotificationConfigurations.getProducerProps(bootStrapServer));
        producer.send(new ProducerRecord<>(notification.getDoorId().toString(), notification.getNotification()));
        producer.close();
        return true;
    }

    @Override
    public boolean addNewDoorIdAsTopic(String accountId) {
        kafkaAdmin.createTopics(Collections.singleton(TopicBuilder.name(accountId)
                .partitions(5)
                .replicas((short) 1)
                .build()));
        return true;
    }

    @Override
    public boolean deleteDoorIdAsTopic(String accId) {
        kafkaAdmin.deleteTopics(Collections.singleton(accId));
        return !kafkaTopicExist(accId);
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

    private boolean kafkaTopicExist(String topic) {
        return getKafkaTopics().contains(topic);
    }

    @Override
    public boolean createConsumerGroup(String accId) {
        try {
            kafkaAdmin.listConsumerGroups().all().get().add(new ConsumerGroupListing(accId, true));
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
        return consumerGroupExistsWithId(accId);
    }

    @Override
    public boolean deleteConsumerGroup(String accId) {
        kafkaAdmin.deleteConsumerGroups(Collections.singleton(accId));
        return !consumerGroupExistsWithId(accId);
    }

    private boolean consumerGroupExistsWithId(String io) {
        boolean result = false;
        try {
            result = kafkaAdmin.listConsumerGroups().all().get().stream().anyMatch(t -> io.equals(t.toString()));
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
        return result;
    }
}
