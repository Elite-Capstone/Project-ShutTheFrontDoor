package com.theelite.users.model;

import lombok.*;
//import org.springframework.data.annotation.Id;
//import org.springframework.data.mongodb.core.mapping.Document;

import java.util.UUID;

@AllArgsConstructor
@NoArgsConstructor
@Getter
@Setter
//@Data
//@Document(collection = "Invitations")
public class Invitation {
    private String senderEmail;
    private UUID accountId;
    //    @Id
    private String receiverEmail;
}
