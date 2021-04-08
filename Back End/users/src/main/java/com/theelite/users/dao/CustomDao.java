package com.theelite.users.dao;

import com.theelite.users.model.*;

import java.util.UUID;

public interface CustomDao {
    boolean userExistsWithEmail(String email);
    void updateUserAccount(String user, UUID accountId);
    void deleteUserAccount(String email);
    void updateUserInfo(User user);
    boolean validateUser(String email, String token);
    void addNewTokenToUser(String email, UserToken token);
    void removeToken();
    void updateUserPassword(String user, String password);
    void updateUserRole(String user, UserRole userRole);
    void testDatabaseConnection();
    void saveNewFamilyAccount(Account account);
    void deleteFamilyAccount(UUID account);
    long numberOfAdminsInFamilyAccount(UUID accountId);
    void saveNewInvitation(Invitation invitation);
    void cancelInvitation(String invitation);
    Invitation getInvitation(String user);
    boolean invitationExists(String user);
}
