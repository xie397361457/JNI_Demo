package com.jni.ndk

import android.Manifest
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.support.v4.app.ActivityCompat
import kotlinx.android.synthetic.main.activity_play_sound.*
import org.fmod.FMOD
import java.io.File

class PlaySoundActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_play_sound)
        requestPermissions()
        FMOD.init(this)
        buttonA.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_sound(path)
        }
        buttonB.setOnClickListener {
            SoundUntil.close_sound()
        }
        buttonC.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_effect(path,SoundUntil.MY_FMOD_DSP_TYPE_LUO_LI)
        }
        buttonD.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_effect(path,SoundUntil.MY_FMOD_DSP_TYPE_DA_SHU)
        }
        buttonE.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_effect(path,SoundUntil.MY_FMOD_DSP_TYPE_JINGSONG)
        }
        buttonF.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_effect(path,SoundUntil.MY_FMOD_DSP_TYPE_HUISHENG)
        }
        buttonG.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_effect(path,SoundUntil.MY_FMOD_DSP_TYPE_KONGLING)
        }
        buttonH.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_effect(path,SoundUntil.MY_FMOD_DSP_TYPE_GUAOGUAI)
        }
        buttonJ.setOnClickListener {
            val path = Environment.getExternalStorageDirectory().absolutePath + File.separatorChar + "岑宁儿 - 追光者 [mqms2].mp3"
            SoundUntil.play_effect(path,SoundUntil.MY_FMOD_DSP_TYPE_XIAOHUANGREN)
        }
    }

    private fun requestPermissions(){
        ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.RECORD_AUDIO), 321)
    }

    override fun onDestroy() {
        super.onDestroy()
        FMOD.close()
    }
}
