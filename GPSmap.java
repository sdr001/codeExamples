package com.morningside.edu;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.widget.TextView;

import com.google.android.maps.GeoPoint;

public class GPSmap extends Activity {

    GeoPoint geoPoint;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LocationManager locManager;
        setContentView(R.layout.gps);
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
        TextView myLocationText = (TextView)findViewById(R.id.realLocation);
        String latLongString = "";
        if (location != null) {
            double lat = location.getLatitude();
            double lng = location.getLongitude();
            latLongString = "Lat:" + lat + "\nLong:" + lng;



        } else {
            latLongString = "No location found";
        }
        Dialog d = new Dialog(this);
		d.setTitle("Heck ya!");
		TextView tv = new TextView(this);
		tv.setText("Your Current Position is:\n" +
                latLongString);
		d.setContentView(tv);
		d.show();
         myLocationText.setText("Your Current Position is:\n" +
                latLongString);
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
