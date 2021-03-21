package com.theelite.portal.request

import com.google.gson.GsonBuilder
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory


object RetroFit {

    fun get(url: String): Retrofit {
        val gson = GsonBuilder()
                .setLenient()
                .create()
        return Retrofit.Builder().baseUrl(url).addConverterFactory(GsonConverterFactory.create(gson)).build()
    }

}