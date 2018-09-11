package com.example.znickle.webchatapp;

import android.app.ActionBar;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

import com.koushikdutta.async.future.Future;
import com.koushikdutta.async.http.AsyncHttpClient;
import com.koushikdutta.async.http.WebSocket;

import org.json.JSONException;
import org.json.JSONObject;


public class MessageRoom extends AppCompatActivity {
    public static final String EXTRA_MESSAGE = "com.example.znickle.MessageRoom.MESSAGE";
    public WebSocket webSocket;
    public String username = "user";
    public ScrollView scrollView;

    public LinearLayout scroll;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_message_room);

        //import, from the intent, the string for the room and username
        Intent intent = getIntent();
        username = intent.getStringExtra(LoginActivity.extra_username);
        final String roomName = intent.getStringExtra(LoginActivity.extra_room);

        //for the text in the particular boxes, find and set the values
        TextView room_name2 = findViewById(R.id.room_name2);
        room_name2.setText(roomName);

        final String usernameFinal = username;
        final String roomNameFinal = roomName;

        scroll = findViewById(R.id.message_board);
        scrollView =findViewById(R.id.scrollMessageBoard);

        final Future<WebSocket> websocket = AsyncHttpClient.getDefaultInstance().websocket("http://10.0.2.2:8080/", "my-protocol", new AsyncHttpClient.WebSocketConnectCallback() {
            @Override
            public void onCompleted(Exception ex, WebSocket webSocket) {
                if (ex != null) {
                    ex.printStackTrace();
                    return;
                }
                MessageRoom.this.webSocket = webSocket;
                webSocket.send(usernameFinal + " " +  roomNameFinal);
                webSocket.setStringCallback(new WebSocket.StringCallback() {
                    public void onStringAvailable(String s) {
                        System.out.println("I got a string: " + s);
                        postMessage(s);
                    }
                });
            }

            });

        }
    public void postMessage(String s) {
        final String string = s;
        JSONObject mJSON = null;
        try {
            mJSON = new JSONObject(string);
        } catch (JSONException e) {
            e.printStackTrace();
        }  String messageUser = "";
        try {
            messageUser = mJSON.getString("user");
        } catch (JSONException e) {
            e.printStackTrace();
        } String messageMessage = "";
        try {
            messageMessage = mJSON.getString("message");
        } catch (JSONException e) {
            e.printStackTrace();
        } final String finalString = (messageUser + ": " + messageMessage);


        final Handler handler = new Handler(MessageRoom.this.getMainLooper());
//        final String string = s;
        handler.post(new Runnable() {
            @Override
            public void run() {

                TextView messagePost = new TextView(MessageRoom.this);
                messagePost.setText(finalString);
                scroll.addView(messagePost);
                scrollView.fullScroll(View.FOCUS_DOWN);

                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        TextView text = new TextView(MessageRoom.this);
                        final String blankLine = "";
                        text.setText(blankLine);
                        text.setLayoutParams((new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, ActionBar.LayoutParams.WRAP_CONTENT)));
                        scrollView.fullScroll(View.FOCUS_DOWN);

                    }
                });
            }
        });
    }
    public void sendMessageButton(View view) {
        EditText messageFromUser = findViewById(R.id.userMessage);
        String message = messageFromUser.getText().toString();
        String combinedMessage = username + " " + message;
        //need to figure my shit out manana
        webSocket.send(combinedMessage);
    }
}