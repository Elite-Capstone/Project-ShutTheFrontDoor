package com.theelite.notifications.configuration;

import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.KafkaAdminClient;
import org.springframework.boot.autoconfigure.kafka.KafkaProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.kafka.core.ConsumerFactory;
import org.springframework.kafka.core.DefaultKafkaConsumerFactory;
import org.springframework.kafka.core.KafkaAdmin;

import java.util.Properties;
import java.util.UUID;

@Configuration
public class NotificationConfigurations {

    @Bean
    public ConsumerFactory<UUID, Notification> consumerFactory(KafkaProperties kafkaProperties) {
        return new DefaultKafkaConsumerFactory<>(kafkaProperties.buildConsumerProperties());
    }

    @Bean
    public KafkaAdmin kafkaAdmin(KafkaProperties kafkaProperties) {
        return new KafkaAdmin(kafkaProperties.buildAdminProperties());
    }

    @Bean
    public AdminClient kafkaAdminClient(KafkaProperties kafkaProperties) {

//        Properties properties = new Properties();
//        properties.put("bootstrap.servers", kafkaProperties.);
//        properties.put("connections.max.idle.ms", 10000);
//        properties.put("request.timeout.ms", 5000);
//        try (
//                AdminClient client =
        return KafkaAdminClient.create(kafkaProperties.buildAdminProperties());

//        ) {
//            CreateTopicsResult result = client.createTopics(Arrays.asList(
//                    new NewTopic("storage-topic", 1, (short) 1),
//                    new NewTopic("global-id-topic", 1, (short) 1),
//                    new NewTopic("snapshot-topic", 1, (short) 1)
//            ));
//            try {
//                result.all().get();
//            } catch (InterruptedException | ExecutionException e) {
//                throw new IllegalStateException(e);
//            }
//        }
    }
}
