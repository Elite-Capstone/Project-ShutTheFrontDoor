package com.theelite.users.service;

import com.theelite.users.dao.UserDao;
import com.theelite.users.model.Invitation;
import com.theelite.users.model.User;
import com.theelite.users.model.UserRole;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.UUID;

@Service
public class UserServiceImpl implements UserService {
    private ArrayList<Invitation> invitations = new ArrayList<>();
    @Autowired
    private PasswordEncoder passwordEncoder;

    @Autowired
    private UserDao userDao;


    @Override
    public boolean addUser(User user) {
        if (userDao.userExistsWithEmail(user.getEmail())) return false;
        Invitation userInvitation = getInvitationForUser(user.getEmail());
        if (userInvitation != null) {
            user.setAccountId(userInvitation.getAccountId());
            invitations.remove(userInvitation);
        } else user.setAccountId(UUID.randomUUID());
        user.setPassword(passwordEncoder.encode(user.getPassword()));
        userDao.save(user);
        return true;
    }

    @Override
    public boolean authenticateUser(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
        User savedUserInfo = userDao.findById(user.getEmail()).get();
        return passwordEncoder.matches(user.getPassword(), savedUserInfo.getPassword());
    }

    @Override
    public boolean deleteUser(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
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
        if (getInvitationForUser(invitation.getReceiverEmail()) != null) return false;
        //TODO: Send email to targeted user :)
        return invitations.add(invitation);
    }

    @Override
    public boolean cancelUserInvitation(Invitation invitation) {
        return invitations.remove(invitation);
    }

    @Override
    public boolean removeUserFromAccount(String user) {
        if (!userDao.userExistsWithEmail(user)) return false;
        userDao.updateUserAccount(user, null);
        return true;
    }

    private Invitation getInvitationForUser(String email) {
        if (invitations.isEmpty()) return null;
        for (Invitation invitation : invitations) {
            if (invitation.getReceiverEmail().equals(email)) return invitation;
        }
        return null;
    }

    @Override
    public boolean updateUserInfo(User user) {
        if (!userDao.userExistsWithEmail(user.getEmail())) return false;
        userDao.updateUserInfo(user);
        return true;
    }

    @Override
    public ResponseEntity getHealth() {
        try {
            userDao.testDatabaseConnection();
        } catch (Exception e) {
            return new ResponseEntity(HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return new ResponseEntity(HttpStatus.ACCEPTED);
    }
}
