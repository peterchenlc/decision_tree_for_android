package com.peterchen.decisiontree;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView predictResult = findViewById(R.id.predict_result);
        Button trainBtn = findViewById(R.id.train_btn);
        trainBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                train();
            }
        });

        Button predictBtn = findViewById(R.id.predict_btn);
        predictBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int result = predict();
                predictResult.setText("Predict Result: " + result);
            }
        });
    }

    public native void train();

    public native int predict();
}
