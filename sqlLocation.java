package com.morningside.edu;

import com.google.android.maps.GeoPoint;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Typeface;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class sqlLocation extends Activity implements OnClickListener {

	/**
	 * @param args
	 */
	Button cords, submit, sqlView, delete;
	EditText name, location;
	AddSpot entry = new AddSpot(sqlLocation.this);
	String Name = "";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.add_location_sql);
		submit = (Button) findViewById(R.id.submit);
		delete = (Button) findViewById(R.id.sqlDelete);
		name = (EditText) findViewById(R.id.name);
		location = (EditText) findViewById(R.id.location);
		cords = (Button) findViewById(R.id.cords);
		sqlView = (Button) findViewById(R.id.locations);
		sqlView.setOnClickListener(this);
		submit.setOnClickListener(this);
		cords.setOnClickListener(this);
		delete.setOnClickListener(this);
		
		
		 
	}
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		
		switch (arg0.getId()) {
		case R.id.submit:
			
			boolean didItWork = true;
			try{
				Name = name.getText().toString();
				String Location = location.getText().toString();
				
				String dl = "0";
				entry.open();
				entry.createEntry(Name, Location);
				entry.close();
				
			}catch (Exception e) {
				didItWork = false;
			}finally {
				if(didItWork){
					Dialog d = new Dialog(this);
					d.setTitle("Success!");
					TextView tv = new TextView(this);
					tv.setTextSize(16);
	                tv.setTypeface(Typeface
	                                .create("Tahoma", Typeface.BOLD));
					tv.setText("");
					d.setContentView(tv);
					d.show();
				}
				else{
					Dialog d = new Dialog(this);
					d.setTitle("Error");
					TextView tv = new TextView(this);
					tv.setTextSize(16);
	                tv.setTypeface(Typeface
	                                .create("Tahoma", Typeface.BOLD));
					tv.setText("");
					d.setContentView(tv);
					d.show();
				}
			}
			break;
		case R.id.locations:
			Intent i = new Intent("android.intent.action.SQLVIEW");
			startActivity(i);
			break;
			
		case R.id.cords:
			gpsLocation();
			break;
			
		case R.id.sqlDelete:
			boolean didItWork2 = true;
			try{
			Name = name.getText().toString();
			entry.open();
			entry.sqlDelete(Name);
			entry.close();
			}catch (Exception e) {
				didItWork = false;
			}finally {
				if(didItWork2){
					Dialog d = new Dialog(this);
					d.setTitle("Heck ya!");
					TextView tv = new TextView(this);
					tv.setTextSize(16);
	                tv.setTypeface(Typeface
	                                .create("Tahoma", Typeface.BOLD));
					tv.setText("success");
					d.setContentView(tv);
					d.show();
				}
			}
			break;

		}
		
	}
	public void gpsLocation() {
		
		LocationManager locManager;
		locManager = (LocationManager)getSystemService(Context.LOCATION_SERVICE); 
        locManager.requestLocationUpdates(LocationManager.GPS_PROVIDER,1000L,500.0f, locationListener);
        Location location = locManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
        
        if(location != null)                                
        {
        	
        double latitude = location.getLatitude();
        double longitude = location.getLongitude();
        
        } 
     
 }
	
	private void updateWithNewLocation(Location location) {
        EditText myLocationText = (EditText)findViewById(R.id.location);
        String latLongString = "";
        if (location != null) {
            double lat = location.getLatitude();
            double lng = location.getLongitude();
            latLongString = lat + "," + lng;


        } else {
            latLongString = "No location found";
        }
       
         myLocationText.setText(latLongString);
    }

    private final LocationListener locationListener = new LocationListener() {
        public void onLocationChanged(Location location) {
            updateWithNewLocation(location);
        }

        public void onProviderDisabled(String provider) {
            updateWithNewLocation(null);
        }

        public void onProviderEnabled(String provider) {
        }

        public void onStatusChanged(String provider, int status, Bundle extras) {
        }
    };

	
}
