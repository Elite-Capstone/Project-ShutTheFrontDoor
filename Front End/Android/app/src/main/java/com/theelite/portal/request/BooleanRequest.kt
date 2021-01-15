package com.theelite.portal.request

import com.android.volley.*
import com.android.volley.toolbox.HttpHeaderParser.parseCacheHeaders
import java.io.UnsupportedEncodingException


internal class BooleanRequest(
    method: Int,
    url: String?,
    requestBody: String?,
    listener: Response.Listener<Boolean>,
    errorListener: Response.ErrorListener
) :
    Request<Boolean?>(method, url, errorListener) {
    private val mListener: Response.Listener<Boolean> = listener
    private val mErrorListener: Response.ErrorListener = errorListener
    private val mRequestBody: String? = requestBody
    private val PROTOCOL_CHARSET = "utf-8"

    //    val bodyContentType = String.format("application/json; charset=%s", PROTOCOL_CHARSET)
    private val PROTOCOL_CONTENT_TYPE =
        String.format("application/json; charset=%s", PROTOCOL_CHARSET)

    override fun parseNetworkResponse(response: NetworkResponse): Response<Boolean?>? {
        val parsed: Boolean?
        parsed = try {
            java.lang.Boolean.valueOf(String(response.data))
        } catch (e: UnsupportedEncodingException) {
            java.lang.Boolean.valueOf(String(response.data))
        }
        return Response.success(parsed, parseCacheHeaders(response))
    }

    override fun parseNetworkError(volleyError: VolleyError?): VolleyError {
        return super.parseNetworkError(volleyError)
    }

    override fun deliverResponse(response: Boolean?) {
        mListener.onResponse(response)
    }

    override fun deliverError(error: VolleyError?) {
        mErrorListener.onErrorResponse(error)
    }

    override fun getBodyContentType(): String {
        return PROTOCOL_CONTENT_TYPE
    }


    @Throws(AuthFailureError::class)
    override fun getBody(): ByteArray? {
        return try {
            mRequestBody?.toByteArray(charset(PROTOCOL_CHARSET))
        } catch (uee: UnsupportedEncodingException) {
            VolleyLog.wtf(
                "Unsupported Encoding while trying to get the bytes of %s using %s",
                mRequestBody, PROTOCOL_CHARSET
            )
            null
        }
    }

}