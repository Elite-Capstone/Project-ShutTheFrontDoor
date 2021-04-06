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
import com.theelite.portal.Objects.Command
import com.theelite.portal.Objects.Notification
import com.theelite.portal.Objects.TimeOfPublish
import com.theelite.portal.R
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
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import java.util.*


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
        notifications = mutableListOf()
        recentNotificationRecyclerView = root.findViewById(R.id.recentNotificationsRecyclerView)
        recentNotificationRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        recentNotificationsAdapter =
            RecentNotificationsAdapter(notifications, this.requireActivity(), this)
        recentNotificationRecyclerView.adapter = recentNotificationsAdapter
        loadExistingNotifications()
        getNotifications()
    }

    private fun loadExistingNotifications() {
        println("Loading existing notifications")
        notifications.addAll(backgroundThreadRealm.where(Notification::class.java).findAllAsync())
        println("Notifications is ${notifications.size}")
        orderNotifications()
        recentNotificationsAdapter?.notifyDataSetChanged()
    }

    private fun getNotifications() {
        val retrofit = RetroFit.get(getString(R.string.url))
        val notifService: NotificationService = retrofit.create(NotificationService::class.java)

        println("$email and $token")
        val call = notifService.getRecentNotifications(
            email!!,
            token!!
        )
//        GlobalScope.launch(context = Dispatchers.IO) {
//            val result = notifService.getRecentNotifications(email!!, token!!).execute()
//            if (result.isSuccessful && result.body()!! != null && result.body()!!.isNotEmpty()) {
//                notifications.addAll(result.body()!!)
//
//                GlobalScope.launch(context = Dispatchers.Main) {
//                    backgroundThreadRealm.executeTransactionAsync { transactionRealm ->
//                        transactionRealm.insert(result.body()!!)
//                    }
//                    orderNotifications()
////                    recentNotificationsAdapter = RecentNotificationsAdapter(notifications, this., this@NotificationsFragment)
//                    forceReloadAdapter()
//                }
//            }
//        }


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
                    forceReloadAdapter()
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
        when (name) {
            "StreamActivity" -> {
                val intent = Intent(this.context, StreamActivity::class.java)
                this.startActivity(intent)
            }
            "lock" -> {
                changeState("lock")
            }
            "unlock" -> {
                changeState("unlock")
            }
            else -> {
                val i = Intent(Intent.ACTION_VIEW)
                i.data = Uri.parse(name)
                requireActivity().startActivity(i)
            }
        }
    }

    private fun changeState(state: String) {
        val retrofit = RetroFit.get(getString(R.string.url))
        val lockService: LockService = retrofit.create(LockService::class.java)

        var commandRequest: String = ""

        val c = Calendar.getInstance()

        val year = c.get(Calendar.YEAR)
        val month = c.get(Calendar.MONTH)
        val day = c.get(Calendar.DAY_OF_MONTH)

        val hour = c.get(Calendar.HOUR_OF_DAY)
        val minute = c.get(Calendar.MINUTE)
        val second = c.get(Calendar.SECOND)

        when (state) {
            "lock" -> {
                commandRequest = "Lock door"
            }
            "unlock" -> {
                commandRequest = "Unlock door"
            }
        }

        var timeOfPublish = TimeOfPublish(year, month, day, hour, minute, second)
        var command: Command =
            Command(
                timeOfPublish,
                "00b288a8-3db1-40b5-b30f-532af4e12f4b",
                commandRequest,
                0,
                0
            )
        println("$email and $token")
        val call = lockService.sendCommand(
            command,
            email!!,
            token!!
        )

        call.enqueue(object : Callback<String> {
            override fun onResponse(call: Call<String>, response: Response<String>) {
                if (response.isSuccessful) {
                }
            }

            override fun onFailure(call: Call<String>, t: Throwable) {
                Toast.makeText(activity, "${t.message}", Toast.LENGTH_SHORT).show()
            }
        })
    }

    private fun forceReloadAdapter() {
        recentNotificationRecyclerView.adapter =
            RecentNotificationsAdapter(notifications, this.requireContext(), this)
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