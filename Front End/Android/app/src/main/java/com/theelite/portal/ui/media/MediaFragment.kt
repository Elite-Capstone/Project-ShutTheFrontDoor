package com.theelite.portal.ui.media

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.R
import com.theelite.portal.ui.adapters.VideoDownloadAdapter


class MediaFragment : Fragment(){

    private lateinit var root: View
    private lateinit var videoAdapter:VideoDownloadAdapter
    private lateinit var nameList:MutableList<String>
    private lateinit var videoRecyclerView: RecyclerView

    private lateinit var swipeRefreshLayout: SwipeRefreshLayout

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        root = inflater.inflate(R.layout.fragment_media, container, false)
        videoRecyclerView = root.findViewById(R.id.videoRecyclerview)
        videoRecyclerView.layoutManager =
            LinearLayoutManager(this.context, RecyclerView.VERTICAL, false)
        //populate names list
        nameList = mutableListOf()
        nameList.add("vid1")
        nameList.add("vid2")
        nameList.add("vid3")
        videoAdapter = VideoDownloadAdapter(nameList,this)
        videoRecyclerView.adapter = videoAdapter
        return root

    }

    private fun setUpRefreshLayout() {
        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshRecentNotifications)
        swipeRefreshLayout.setOnRefreshListener {
            //get new names
        }
    }

}