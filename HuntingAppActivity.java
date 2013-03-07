package com.morningside.edu;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class HuntingAppActivity extends Activity implements View.OnClickListener{
	
	protected Button location_button, mapButton, hunt_button, weather_button;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.huntingapp);
        loadScreen();  
    } 
    
    protected void loadScreen() {
		// TODO
    	location_button = (Button) findViewById(R.id.goToLocation);
    	mapButton = (Button) findViewById(R.id.mapButton);
    	hunt_button = (Button) findViewById(R.id.startHunt);
    	weather_button = (Button) findViewById(R.id.weather);
		location_button.setOnClickListener(this);
		mapButton.setOnClickListener(this);
		hunt_button.setOnClickListener(this);
		weather_button.setOnClickListener(this);
    }

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.goToLocation:
		Intent i = new Intent("android.intent.action.LOCATION");
        startActivity(i);
        
        break;
		case R.id.mapButton:
			Intent j = new Intent("android.intent.action.MAPGO");
	        startActivity(j);
	        
	        break;
		case R.id.startHunt:
			Intent k = new Intent("android.intent.action.STARTHUNT");
	        startActivity(k);
	        
	        break;
		case R.id.weather:
			Intent l = new Intent("android.intent.action.WEATHER");
	        startActivity(l);
	        
	        break;
        }
		
	}
}
