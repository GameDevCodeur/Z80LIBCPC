; Written by Frederic Coste 2020

; Format: Pixel[X,Y]=R,G,B (from 0 to 255: 0=Entirely Masked, 255=Not Masked)
;
; [0,0] [1,0] [2,0]
; [0,1] [1,1] [2,1]
; [0,2] [1,2] [2,2]
; [0,3] [1,3] [2,3]
 
Width=3
Height=4

NbAlias=2
Alias[0]=128
Alias[1]=80


Pixel[0,0]=255,(0),(1)
Pixel[0,1]=255,(0),(1)
Pixel[0,2]=255,(0),(1)
Pixel[0,3]=(0),(1),(1)

Pixel[1,0]=(1),255,(0)
Pixel[1,1]=(1),255,(0)
Pixel[1,2]=(1),255,(0)
Pixel[1,3]=(1),(0),(1)

Pixel[2,0]=(0),(1),255
Pixel[2,1]=(0),(1),255
Pixel[2,2]=(0),(1),255
Pixel[2,3]=(1),(1),(0)
