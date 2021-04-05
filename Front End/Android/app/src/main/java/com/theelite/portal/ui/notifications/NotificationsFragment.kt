package com.theelite.portal.ui.notifications

import android.content.Intent
import android.content.SharedPreferences
import android.net.Uri
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
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
import com.theelite.portal.ui.login.LoginActivity
import com.theelite.portal.ui.stream.StreamActivity
import io.realm.Realm
import io.realm.RealmConfiguration
import io.realm.RealmResults
import io.realm.exceptions.RealmMigrationNeededException
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.launch
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response


class NotificationsFragment : Fragment(), ClickListener {

    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var recentNotificationRecyclerView: RecyclerView
    private lateinit var root: View
    private lateinit var recentNotificationsAdapter: RecentNotificationsAdapter
    private var notifications: MutableList<Notification> = mutableListOf()
    private lateinit var backgroundThreadRealm: Realm

    private var email: String? = null
    private var token: String? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        loadState()
        setUpRealm()
        root = inflater.inflate(R.layout.fragment_notifications, container, false)
        setUpRecyclerView()
        setUpRefreshLayout()
        return root
    }

    //TODO: Allow user to fetch new notifications
    private fun setUpRefreshLayout() {
        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshRecentNotifications)
        swipeRefreshLayout.setOnRefreshListener {
            getNotifications()
        }
    }


    private fun setUpRecyclerView() {
        notifications = java.util.ArrayList()
        recentNotificationRecyclerView = root.findViewById(R.id.recentNotificationsRecyclerView)
        recentNotificationRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        recentNotificationsAdapter = RecentNotificationsAdapter(notifications, this)
        recentNotificationRecyclerView.adapter = recentNotificationsAdapter
        loadExistingNotifications()
        getNotifications()
    }

    private fun loadExistingNotifications() {
        println("Loading existing notifications")
        notifications.addAll(backgroundThreadRealm.where(Notification::class.java).findAllAsync())
        println("Notifications is ${notifications.size}")
        orderNotifications()
        if (recentNotificationsAdapter != null) recentNotificationsAdapter.notifyDataSetChanged()
    }

    private fun getNotifications() {
        val retrofit = RetroFit.get(getString(R.string.url))
        val notifService: NotificationService = retrofit.create(NotificationService::class.java)

        println("$email and $token")
        val call = notifService.getRecentNotifications(
            "$email",
            "$token"
        )

        call.enqueue(object : Callback<List<Notification>> {
            override fun onResponse(
                call: Call<List<Notification>>,
                response: Response<List<Notification>>
            ) {
                if (response.isSuccessful) {
                    notifications.addAll(response.body()!!)
//                    GlobalScope.launch(context = Dispatchers.IO) {
                    backgroundThreadRealm.executeTransactionAsync { transactionRealm ->
                        transactionRealm.insert(response.body()!!)
//                        }
                    }
                    orderNotifications()
                    recentNotificationsAdapter = RecentNotificationsAdapter(notifications, this@NotificationsFragment)
                }
                if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
            }

            override fun onFailure(call: Call<List<Notification>>, t: Throwable) {
                Toast.makeText(activity, "${t.message}", Toast.LENGTH_SHORT).show()
                if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
            }

        })

    }

    private fun setUpRealm() {
        Realm.init(this.activity)
        val realmName: String = "DoorhubNotifications"
        try {
            val config = RealmConfiguration.Builder().name(realmName)
                .deleteRealmIfMigrationNeeded()
                .build()
            backgroundThreadRealm = Realm.getInstance(config)
        } catch (e: RealmMigrationNeededException) {
            println(e.message)
        }
    }


    private fun orderNotifications() {
        notifications.sortByDescending { it.date }
    }

    override fun onItemClicked(name: String) {

        if (name.equals("StreamActivity")){
            val intent = Intent(this.context, StreamActivity::class.java)
            this.startActivity(intent)
        }
        else {
            val i = Intent(Intent.ACTION_VIEW)
            i.data = Uri.parse(name)
            requireActivity().startActivity(i)
        }
    }

    private fun loadState() {
        val sharedPreferences: SharedPreferences = this.requireActivity()
            .getSharedPreferences(LoginActivity.SHARED_PREFS, AppCompatActivity.MODE_PRIVATE)
        email = sharedPreferences.getString(LoginActivity.EMAIL, null)
        token = sharedPreferences.getString(LoginActivity.TOKEN, null)
    }


}