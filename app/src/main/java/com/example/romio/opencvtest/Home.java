package com.example.romio.opencvtest;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.io.FileNotFoundException;
import java.io.InputStream;

public class Home extends AppCompatActivity {
    public static final int PICK_IMAGE = 1;
    Button btnCamera,btnGalery;
    Bitmap selectedImage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.home);

        btnCamera = (Button) findViewById(R.id.btnCamera);
        btnGalery = (Button) findViewById(R.id.btnGalery);

        btnCamera.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent cameraIntent = new Intent(getApplicationContext(),OpenCVCamera.class);
                startActivity(cameraIntent);
            }
        });

        btnGalery.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setType("image/*");
                intent.setAction(Intent.ACTION_PICK);
                startActivityForResult(intent, PICK_IMAGE);
            }
        });
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (requestCode == PICK_IMAGE) {
            Intent openCvImg = new Intent(getApplicationContext(),OpenCVCamera.class);
            openCvImg.putExtra("imgUri",data.getData().toString());
            startActivity(openCvImg);
        }
    }
}
