package com.theelite.portal.ui.home

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class HomeViewModel : ViewModel() {

    private val _text = MutableLiveData<String>().apply {
        // TODO: Can contain information about app, such as # of new notifications or
        //  # of recent actions in logs or if actions were performed
        value = "Welcome to Doorhub!"
    }
    val text: LiveData<String> = _text
}