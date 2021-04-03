package com.theelite.portal.ui.media

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
import com.theelite.portal.request.MediaService
import com.theelite.portal.request.NotificationService
import com.theelite.portal.request.RetroFit
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.adapters.VideoDownloadAdapter
import com.theelite.portal.ui.login.LoginActivity
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response


class MediaFragment : Fragment(), ClickListener {

    private lateinit var root: View
    private lateinit var videoAdapter: VideoDownloadAdapter
    private lateinit var nameList: MutableList<String>
    private lateinit var videoRecyclerView: RecyclerView

    private lateinit var newNames: List<String>

    private lateinit var swipeRefreshLayout: SwipeRefreshLayout

    private var email: String? = null
    private var token: String? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        root = inflater.inflate(R.layout.fragment_media, container, false)
        videoRecyclerView = root.findViewById(R.id.videoRecyclerview)
        videoRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        setUpRefreshLayout()
        loadState()
        nameList = mutableListOf()
        getFileNames()
        videoAdapter = VideoDownloadAdapter(nameList, this)
        videoRecyclerView.adapter = videoAdapter
        return root

    }

    override fun onItemClicked(name: String) {
        val i = Intent(Intent.ACTION_VIEW)
        i.data = Uri.parse(name)
        requireActivity().startActivity(i)
    }

    private fun setUpRefreshLayout() {
        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshMedia)
        swipeRefreshLayout.setOnRefreshListener {
            getFileNames()
        }
    }

    private fun getFileNames() {
        val retrofit = RetroFit.get(getString(R.string.url))
        val mediaService: MediaService = retrofit.create(MediaService::class.java)


        val call = mediaService.getFileNames(
            "$email",
            "$token"
        )

        call.enqueue(object : Callback<List<String>> {
            override fun onResponse(
                call: Call<List<String>>,
                response: Response<List<String>>
            ) {
                if (response.isSuccessful && response.body() != null) {
                    println(response.body()?.toString())
                    newNames = findRedundant((response.body() as MutableList<String>?)!!)
                    nameList.addAll(newNames)
                    videoAdapter.notifyDataSetChanged()
                }
                if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
            }

            override fun onFailure(call: Call<List<String>>, t: Throwable) {
                println("On failure: " + t.message)
                Toast.makeText(activity, "${t.message}", Toast.LENGTH_SHORT).show()
                if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
            }

        })

    }

    private fun loadState() {
        val sharedPreferences: SharedPreferences = this.requireActivity()
            .getSharedPreferences(LoginActivity.SHARED_PREFS, AppCompatActivity.MODE_PRIVATE)
        email = sharedPreferences.getString(LoginActivity.EMAIL, null)
        token = sharedPreferences.getString(LoginActivity.TOKEN, null)
    }

    private fun findRedundant(allString: MutableList<String>): List<String> {
        allString.removeAll(nameList)
        return allString
    }
}