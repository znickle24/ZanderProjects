package com.example.znickle.webchatapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import static android.provider.AlarmClock.EXTRA_MESSAGE;

public class LoginActivity extends AppCompatActivity {
    //USER AND ROOM
    public static final String extra_username = "user";
    public static final String extra_room = "room";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
    }
    public void sendMessage(View view) {
        Intent intent = new Intent (this, MessageRoom.class);
        //get username from input box and store it in a string username
        EditText enterUsername = (EditText)findViewById(R.id.editUsername);
        String username = enterUsername.getText().toString();
        //get roomname from input box and store it in a string roomName
        EditText enterRoomName = (EditText) findViewById(R.id.editRoomName);
        String roomName = enterRoomName.getText().toString();
        //sending the username and the roomname
        intent.putExtra(extra_username, username);
        intent.putExtra(extra_room, roomName);
        //once all of that is complete, need to start the process
        startActivity(intent);

    }


}
