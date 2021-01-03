package com.theelite.portal.ui.settings

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.OptionsAdapter
import com.theelite.portal.ui.OptionsList

import com.theelite.portal.ui.accountSettings.AccountSettings
import com.theelite.portal.ui.login.LoginActivity

class SettingsFragment : Fragment(), ClickListener {

    //    private lateinit var settingsViewModel: SettingsViewModel
    private lateinit var settingsRecyclerView: RecyclerView

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
//        settingsViewModel = ViewModelProviders.of(this).get(SettingsViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_settings, container, false)
        settingsRecyclerView = root.findViewById(R.id.settingsRecyclerView)
        settingsRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)

        val settingsOptions = arrayOf("Account", "Notifications", "Help", "Log Out")
        val settingsOptionsIcons = arrayOf("account", "notification", "help", "log_out")

        val settingsAdapter =
            OptionsAdapter(OptionsList.getListOfSettingsOptions(this.context, settingsOptions, settingsOptionsIcons, this.resources), this)
        settingsRecyclerView.adapter = settingsAdapter
        //        val textView: TextView = root.findViewById(R.id.text_dashboard)
//        settingsViewModel.text.observe(viewLifecycleOwner, Observer {
//            textView.text = it
//        })
        return root
    }



    override fun onItemClicked(name: String) {
        println("Clicked on $name from settings")
        when (name) {
            "Account" -> {
                val accountIntent = Intent(this.context, AccountSettings::class.java)
                this.startActivity(accountIntent)
            }
            "Notifications" -> {
            }
            "Log Out" -> {
                val intent = Intent(this.context, LoginActivity::class.java)
                this.startActivity(intent)
                this.activity?.finish()
            }
        }
    }
//    override fun onCreate(savedInstanceState: Bundle?) {
//        super.onCreate(savedInstanceState)
//        settingsRecyclerView = findViewById(R.id.settingsRecyclerView)
//    }
}