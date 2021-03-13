package com.theelite.portal.ui.notifications

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.Objects.Notification
import com.theelite.portal.R
import com.theelite.portal.request.NotificationService
import com.theelite.portal.request.RetroFit
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.adapters.RecentNotificationsAdapter
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response


class NotificationsFragment : Fragment(), ClickListener {

    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var recentNotificationRecyclerView: RecyclerView
    private lateinit var root: View
    private lateinit var recentNotificationsAdapter: RecentNotificationsAdapter
    private lateinit var notifications: MutableList<Notification>

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
//            println("Recent Notifications -- Starting to Refresh!!")
            getNotifications()
//                recentNotificationsAdapter.notify()
        }
    }


    private fun setUpRecyclerView() {
        notifications = java.util.ArrayList()
        recentNotificationRecyclerView = root.findViewById(R.id.recentNotificationsRecyclerView)
        recentNotificationRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        recentNotificationsAdapter = RecentNotificationsAdapter(notifications,this)
        recentNotificationRecyclerView.adapter = recentNotificationsAdapter
        getNotifications()
    }

    private fun getNotifications() {

        val retrofit = RetroFit.get(getString(R.string.url))
        val notifService: NotificationService = retrofit.create(NotificationService::class.java)

        val call = notifService.getRecentNotifications(
            "00b288a8-3db1-40b5-b30f-532af4e12f4b",
            "test@test.com"
        )

        call.enqueue(object : Callback<List<Notification>> {
            override fun onResponse(
                call: Call<List<Notification>>,
                response: Response<List<Notification>>
            ) {
                if (response.isSuccessful) {
                    notifications.addAll(response.body()!!)
                    orderNotifications()
                    recentNotificationsAdapter.notifyDataSetChanged()
                }
                if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
            }

            override fun onFailure(call: Call<List<Notification>>, t: Throwable) {
                Toast.makeText(activity, "${t.message}", Toast.LENGTH_SHORT).show()
            }

        })

    }



    private fun orderNotifications() {
        notifications.sortByDescending { it.date }
    }

    override fun onItemClicked(name: String) {
        val i = Intent(Intent.ACTION_VIEW)
        i.data = Uri.parse(name)
        activity!!.startActivity(i)
    }

}