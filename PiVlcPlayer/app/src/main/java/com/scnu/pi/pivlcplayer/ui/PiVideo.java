package com.scnu.pi.pivlcplayer.ui;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.FrameLayout;

import com.scnu.pi.pivlcplayer.R;
import com.yyl.videolist.VideoView;

/**
 * Created by lzj on 2017/3/23.
 */

public class PiVideo extends FrameLayout {

    private VideoView videoPlayerOne;
    private PiVideoView videoPlayerTwo;

    public PiVideo(Context context) {
        super(context);
        initView(context);

    }

    public PiVideo(Context context, AttributeSet attrs) {
        super(context,attrs);
        initView(context);

    }

    public PiVideo(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView(context);
    }


    private void initView(Context context) {

        LayoutInflater.from(context).inflate(R.layout.view_pi_video, this);
        videoPlayerOne = (VideoView) findViewById(R.id.pi_video_one);
        videoPlayerTwo = (PiVideoView) findViewById(R.id.pi_video_two);
    }


    public void playVideo(String pathOne,String pathTwo) {
        videoPlayerOne.startPlay(pathOne);
        videoPlayerTwo.startPlay(pathTwo);
    }
    public void playOne(String path){
        System.out.println("one");
        videoPlayerOne.startPlay(path);

    }

    public void playTwo(String path){
        System.out.println("two");
        videoPlayerTwo.startPlay(path);

    }

}
