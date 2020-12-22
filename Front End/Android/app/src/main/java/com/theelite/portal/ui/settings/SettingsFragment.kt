package com.theelite.portal.ui.settings

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R

class SettingsFragment : Fragment() {

    private lateinit var settingsViewModel: SettingsViewModel
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
        val options = ArrayList<String>()
        options.add("Account")
        options.add("Notifications")
        options.add("Help")
        options.add("Log out")
        val settingsAdapter = SettingsAdapter(options)
        settingsRecyclerView.adapter = settingsAdapter
        //        val textView: TextView = root.findViewById(R.id.text_dashboard)
//        settingsViewModel.text.observe(viewLifecycleOwner, Observer {
//            textView.text = it
//        })
        return root
    }

//    override fun onCreate(savedInstanceState: Bundle?) {
//        super.onCreate(savedInstanceState)
//        settingsRecyclerView = findViewById(R.id.settingsRecyclerView)
//    }
}