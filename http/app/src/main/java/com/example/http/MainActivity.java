package com.example.http;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    private TextView mTextViewResult;
    private RequestQueue mQueue;

    private Handler mHandler = new Handler();
    private Runnable mRunnable;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 創建Runnable，用於定時任務
        mRunnable = new Runnable() {
            @Override
            public void run() {
                jsonParse();
                mHandler.postDelayed(this, 5000); // 5秒後再次執行
            }
        };


        // 開始定時任務
        mHandler.postDelayed(mRunnable, 0);

        mTextViewResult = findViewById(R.id.text_view_result);
        Button buttonOpen = findViewById(R.id.button_open);
        Button buttonClose = findViewById(R.id.button_close);

        mQueue = Volley.newRequestQueue( this);


        buttonOpen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                performGetRequestOpen();
            }
        });

        buttonClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                performGetRequestOpenClose();
            }
        });

    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        // 移除Runnable，避免內存洩漏
        mHandler.removeCallbacks(mRunnable);
    }
    public void onButtonClicked(View view) {
        // 跳轉到 SecondActivity
        Intent intent = new Intent(this, SecondActivity.class);
        startActivity(intent);
    }
    private void jsonParse(){
        TextView dynamicText = findViewById(R.id.dynamicText);
        TextView dynamicText2 = findViewById(R.id.dynamicText2);
        String url = ""; //url
        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            String temp = response.getString("temperature");
                            String hum = response.getString("humidity");
                            String result = "Temperature: " + temp + "°C, Humidity: " + hum + "%";
                            String celsiusSymbol = "\u2103";
                            dynamicText.setText(temp + celsiusSymbol);
                            String percentSymbol = "%";
                            dynamicText2.setText(hum + percentSymbol);
                            mTextViewResult.setText(result);
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                error.printStackTrace();
            }
        });

        mQueue.add(request);
    }
    private void performGetRequestOpen() {
        String url = ""; //
        mTextViewResult.setText("a");
        RequestQueue requestQueue = Volley.newRequestQueue(MainActivity.this);

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,

                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        requestQueue.stop();
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                error.printStackTrace();
                requestQueue.stop();
            }
        });
        requestQueue.add(stringRequest);
    }
    private void performGetRequestOpenClose() {
        String url = ""; //

        RequestQueue requestQueue = Volley.newRequestQueue(MainActivity.this);

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,

                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        requestQueue.stop();
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                error.printStackTrace();
                requestQueue.stop();
            }
        });
        requestQueue.add(stringRequest);
    }
}

