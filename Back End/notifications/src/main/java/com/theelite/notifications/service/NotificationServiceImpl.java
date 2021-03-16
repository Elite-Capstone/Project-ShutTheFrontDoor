package com.theelite.notifications.service;

import com.theelite.notifications.communication.DeviceService;
import com.theelite.notifications.communication.UserService;
import com.theelite.notifications.configuration.NotificationConfigurations;
import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.ConsumerGroupListing;
import org.apache.kafka.clients.admin.DeleteTopicsResult;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.env.Environment;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.kafka.config.TopicBuilder;
import org.springframework.stereotype.Service;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import java.io.IOException;
import java.time.Duration;
import java.util.*;
import java.util.concurrent.ExecutionException;


@Service
public class NotificationServiceImpl implements NotificationService {

    private final AdminClient kafkaAdmin;

    @Value("${spring.kafka.bootstrap-servers}")
    private String bootStrapServer;

    private final UserService userService;
    private final DeviceService deviceService;

    public NotificationServiceImpl(AdminClient kafkaAdmin, Environment environment) {
        this.kafkaAdmin = kafkaAdmin;
        this.userService = this.buildRetrofitObject(environment.getProperty("user.url"), UserService.class);
        this.deviceService = this.buildRetrofitObject(environment.getProperty("device.url"), DeviceService.class);
    }


    @Override
    public ResponseEntity<String> getHealth() {
        try {
            kafkaAdmin.listTopics();

            if (userService == null)
                return new ResponseEntity<>("userService is null", HttpStatus.INTERNAL_SERVER_ERROR);
            else if (deviceService == null)
                return new ResponseEntity<>("deviceService is null", HttpStatus.INTERNAL_SERVER_ERROR);

        } catch (Exception e) {
            return new ResponseEntity<>("Error with Kafka or something", HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return new ResponseEntity<>("Everything seems to be fine", HttpStatus.OK);
    }

    @Override
    public List<Notification> getRecentNotifications(String email, String token) {
        if (!userIsLegit(email, token)) return null;
        String accountId = getFamilyAccountForUser(email);
        List<String> topics = getDeviceIdsForAccount(accountId);
        if (accountId != null && topics != null && topics.size() > 0) {

            List<Notification> notifications = new ArrayList<>();
            KafkaConsumer<String, Notification> consumer = new KafkaConsumer<>(NotificationConfigurations.getConsumerProps(email, email, bootStrapServer));

            consumer.subscribe(topics);
            ConsumerRecords<String, Notification> records = consumer.poll(Duration.ofSeconds(2));
            consumer.commitAsync();

            for (ConsumerRecord<String, Notification> r : records) {
                notifications.add(r.value());
            }
            consumer.close();
            return notifications;
        }
        return null;
    }

    @Override
    public boolean publishNotification(Notification notification) {
        KafkaProducer<String, Notification> producer = new KafkaProducer<>(NotificationConfigurations.getProducerProps(bootStrapServer));
        notification.setDate(new Date());
        producer.send(new ProducerRecord<>(notification.getDoorId().toString(), notification));
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
        deleteTopics(Collections.singletonList(accId));
        return !kafkaTopicExist(accId);
    }

    @Override
    public void deleteTopics(List<String> accId) {
        DeleteTopicsResult result = kafkaAdmin.deleteTopics(accId);
        while (!result.all().isDone());
    }


    @Override
    public List<String> getKafkaTopics() {
        List<String> topics = null;
        try {
            topics = new ArrayList<>(kafkaAdmin.listTopics().names().get());
        } catch (Exception e) {
            System.out.println(e.getMessage());
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

    private <T> T buildRetrofitObject(String url, Class<T> retrofitClass) {
        if (url == null || url.isBlank()) {
            System.out.println("The url for " + retrofitClass.getName() + " is null.");
            return null;
        }

        Retrofit retrofit = new Retrofit.Builder().baseUrl(url).addConverterFactory(GsonConverterFactory.create()).build();
        return retrofit.create(retrofitClass);
    }

    private boolean userIsLegit(String email, String token) {
        Boolean result = false;
        try {
            result = userService.validateUser(email, token).execute().body();
//            if (result == null) return false;
        } catch (NullPointerException npr) {
            System.out.println("Null pointer exception when trying to validate user");
            return false;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }

    private List<String> getDeviceIdsForAccount(String accountId) {
        List<String> deviceIds = null;
        try {
            deviceIds = deviceService.getDeviceIdsForAccount(accountId).execute().body();
        } catch (IOException | NullPointerException e) {
            System.out.println(e.getMessage());
        }
        return deviceIds;
    }

    private String getFamilyAccountForUser(String email) {
        String accountId = null;
        try {
            accountId = userService.getFamilyAccount(email).execute().body();
        } catch (NullPointerException | IOException exception) {
            System.out.println(exception.getMessage());
        }

        return accountId;
    }
}
