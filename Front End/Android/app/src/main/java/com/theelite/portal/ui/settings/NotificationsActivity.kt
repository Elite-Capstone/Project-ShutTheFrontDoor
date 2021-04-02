package com.theelite.portal.ui.settings

import android.os.Bundle
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener

class NotificationsActivity : AppCompatActivity(), ClickListener {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_notif)
        setupPageView()
    }

    private fun setupPageView() {

    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        println("Pressed on back arrow from Notifications Settings")
        super.onBackPressed()
        return true
    }

    override fun onItemClicked(name: String) {
        TODO("Not yet implemented")
    }
}