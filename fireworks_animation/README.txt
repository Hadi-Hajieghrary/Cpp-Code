Author: Hadi.Hajieghrary@Gmail.Com
LinkedIn: https://www.linkedin.com/in/hadihajieghrary/


![](movie.gif)


---

Install SDL:

(ref: https://gist.github.com/BoredBored/3187339a99f7786c25075d4d9c80fad5)
#install sdl2
sudo apt install libsdl2-dev libsdl2-gfx-dev libsdl2-2.0-0 -y

#install sdl image  - if you want to display images
sudo apt install libjpeg-dev libwebp-dev libtiff5-dev libsdl2-image-dev libsdl2-image-2.0-0 -y

#install sdl mixer  - if you want sound
sudo apt install libmikmod-dev libfishsound1-dev libsmpeg-dev liboggz2-dev libflac-dev libfluidsynth-dev libsdl2-mixer-dev libsdl2-mixer-2.0-0 -y

#install sdl true type fonts - if you want to use text
sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y


-----
to create avi from bitmap pictures:
	ffmpeg -r 30 -i img%3d.bmp -vcodec huffyuv movie.avi

ro create gif from avi:
	ffmpeg -i movie.avi -r 15 -vf scale=512:-1 -ss 00:00:03 -to 00:00:06 movie.gif
