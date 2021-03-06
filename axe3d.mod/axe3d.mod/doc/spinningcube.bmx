' spinning cube

' change Flip -> bbFlip for sdk testing

Strict

Import axe3d.m3d

'Import axe3d.minib3d
'Import axe3d.b3dsdk
'Import sidesign.minib3d

'Import axe3d.drivers - not yet implemented


Global brush:TBrush
Global cube:TEntity
Global cam:TEntity
Global light:TEntity

Graphics3D 800,600

'brush=CreateBrush(255,0,0)

brush=LoadBrush("logo.png",3)

cube=CreateCube()
ScaleEntity cube,1,1,1

PaintEntity cube,brush

cam=CreateCamera()
MoveEntity cam,0,0,-20

CameraClsColor cam,185,0,40

light=CreateLight()
MoveEntity light,-25,25,-50

While Not KeyHit(KEY_ESCAPE)
	TurnEntity cube,0,1,0
	UpdateWorld
	RenderWorld
	Flip
'	bbFlip
Wend
