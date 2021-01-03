package com.theelite.portal.ui.notifications

import android.os.Bundle
import android.os.Handler
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.R

class NotificationsFragment : Fragment() {

    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var recentNotificationRecyclerView: RecyclerView
    private lateinit var root: View

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        root = inflater.inflate(R.layout.fragment_notifications, container, false)
        setUpRecyclerView()
        setUpRefreshLayout()
        return root
    }


    //TODO: Allow user to fetch new notifications
    private fun setUpRefreshLayout() {
        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshRecentNotifications)
        swipeRefreshLayout.setOnRefreshListener {
                println("Recent Notifications -- Starting to Refresh!!")
                val handler = Handler()
                handler.postDelayed(Runnable {
                    if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
                }, 1000)
        }
    }


    private fun setUpRecyclerView() {
        recentNotificationRecyclerView = root.findViewById(R.id.recentNotificationsRecyclerView)
        recentNotificationRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        val recentNotificationsAdapter = RecentNotificationsAdapter(getNotifications())
        recentNotificationRecyclerView.adapter = recentNotificationsAdapter
    }

    private fun getNotifications(): ArrayList<String> {
        val notifications = ArrayList<String>()
        var index = 5
        while (index >= 0) {
            notifications.add("Received notifications ${index--}")
        }
        return notifications
    }


}