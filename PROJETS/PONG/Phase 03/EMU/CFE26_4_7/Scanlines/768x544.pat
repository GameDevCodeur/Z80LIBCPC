; Written by Frederic Coste 2017

; Describes intensity for R,G,B components of each pixel
; Format: Pixel[X,Y]=R,G,B (from 0 to 255: 0=Black, 255=Bright)


Width=2
Height=2
; [0,0] [1,0]
; [0,1] [1,1]


NbAlias=1
Alias[0]=164 ; Scanline intensity

 
Pixel[0,0]=255,255,255
Pixel[0,1]=(0),(0),(0)

Pixel[1,0]=255,255,255
Pixel[1,1]=(0),(0),(0)
