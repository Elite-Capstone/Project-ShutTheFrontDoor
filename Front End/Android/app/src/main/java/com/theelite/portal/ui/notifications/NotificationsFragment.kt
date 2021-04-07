package com.theelite.portal.ui.notifications

import android.content.Intent
import android.content.SharedPreferences
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.Objects.Command
import com.theelite.portal.Objects.Notification
import com.theelite.portal.R
import com.theelite.portal.request.DeviceService
import com.theelite.portal.request.LockService
import com.theelite.portal.request.NotificationService
import com.theelite.portal.request.RetroFit
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.adapters.RecentNotificationsAdapter
import com.theelite.portal.ui.login.LoginActivity
import com.theelite.portal.ui.stream.StreamActivity
import io.realm.Realm
import io.realm.RealmConfiguration
import io.realm.exceptions.RealmMigrationNeededException
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response


class NotificationsFragment : Fragment(), ClickListener {

    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var recentNotificationRecyclerView: RecyclerView
    private lateinit var root: View
    private lateinit var deleteNotifs: ImageButton
    private lateinit var recentNotificationsAdapter: RecentNotificationsAdapter
    private var notifications: MutableList<Notification> = mutableListOf()
    private lateinit var backgroundThreadRealm: Realm
    private lateinit var deviceService: DeviceService

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
        deleteNotifs = root.findViewById(R.id.deleteButton)
        setUpRecyclerView()
        setUpRefreshLayout()
        deleteNotifs.setOnClickListener() {
            deleteOldNotifs()
            forceReloadAdapter("lock")
        }
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
        notifications = mutableListOf()
        recentNotificationRecyclerView = root.findViewById(R.id.recentNotificationsRecyclerView)
        recentNotificationRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        recentNotificationsAdapter =
            RecentNotificationsAdapter(notifications, this.requireActivity(), this, "lock")
        recentNotificationRecyclerView.adapter = recentNotificationsAdapter
        loadExistingNotifications()
        getNotifications()
    }

    private fun deleteOldNotifs() {
        backgroundThreadRealm.beginTransaction()
        backgroundThreadRealm.deleteAll()
        backgroundThreadRealm.commitTransaction()
        notifications.clear()
    }

    private fun loadExistingNotifications() {
        println("Loading existing notifications")
        notifications.addAll(backgroundThreadRealm.where(Notification::class.java).findAllAsync())
        println("Notifications is ${notifications.size}")
        orderNotifications()
        recentNotificationsAdapter?.notifyDataSetChanged()
        getButtonText()
    }

    private fun getNotifications() {
        val retrofit = RetroFit.get(getString(R.string.url))
        val notifService: NotificationService = retrofit.create(NotificationService::class.java)

        val call = notifService.getRecentNotifications(
            email!!,
            token!!
        )

        call.enqueue(object : Callback<List<Notification>> {
            override fun onResponse(
                call: Call<List<Notification>>,
                response: Response<List<Notification>>
            ) {

                if (response.isSuccessful && response.body() != null && response.body()!!
                        .isNotEmpty()
                ) {
                    notifications.addAll(response.body()!!)
                    backgroundThreadRealm.executeTransactionAsync {
                        it.insert(response.body()!!)
                    }

                    orderNotifications()
                    getButtonText()
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
        Realm.init(this.requireActivity())
        val realmName = "DoorhubNotifications"
        try {
            val config = RealmConfiguration.Builder().name(realmName)
                .deleteRealmIfMigrationNeeded()
                .build()
            backgroundThreadRealm = Realm.getInstance(config)
        } catch (e: RealmMigrationNeededException) {
            println(e.message)
        }
        val retrofit = RetroFit.get(getString(R.string.url))
        deviceService = retrofit.create(DeviceService::class.java)
    }


    private fun orderNotifications() {
        notifications.sortByDescending { it.date }
    }

    private fun getButtonText() {
        if (notifications.isNotEmpty()) {
            forceReloadAdapter("updating!")
            lifecycleScope.launch(Dispatchers.IO) {
                delay(1000 * 5)
                var myText = getText()
                lifecycleScope.launch(Dispatchers.Main) {
                    forceReloadAdapter(myText)
                }
            }
        }
    }

    private suspend fun getText(): String {
        val response = deviceService.getDevices(email!!, token!!).execute()
        if (response.isSuccessful && response.body() != null && response.body()!![0].status?.statusList != null) return if (response.body()!![0].status?.statusList?.doorLocked!!) "unlock" else "lock"
        return "unlock"
    }

    override fun onItemClicked(name: String) {
        when (name) {
            "StreamActivity" -> {
                val intent = Intent(this.context, StreamActivity::class.java)
                this.startActivity(intent)
            }
            "lock" -> {
                sendCommand("Lock door")
            }
            "unlock" -> {
                sendCommand("Unlock door")
            }
            else -> {
            }
        }
    }

    private fun sendCommand(command: String) {
//        val retrofit = RetroFit.get(getString(R.string.url))
//        val lockService: LockService = retrofit.create(LockService::class.java)

        val command =
            Command(
                null,
                "00b288a8-3db1-40b5-b30f-532af4e12f4b",
                command,
                0,
                0
            )

        GlobalScope.launch(Dispatchers.IO) {
            deviceService.sendCommand(command, email!!, token!!).execute()
        }
        getButtonText()
    }

    private fun forceReloadAdapter(text: String) {
        recentNotificationRecyclerView.adapter =
            RecentNotificationsAdapter(notifications, requireContext(), this, text)
    }

    private fun loadState() {
        val sharedPreferences: SharedPreferences = this.requireActivity()
            .getSharedPreferences(
                LoginActivity.SHARED_PREFS,
                AppCompatActivity.MODE_PRIVATE
            )
        email = sharedPreferences.getString(LoginActivity.EMAIL, null)
        token = sharedPreferences.getString(LoginActivity.TOKEN, null)
    }


}
