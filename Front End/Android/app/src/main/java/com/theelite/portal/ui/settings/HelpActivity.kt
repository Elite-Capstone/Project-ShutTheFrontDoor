package com.theelite.portal.ui.settings


import android.os.Bundle
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener

class HelpActivity : AppCompatActivity(), ClickListener {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.help_activity)
        setupPageView()
    }

    private fun setupPageView() {

    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        println("Pressed on back arrow from Help Page")
        super.onBackPressed()
        return true
    }

    override fun onItemClicked(name: String) {
        TODO("Not yet implemented")
    }
}