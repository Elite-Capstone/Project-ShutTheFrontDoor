package com.theelite.portal.ui.home

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.login.LoginActivity
import com.theelite.portal.ui.settings.AccountSettingsActivity
import com.theelite.portal.ui.settings.DeviceStatusActivity
import com.theelite.portal.ui.settings.NotificationsActivity
import com.theelite.portal.ui.stream.StreamActivity

class HomeFragment : Fragment() {

    private val homeViewModel by activityViewModels<HomeViewModel>()
    private lateinit var textViewHome: TextView
    private  lateinit var streamButton: Button

    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_home, container, false)
        textViewHome = root.findViewById(R.id.text_home)
        streamButton = root.findViewById(R.id.streamVideo)
        homeViewModel.text.observe(viewLifecycleOwner, Observer {
            textViewHome.text = it
        })
        streamButton.setOnClickListener(){
            onItemClicked()
        }
        return root

    }

    fun onItemClicked() {
        val intent = Intent(this.context, StreamActivity::class.java)
        this.startActivity(intent)
    }

}
