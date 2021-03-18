package com.theelite.users.model;


import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import org.springframework.data.annotation.Id;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

@AllArgsConstructor
@Getter
@Setter
public class Account {
    @Id
    private UUID accountId;
    private String[] deviceIds;

    public Account() {
        this.accountId = UUID.randomUUID();
        this.deviceIds = new String[0]; // New Empty Array
    }
}
