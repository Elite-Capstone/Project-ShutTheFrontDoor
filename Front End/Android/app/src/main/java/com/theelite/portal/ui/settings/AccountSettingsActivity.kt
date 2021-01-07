package com.theelite.portal.ui.settings

import android.os.Bundle
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.OptionsAdapter
import com.theelite.portal.ui.OptionsList

class AccountSettingsActivity : AppCompatActivity(), ClickListener {

    private lateinit var recyclerView: RecyclerView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_account)
        setUpView()
    }

    private fun setUpView() {
        recyclerView = findViewById(R.id.accountSettingsRecyclerView)
        recyclerView.layoutManager = LinearLayoutManager(this)

        val accountSettingsOptions = arrayOf("Profile", "Users")
        val accountSettingsOptionsIcon = arrayOf("account", "users")

        val optionsAdapter = OptionsAdapter(
            OptionsList.getListOfSettingsOptions(
                this,
                accountSettingsOptions,
                accountSettingsOptionsIcon,
                this.resources
            ), this
        )

        recyclerView.adapter = optionsAdapter
    }


    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        println("Pressed on back arrow from account Settings")
        super.onBackPressed()
        return true
    }

    override fun onItemClicked(name: String) {
//        TODO Not yet implemented
        when (name) {
            "Profile" -> println("Clicked on $name")
            "Users" -> println("Clicked on $name")
        }

    }
}