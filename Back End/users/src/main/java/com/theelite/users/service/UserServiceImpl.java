package com.theelite.users.service;

import com.theelite.users.communication.DeviceService;
import com.theelite.users.communication.MediaService;
import com.theelite.users.communication.NotifService;
import com.theelite.users.dao.UserDao;
import com.theelite.users.model.*;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import retrofit2.Retrofit;

import java.io.IOException;
import java.security.SecureRandom;
import java.util.*;

@Service
public class UserServiceImpl implements UserService {
    private final SecureRandom secureRandom;
    private final Base64.Encoder base64Encoder;
    private final PasswordEncoder passwordEncoder;
    private final UserDao userDao;

    @Value("${device.url}")
    private String deviceUrl;
    @Value("${notif.url}")
    private String notifUrl;
    @Value("${media.url}")
    private String mediaUrl;
    private DeviceService deviceService;
    private NotifService notifService;
    private MediaService mediaService;

    public UserServiceImpl(SecureRandom secureRandom, Base64.Encoder base64Encoder, PasswordEncoder passwordEncoder, UserDao userDao) {
        this.secureRandom = secureRandom;
        this.base64Encoder = base64Encoder;
        this.passwordEncoder = passwordEncoder;
        this.userDao = userDao;
        this.deviceService = this.buildRetrofitObject(deviceUrl, DeviceService.class);
        this.notifService = this.buildRetrofitObject(notifUrl, NotifService.class);
        this.mediaService = this.buildRetrofitObject(mediaUrl, MediaService.class);
    }

    private void userCreatedNewFamilyAccount(String accountId) {
        try {
            notifService.createNewConsumerGroup(accountId).execute();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        } catch (NullPointerException e) {
            System.out.println("notifService is null; " + e.getMessage());
        }
    }

    // User Signs up
    @Override
    public String addUser(User user) {
        if (userDao.userExistsWithEmail(user.getEmail())) return ApiResponses.unsuccessful;

        Invitation userInvitation = userDao.getInvitation(user.getEmail());
        String token = generateNewToken();
        UserToken userToken = new UserToken(token, new Date());
        if (userInvitation != null) {
            user.setAccountId(userInvitation.getAccountId());
            userDao.cancelInvitation(user.getEmail());
            //Regular user in family account
            user.setRole(UserRole.Regular);
        } else {
            // Only user in his family account
            user.setAccountId(UUID.randomUUID());
            user.setRole(UserRole.Admin);
            // to kafka to create new consumer group
            userCreatedNewFamilyAccount(user.getAccountId().toString());
        }

        user.setTokens(Collections.singletonList(userToken));
        user.setPassword(passwordEncoder.encode(user.getPassword()));
        userDao.save(user);

        return token;
    }

    // User Logs in
    @Override
    public String authenticateUser(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return ApiResponses.unsuccessful;
        User savedUserInfo = userDao.findById(user.getEmail()).get();
        if (passwordEncoder.matches(user.getPassword(), savedUserInfo.getPassword())) {
            UserToken userToken = new UserToken(generateNewToken(), new Date());
            userDao.addNewTokenToUser(user.getEmail(), userToken);
            return userToken.getToken();
        } else return ApiResponses.unsuccessful;
    }

    @Override
    public boolean deleteUser(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
        User userInfo = userDao.findById(user.getEmail()).get();
        String famAcc = userInfo.getAccountId().toString();

        if (userInfo.getRole().equals(UserRole.Admin) && userDao.numberOfAdminsInFamilyAccount(UUID.fromString(famAcc)) == 1) {
            try {
                deviceService.familyAccountDeleted(famAcc).execute();
                notifService.deleteConsumerGroup(famAcc).execute();
                mediaService.deleteAllForFamilyAccount(famAcc).execute();
            } catch (IOException | NullPointerException e) {
                System.out.println(e.getMessage());
            }
        }
        userDao.deleteById(user.getEmail());
        return true;
    }

    @Override
    public User getUserWithEmail(String email) {
        if (userDao.userExistsWithEmail(email)) return userDao.findById(email).get();
        else return null;
    }

    @Override
    public boolean modifyUserRole(User user, int role) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
        switch (role) {
            case 0:
                userDao.updateUserRole(user.getEmail(), UserRole.Admin);
                break;
            case 2:
                userDao.updateUserRole(user.getEmail(), UserRole.Guest);
                break;
            default:
                userDao.updateUserRole(user.getEmail(), UserRole.Regular);
                break;
        }
        return true;
    }

    @Override
    public boolean inviteUserToAccount(Invitation invitation) {
        if (userDao.userExistsWithEmail(invitation.getReceiverEmail())) {
            userDao.updateUserAccount(invitation.getReceiverEmail(), invitation.getAccountId());
            return true;
        }
        if (userDao.getInvitation(invitation.getReceiverEmail()) != null) return false;
        //TODO: Send email to targeted user :)
        userDao.saveNewInvitation(invitation);
        return true;
    }

    @Override
    public boolean cancelUserInvitation(Invitation invitation) {
        userDao.cancelInvitation(invitation.getReceiverEmail());
        return !userDao.invitationExists(invitation.getReceiverEmail());
    }

    @Override
    public boolean removeUserFromAccount(String user) {
        if (!userDao.userExistsWithEmail(user)) return false;
        userDao.updateUserAccount(user, null);
        return true;
    }

    @Override
    public boolean updateUserInfo(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
        userDao.updateUserInfo(user);
        return true;
    }

    @Override
    public boolean validateUser(String email, String token) {
        return userDao.validateUser(email, token);
    }

    @Override
    public ResponseEntity<String> getHealth() {
        try {
            userDao.testDatabaseConnection();
            if (notifService == null)
                return new ResponseEntity<>("notifService is null", HttpStatus.INTERNAL_SERVER_ERROR);
            else if (deviceService == null)
                return new ResponseEntity<>("deviceService is null", HttpStatus.INTERNAL_SERVER_ERROR);
            else if (mediaService == null)
                return new ResponseEntity<>("mediaService is null", HttpStatus.INTERNAL_SERVER_ERROR);
        } catch (Exception e) {
            return new ResponseEntity<>("Error with the db somehow", HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return new ResponseEntity<>("Everything seems to be fine!", HttpStatus.OK);
    }

    @Override
    public String getFamilyAccountForUser(String email) {
        return getUserWithEmail(email).getAccountId().toString();
    }

    private String generateNewToken() {
        byte[] randomBytes = new byte[24];
        secureRandom.nextBytes(randomBytes);
        return base64Encoder.encodeToString(randomBytes);

    }

    public String testToken() {
        return generateNewToken();
    }

    private <T> T buildRetrofitObject(String url, Class<T> retroClass) {
        if (url == null || url.isBlank() || url.isEmpty()) {
            System.out.println("Could not create Retrofit object as url for " + retroClass.getName() + " is null.");
            return null;
        }
        Retrofit retrofit = new Retrofit.Builder().baseUrl(url).build();
        return retrofit.create(retroClass);
    }
}
