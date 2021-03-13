package com.theelite.notifications.service;

import com.theelite.notifications.communication.UserService;
import com.theelite.notifications.configuration.NotificationConfigurations;
import com.theelite.notifications.model.Notification;
import com.theelite.notifications.model.User;
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
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.kafka.config.TopicBuilder;
import org.springframework.stereotype.Service;
import retrofit2.Retrofit;

import java.io.IOException;
import java.time.Duration;
import java.util.*;
import java.util.concurrent.ExecutionException;


@Service
public class NotificationServiceImpl implements NotificationService {

    public NotificationServiceImpl(AdminClient kafkaAdmin) {
        this.kafkaAdmin = kafkaAdmin;
        this.buildRetrofitObjects();
    }

    private final AdminClient kafkaAdmin;

    @Value("${user.ms.url}")
    private String userMsUrl;

    @Value("${spring.kafka.bootstrap-servers}")
    private String bootStrapServer;
    private UserService userService = null;

    @Override
    public ResponseEntity<String> getHealth() {
        try {
            kafkaAdmin.listTopics();
        } catch (Exception e) {
            return new ResponseEntity<>("Error with Kafka or something", HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return new ResponseEntity<>("Everything seems to be fine", HttpStatus.OK);
    }

    @Override
    public List<Notification> getRecentNotifications(String email, String token) {
        if (!userIsLegit(email, token)) return null;
        String accountId = getFamilyAccountForUser(email);
        if (accountId != null) {
            //TODO retrieveAll topic -- doorIds from accountId
            String topic = "00b288a8-3db1-40b5-b30f-532af4e12f4b";
            KafkaConsumer<String, Notification> consumer = new KafkaConsumer<>(NotificationConfigurations.getConsumerProps(email, accountId, bootStrapServer));
            consumer.subscribe(Collections.singleton(topic));
            ConsumerRecords<String, Notification> records = consumer.poll(Duration.ofSeconds(2));
            consumer.commitAsync();
            List<Notification> notifications = new ArrayList<>();
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
        kafkaAdmin.deleteTopics(Collections.singleton(accId));
        return !kafkaTopicExist(accId);
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

    private void buildRetrofitObjects() {
        if (userMsUrl == null || userMsUrl.equals("")) return;
        Retrofit retrofit = new Retrofit.Builder().baseUrl(userMsUrl).build();
        userService = retrofit.create(UserService.class);
    }

    private boolean userIsLegit(String email, String token) {
        boolean result = false;
        try {
            result = userService.validateUser(email, token).execute().body();
        } catch (NullPointerException npr) {
            System.out.println("Null pointer exception when trying to validate user");
            return false;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }

    private String getFamilyAccountForUser(String email) {
        String accountId = null;
        try {
            accountId = userService.getFamilyAccount(email).execute().body();
        } catch (NullPointerException | IOException nullPointerException) {
            System.out.println(nullPointerException.getMessage());
        }

        return accountId;
    }
}
