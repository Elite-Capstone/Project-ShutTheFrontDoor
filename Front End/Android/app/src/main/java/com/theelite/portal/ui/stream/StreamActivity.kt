package com.theelite.portal.ui.stream

import android.os.Bundle
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.appcompat.app.AppCompatActivity
import com.theelite.portal.R


class StreamActivity : AppCompatActivity() {

    private lateinit var webView: WebView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_stream)
        webView = findViewById(R.id.webView)
        webView.webViewClient = WebViewClient()
//        webView.loadUrl("http://7edf7a2bf490.ngrok.io/stream");
        webView.loadUrl("http://01164f71afbd.ngrok.io/stream")

    }

    override fun onBackPressed() {
        super.onBackPressed()
        this.webView.stopLoading()
        this.finish()
    }
}