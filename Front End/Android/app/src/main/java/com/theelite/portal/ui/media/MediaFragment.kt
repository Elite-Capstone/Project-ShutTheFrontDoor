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
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.R
import com.theelite.portal.request.MediaService
import com.theelite.portal.request.RetroFit
import com.theelite.portal.ui.MediaClickListener
import com.theelite.portal.ui.adapters.VideoDownloadAdapter
import com.theelite.portal.ui.login.LoginActivity
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import java.lang.Exception


class MediaFragment : Fragment(), MediaClickListener {

    private lateinit var root: View
    private lateinit var videoAdapter: VideoDownloadAdapter
    private lateinit var nameList: MutableList<String>
    private lateinit var videoRecyclerView: RecyclerView
    private lateinit var mediaService: MediaService
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
        initRetrofit()
        loadState()
        nameList = mutableListOf()
        getFileNames()
        videoAdapter = VideoDownloadAdapter(nameList, this)
        videoRecyclerView.adapter = videoAdapter
        return root

    }

    private fun initRetrofit() {
        val retrofit = RetroFit.get(getString(R.string.url))
        mediaService = retrofit.create(MediaService::class.java)
    }

    override fun toDownload(name: String) {
        val i = Intent(Intent.ACTION_VIEW)
        i.data = Uri.parse(name)
        requireActivity().startActivity(i)
    }

    override fun toDelete(name: String, position: Int) {
        nameList.remove(name)
        videoAdapter.notifyItemRemoved(position)
        GlobalScope.launch(Dispatchers.IO) {
            try {
                val response = mediaService.deleteFile(name).execute()
                if (response.isSuccessful) {
                    println("Response to delete is successful\n${response.body()}")
                }
            } catch (e: Exception) {
                println(e.message)
            }
        }
    }

    private fun setUpRefreshLayout() {
        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshMedia)
        swipeRefreshLayout.setOnRefreshListener {
            getFileNames()
        }
    }

    private fun getFileNames() {
        val call = mediaService.getFileNames(
            email!!,
            token!!
        )

        call.enqueue(object : Callback<List<String>> {
            override fun onResponse(
                call: Call<List<String>>,
                response: Response<List<String>>
            ) {
                if (response.isSuccessful && response.body() != null) {
                    println(response.body()?.toString())
                    nameList = (response.body() as MutableList<String>?)!!
                    videoAdapter = VideoDownloadAdapter(nameList, this@MediaFragment)
                    videoRecyclerView.adapter = videoAdapter
                    println("NameList is now ${nameList.size}")
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
