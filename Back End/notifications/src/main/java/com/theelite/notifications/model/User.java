package com.theelite.notifications.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.UUID;

//public class User {
@AllArgsConstructor
@NoArgsConstructor
@Getter
@Setter
public class User {

    private String firstName;
    private String lastName;
//    @Id
    private String email;
//    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
//    private String password;
    private UUID accountId;
//    private UserRole role;
//    }

}
