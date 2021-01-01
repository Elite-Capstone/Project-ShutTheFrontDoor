package com.theelite.portal.ui.notifications

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R

class NotificationsFragment : Fragment() {

    //    private lateinit var notificationsViewModel: NotificationsViewModel
    private lateinit var recentNotificationRecyclerView: RecyclerView

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
//        notificationsViewModel = ViewModelProviders.of(this).get(NotificationsViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_notifications, container, false)
//        val textView: TextView = root.findViewById(R.id.text_notifications)
//        notificationsViewModel.text.observe(viewLifecycleOwner, Observer { textView.text = it })
        recentNotificationRecyclerView = root.findViewById(R.id.recentNotificationsRecyclerView)
        recentNotificationRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        val recentNotificationsAdapter = RecentNotificationsAdapter(getNotifications())
        recentNotificationRecyclerView.adapter = recentNotificationsAdapter
        return root
    }


    private fun getNotifications(): ArrayList<String> {
        val notifications = ArrayList<String>()
        var index = 10
        while (index-- >= 0) {
            notifications.add("Received notifications $index")
        }
        return notifications
    }


}