package com.theelite.users.communication;

import org.springframework.web.bind.annotation.DeleteMapping;
import retrofit2.Call;
import retrofit2.http.DELETE;
import retrofit2.http.Path;

public interface DeviceService {

    @DELETE("familyAccountDeleted/{acc}")
    Call<Void> familyAccountDeleted(@Path("acc") String acc);
}
