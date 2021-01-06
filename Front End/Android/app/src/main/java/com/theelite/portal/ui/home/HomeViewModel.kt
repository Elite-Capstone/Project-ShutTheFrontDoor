package com.theelite.portal.ui.home

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.theelite.portal.R

class HomeViewModel : ViewModel() {

    private val _greeting = MutableLiveData<String>().apply {
        // TODO: Can contain information about app, such as # of new notifications or
        //  # of recent actions in logs or if actions were performed
        value = "Welcome to Doorhub!"
    }

    fun setGreeting(newGreeting: String) {
        _greeting.value = newGreeting
    }
    val text: LiveData<String> = _greeting
}