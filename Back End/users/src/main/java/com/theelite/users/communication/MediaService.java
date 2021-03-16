package com.theelite.users.communication;

import retrofit2.Call;
import retrofit2.http.DELETE;
import retrofit2.http.Path;

public interface MediaService {
    @DELETE("familyAccountDeleted/{acc}")
    Call<Void> deleteAllForFamilyAccount(@Path("acc") String acc);
}
