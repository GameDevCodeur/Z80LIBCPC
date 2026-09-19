; Written by Frederic Coste 2017

; Describes intensity for R,G,B components of each pixel
; Format: Pixel[X,Y]=R,G,B (from 0 to 255: 0=Black, 255=Bright)


Width=3
Height=3
; [0,0] [1,0] [2,0]
; [0,1] [1,1] [2,1]
; [0,2] [1,2] [2,2]


NbAlias=3
Alias[0]=255 ; Beam
Alias[1]=224 ; Beside beam
Alias[2]=164  ; Grid

 
Pixel[0,0]=(1),(2),(2)
Pixel[0,1]=(0),(2),(2)
Pixel[0,2]=(1),(2),(2)

Pixel[1,0]=(2),(1),(2)
Pixel[1,1]=(2),(0),(2)
Pixel[1,2]=(2),(1),(2)

Pixel[2,0]=(2),(2),(1)
Pixel[2,1]=(2),(2),(0)
Pixel[2,2]=(2),(2),(1)
