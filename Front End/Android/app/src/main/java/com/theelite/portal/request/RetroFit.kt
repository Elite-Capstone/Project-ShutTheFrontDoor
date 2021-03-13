package com.theelite.portal.request

import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

object RetroFit {

    fun get(url: String): Retrofit {
        return Retrofit.Builder()
            .baseUrl(url)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
    }

}