package com.jni.ndk

import android.Manifest
import android.os.Bundle
import android.os.Environment
import android.support.v4.app.ActivityCompat
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.activity_main.view.*
import java.io.File
import java.util.jar.JarFile

class MainActivity : AppCompatActivity() {



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermissions()
        //加密
        button1.setOnClickListener {
            val normal_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"abc.jpg"
            val crypt_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"crypt_abc.jpg"
            Cryptor.crypt(normal_path,crypt_path)
        }
        //解密
        button2.setOnClickListener {

            val crypt_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"crypt_abc.jpg"
            val decrypt_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"decrypt_abc.jpg"
            Cryptor.decrypt(crypt_path,decrypt_path)

        }

        //拆分
        button3.setOnClickListener {
            val diff_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"def.mp4"
            val pattern_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"pattern_def_%d.mp4"
            FileNative.diff(diff_path,pattern_path,3)
            Log.d("TAG","分割成功")
        }

        //合并
        button4.setOnClickListener {
            val pattern_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"pattern_def_%d.mp4"
            val merge_path = Environment.getExternalStorageDirectory().absolutePath+ File.separatorChar+"merge_def.mp4"
            FileNative.merge(pattern_path,3,merge_path)
            Log.d("TAG","合并成功")
        }

        button5.setOnClickListener {
            PosixThread.posixThread()

        }

        button6.setOnClickListener {
            val input =  File(Environment.getExternalStorageDirectory(),"input.mp4").absolutePath
            val output =  File(Environment.getExternalStorageDirectory(),"output_1280x720_yuv420p.yuv").absolutePath
            DecodeYUV.decode(input,output)
        }
        button7.setOnClickListener {
            val input =  File(Environment.getExternalStorageDirectory(),"input.mp4").absolutePath
            FFmpegPlayer.nicePlay(input,videoView.holder.surface)
        }

        PosixThread.init()
    }

    private fun requestPermissions(){
        ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE), 321)
    }
    override fun onDestroy() {
        super.onDestroy()
        PosixThread.destroy()
    }
}
