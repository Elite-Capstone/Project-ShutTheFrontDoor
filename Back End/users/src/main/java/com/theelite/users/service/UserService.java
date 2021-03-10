package com.theelite.users.service;

import com.theelite.users.model.Invitation;
import com.theelite.users.model.User;
import org.springframework.http.ResponseEntity;

public interface UserService {

    String addUser(User user);

    String authenticateUser(User user);

    boolean updateUserInfo(User user);

    boolean deleteUser(User user);

    User getUserWithEmail(String email);

    boolean modifyUserRole(User user, int role);

    boolean inviteUserToAccount(Invitation invitation);

    boolean cancelUserInvitation(Invitation invitation);

    boolean removeUserFromAccount(String user);

    ResponseEntity getHealth();

}
