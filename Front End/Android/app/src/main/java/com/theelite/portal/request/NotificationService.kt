package com.theelite.portal.request

import com.theelite.portal.Objects.Notification
import retrofit2.Call
import retrofit2.http.GET
import retrofit2.http.Path

interface NotificationService {

    @GET("notif/getRecent/{accId}/{email}")
    fun getRecentNotifications(@Path("accId") accId: String, @Path("email") email: String) : Call<List<Notification>>
}