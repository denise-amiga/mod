Type TMiniSurface Extends TSurface

	' no of vertices and triangles in surface

	Field no_verts=0
	Field no_tris=0
	
	' arrays containing vertex and triangle info
	
	Field tris:Short[0]
	Field vert_coords#[0]
	Field vert_tex_coords0#[0]
	Field vert_tex_coords1#[0]
	Field vert_norm#[0]
	Field vert_col#[0]
	
	' arrays containing vertex bone no and weights info - used by animated meshes only
	
	Field vert_bone1_no:Int[0] ' stores bone no - bone no used to reference bones[] array belonging to TMesh
	Field vert_bone2_no:Int[0]
	Field vert_bone3_no:Int[0]
	Field vert_bone4_no:Int[0]
	Field vert_weight1:Float[0]
	Field vert_weight2:Float[0]
	Field vert_weight3:Float[0]
	Field vert_weight4:Float[0]

	' brush applied to surface

	Field brush:TMiniBrush=New TMiniBrush
	
	' vbo
	
	Field vbo_id[7]
	
	' misc vars
	
	Field vert_array_size=1
	Field tri_array_size=1
	Field vmin=1000000 ' used for trimming verts from b3d files
	Field vmax=0 ' used for trimming verts from b3d files

	' reset flag - this is set when mesh shape is changed in TSurface and TMesh
	Field reset_vbo=-1 ' (-1 = all)
	
	' used by Compare to sort array, and TMesh.Update to enable/disable alpha blending
	Field alpha_enable=False

	Method New()
	
		If LOG_NEW
			DebugLog "New TSurface"
		EndIf
	
	End Method
	
	Method Delete()
	
		FreeVBO()
		
		If LOG_DEL
			DebugLog "Del TSurface"
		EndIf
			
	End Method
	
	Method GetSurfaceBrush:TBrush()
		Return brush
	End Method

	' used to sort surfaces into alpha order. used by TMesh.Update
	Method Compare(other:Object)
	
		If TMiniSurface(other)
		
			If alpha_enable>TMiniSurface(other).alpha_enable Then Return 1
			If alpha_enable<TMiniSurface(other).alpha_enable Then Return -1
	
		EndIf
		
		Return 0
	
	End Method
						
	Method Copy:TSurface()
	
		Local surf:TMiniSurface=New TMiniSurface
		
		surf.no_verts=no_verts
		surf.no_tris=no_tris
		
		surf.tris=tris[..]
		surf.vert_coords#=vert_coords#[..]
		surf.vert_tex_coords0#=vert_tex_coords0#[..]
		surf.vert_tex_coords1#=vert_tex_coords1#[..]
		surf.vert_norm#=vert_norm[..]
		surf.vert_col#=vert_col[..]
		
		surf.vert_bone1_no=vert_bone1_no[..]
		surf.vert_bone2_no=vert_bone2_no[..]
		surf.vert_bone3_no=vert_bone3_no[..]
		surf.vert_bone4_no=vert_bone4_no[..]
		surf.vert_weight1=vert_weight1[..]
		surf.vert_weight2=vert_weight2[..]
		surf.vert_weight3=vert_weight3[..]
		surf.vert_weight4=vert_weight4[..]
		
		If brush<>Null
			surf.brush=TMiniBrush(brush.Copy())
		EndIf

		surf.vert_array_size=vert_array_size
		surf.tri_array_size=tri_array_size
		surf.vmin=vmin
		surf.vmax=vmax
		
		surf.reset_vbo=-1

		Return surf
	
	End Method
	
	Method PaintSurface(red:TBrush)
	
		Local bru:TMiniBrush=TMiniBrush(red)

		If brush=Null Then brush=New TMiniBrush
		
		brush.no_texs=bru.no_texs
		brush.name$=bru.name$
		brush.red#=bru.red#
		brush.green#=bru.green#
		brush.blue#=bru.blue#
		brush.alpha#=bru.alpha#
		brush.shine#=bru.shine#
		brush.blend=bru.blend
		brush.fx=bru.fx
		For Local i=0 To 7
			brush.tex[i]=bru.tex[i]
		Next
	
	End Method
	
	Method ClearSurface(clear_verts=True,clear_tris=True)
	
		If clear_verts
		
			no_verts=0
			
			vert_coords#=vert_coords#[..0]
			vert_tex_coords0#=vert_tex_coords0#[..0]
			vert_tex_coords1#=vert_tex_coords1#[..0]
			vert_norm#=vert_norm#[..0]
			vert_col#=vert_col#[..0]
			
			vert_array_size=1
		
		EndIf
		
		If clear_tris
		
			no_tris=0
			
			tris=tris[..0]

			tri_array_size=1
		
		EndIf
		
		' mesh shape has changed - update reset flag
		reset_vbo=-1 ' (-1 = all)
	
	End Method
			
	Method AddVertex(x#,y#,z#,u#=0.0,v#=0.0,w#=0.0)
		
		no_verts=no_verts+1

		' resize arrays
		
		If no_verts>=vert_array_size
		
			Repeat
				vert_array_size=vert_array_size*2
			Until vert_array_size>no_verts
			
			Local vas=vert_array_size
		
			vert_coords=vert_coords[..vas*3]
			vert_tex_coords0=vert_tex_coords0[..vas*2]
			vert_tex_coords1=vert_tex_coords1[..vas*2]
			vert_norm=vert_norm[..vas*3]
			vert_col=vert_col[..vas*4]
		
		EndIf
		
		Local vxi=(no_verts*3)-3
		Local vyi=(no_verts*3)-2
		Local vzi=(no_verts*3)-1		
		Local vui=(no_verts*2)-2
		Local vvi=(no_verts*2)-1
		Local vri=(no_verts*4)-4
		Local vgi=(no_verts*4)-3
		Local vbi=(no_verts*4)-2
		Local vai=(no_verts*4)-1
		
		vert_coords[vxi]=x#
		vert_coords[vyi]=y#
		vert_coords[vzi]=-z# ' ***ogl***

		vert_tex_coords0[vui]=u#
		vert_tex_coords0[vvi]=v#
		
		' default vertex colours
		vert_col#[vri]=1.0
		vert_col#[vgi]=1.0
		vert_col#[vbi]=1.0
		vert_col#[vai]=1.0
				
		Return no_verts-1
	
	End Method
	
	Method AddTriangle(v0,v1,v2)
	
		no_tris=no_tris+1
		
		' resize array
		
		If no_tris>=tri_array_size
		
			Repeat
				tri_array_size=tri_array_size*2
			Until tri_array_size>no_tris
		
			Local tas=tri_array_size
		
			tris=tris[..tas*3]
			
		EndIf
		
		Local v0i=(no_tris*3)-3
		Local v1i=(no_tris*3)-2
		Local v2i=(no_tris*3)-1	
	
		tris[v0i]=v2
		tris[v1i]=v1
		tris[v2i]=v0
		
		' mesh shape has changed - update reset flag
		reset_vbo:|1|2|16
		
		Return no_tris
	
	End Method
	
	Method CountVertices()
	
		Return no_verts
	
	End Method
	
	Method CountTriangles()
	
		Return no_tris
	
	End Method
	
	Method VertexCoords(vid,x#,y#,z#)
	
		vid=vid*3
		vert_coords#[vid]=x#
		vert_coords#[vid+1]=y#
		vert_coords#[vid+2]=z#*-1 ' ***ogl***
		
		' mesh shape has changed - update reset flag
		reset_vbo:|1
	
	End Method
			
	Method VertexColor(vid,r#,g#,b#,a#=1.0)
	
		vid=vid*4
		vert_col#[vid]=r#/255.0
		vert_col#[vid+1]=g#/255.0
		vert_col#[vid+2]=b#/255.0
		vert_col#[vid+3]=a#
		
		' mesh state has changed - update reset flags
		reset_vbo:|8

	End Method
	
	Method VertexNormal(vid,nx#,ny#,nz#)
	
		vid=vid*3
		vert_norm#[vid]=nx#
		vert_norm#[vid+1]=ny#
		vert_norm#[vid+2]=nz#*-1 ' ***ogl***
		
		' mesh state has changed - update reset flags
		reset_vbo:|4

	End Method
	
	Method VertexTexCoords(vid,u#,v#,w#=0.0,coord_set=0)
	
		vid=vid*2
		
		If coord_set=0
		
			vert_tex_coords0#[vid]=u#
			vert_tex_coords0#[vid+1]=v#

		EndIf
		
		If coord_set=1
		
			vert_tex_coords1#[vid]=u#
			vert_tex_coords1#[vid+1]=v#
		
		EndIf
		
		' mesh state has changed - update reset flags
		reset_vbo:|2


	End Method
		
	Method VertexX#(vid)
	
		Return vert_coords[vid*3]

	End Method

	Method VertexY#(vid)
	
		Return vert_coords[(vid*3)+1]

	End Method
	
	Method VertexZ#(vid)
	
		Return -vert_coords[(vid*3)+2] ' ***ogl***

	End Method
	
	Method VertexRed#(vid)
	
		Return vert_col[vid*4]*255.0

	End Method
	
	Method VertexGreen#(vid)
	
		Return vert_col[(vid*4)+1]*255.0

	End Method
	
	Method VertexBlue#(vid)
	
		Return vert_col[(vid*4)+2]*255.0

	End Method
	
	Method VertexAlpha#(vid)
	
		Return vert_col[(vid*4)+3]

	End Method
	
	Method VertexNX#(vid)
	
		Return vert_norm[vid*3]

	End Method
	
	Method VertexNY#(vid)
	
		Return vert_norm[(vid*3)+1]

	End Method
	
	Method VertexNZ#(vid)
	
		Return -vert_norm[(vid*3)+2] ' ***ogl***

	End Method
	
	Method VertexU#(vid,coord_set=0)
	
		If coord_set=0 Then Return vert_tex_coords0[vid*2]
		If coord_set=1 Then Return vert_tex_coords1[vid*2]

	End Method
	
	Method VertexV#(vid,coord_set=0)
	
		If coord_set=0 Then Return vert_tex_coords0[(vid*2)+1]
		If coord_set=1 Then Return vert_tex_coords1[(vid*2)+1]

	End Method
	
	Method VertexW#(vid,coord_set=0)
	
		Return 0

	End Method
	
	Method TriangleVertex(tri_no,corner)
	
		Local vid[3]
	
		tri_no=(tri_no+1)*3
		vid[0]=tris[tri_no-1]
		vid[1]=tris[tri_no-2]
		vid[2]=tris[tri_no-3]
		
		Return vid[corner]
	
	End Method
		
	Method UpdateNormals()
	
		Rem
		Local norm_map:TMap=New TMap

		For Local t=0 Until no_tris
			
			Local tri_no=(t+1)*3
			
			Local v0=tris[tri_no-3]
			Local v1=tris[tri_no-2]
			Local v2=tris[tri_no-1]
	
			Local ax#=vert_coords[v1*3+0]-vert_coords[v0*3+0]
			Local ay#=vert_coords[v1*3+1]-vert_coords[v0*3+1]
			Local az#=vert_coords[v1*3+2]-vert_coords[v0*3+2]
	
			Local bx#=vert_coords[v2*3+0]-vert_coords[v1*3+0]
			Local by#=vert_coords[v2*3+1]-vert_coords[v1*3+1]
			Local bz#=vert_coords[v2*3+2]-vert_coords[v1*3+2]
	
			Local nx#=(ay#*bz#)-(az#*by#) ' TriangleNX#(t)
			Local ny#=(az#*bx#)-(ax#*bz#) ' TriangleNX#(t)
			Local nz#=(ax#*by#)-(ay#*bx#) ' TriangleNX#(t)
					
			For Local c=0 Until 3
			
				Local v=TriangleVertex(t,c)
			
				Local vx#=vert_coords[v*3] ' surf.VertexX(v) 
				Local vy#=vert_coords[(v*3)+1] ' surf.VertexY(v)
				Local vz#=vert_coords[(v*3)+2] ' surf.VertexZ(v)
				
				Local vert:TVector=New TVector
				vert.x=vx
				vert.y=vy
				vert.z=vz

				Local norm:TVector=TVector( norm_map.ValueForKey( vert ) )
				
				If norm
				
					norm.x:+nx
					norm.y:+ny
					norm.z:+nz
					
				Else
				
					Local vec:TVector=New TVector
					vec.x=nx
					vec.y=ny
					vec.z=nz
					
					norm_map.Insert vert,vec
					
				EndIf
				
			Next
			
		Next
		
		For Local norm:TVector=EachIn norm_map.Values()
		
			Local d#=1/Sqr(norm.x*norm.x+norm.y*norm.y+norm.z*norm.z)
			norm.x:*d
			norm.y:*d
			norm.z:*d

		Next
	
		For Local v=0 Until no_verts

			Local vx#=vert_coords[v*3] ' VertexX(v)
			Local vy#=vert_coords[(v*3)+1] ' VertexY(v)
			Local vz#=vert_coords[(v*3)+2] ' VertexZ(v)
			
			Local vert:TVector=New TVector
			vert.x=vx
			vert.y=vy
			vert.z=vz
			
			Local norm:TVector=TVector( norm_map.ValueForKey( vert ) )
			If Not norm Continue
			
			vert_norm#[v*3+0]=norm.x ' VertexNormal(v,norm.x,norm.y,norm.z)
			vert_norm#[v*3+1]=norm.y ' VertexNormal(v,norm.x,norm.y,norm.z)
			vert_norm#[v*3+2]=norm.z ' VertexNormal(v,norm.x,norm.y,norm.z)
					
		Next
		End Rem
	
	End Method
		
	Method TriangleNX#(tri_no)

		Local v0=TriangleVertex(tri_no,0)
		Local v1=TriangleVertex(tri_no,1)
		Local v2=TriangleVertex(tri_no,2)
	
		'Local ax#=VertexX#(v1)-VertexX#(v0)
		Local ay#=VertexY#(v1)-VertexY#(v0)
		Local az#=VertexZ#(v1)-VertexZ#(v0)
		
		'Local bx#=VertexX#(v2)-VertexX#(v1)
		Local by#=VertexY#(v2)-VertexY#(v1)
		Local bz#=VertexZ#(v2)-VertexZ#(v1)
		
		Return (ay#*bz#)-(az#*by#)
		
	End Method

	Method TriangleNY#(tri_no)
	
		Local v0=TriangleVertex(tri_no,0)
		Local v1=TriangleVertex(tri_no,1)
		Local v2=TriangleVertex(tri_no,2)

		Local ax#=VertexX#(v1)-VertexX#(v0)
		'Local ay#=VertexY#(v1)-VertexY#(v0)
		Local az#=VertexZ#(v1)-VertexZ#(v0)
		
		Local bx#=VertexX#(v2)-VertexX#(v1)
		'Local by#=VertexY#(v2)-VertexY#(v1)
		Local bz#=VertexZ#(v2)-VertexZ#(v1)
	
		Return (az#*bx#)-(ax#*bz#)
			
	End Method

	Method TriangleNZ#(tri_no)
	
		Local v0=TriangleVertex(tri_no,0)
		Local v1=TriangleVertex(tri_no,1)
		Local v2=TriangleVertex(tri_no,2)
		
		Local ax#=VertexX#(v1)-VertexX#(v0)
		Local ay#=VertexY#(v1)-VertexY#(v0)
		'Local az#=VertexZ#(v1)-VertexZ#(v0)
		
		Local bx#=VertexX#(v2)-VertexX#(v1)
		Local by#=VertexY#(v2)-VertexY#(v1)
		'Local bz#=VertexZ#(v2)-VertexZ#(v1)
		
		Return (ax#*by#)-(ay#*bx#)
		
	End Method
	
	Method UpdateVBO()
	
		If vbo_id[0]=0
			glGenBuffersARB(6,vbo_id)
		EndIf
	
		If reset_vbo=-1 Then reset_vbo=1|2|4|8|16
	
		If reset_vbo&1
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbo_id[0])
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,(no_verts*3*4),vert_coords,GL_STATIC_DRAW_ARB)
		EndIf
		
		If reset_vbo&2
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbo_id[1])
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,(no_verts*2*4),vert_tex_coords0,GL_STATIC_DRAW_ARB)
	
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbo_id[2])
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,(no_verts*2*4),vert_tex_coords1,GL_STATIC_DRAW_ARB)	
		EndIf
		
		If reset_vbo&4
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbo_id[3])
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,(no_verts*3*4),vert_norm,GL_STATIC_DRAW_ARB)
		EndIf
		
		If reset_vbo&8
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbo_id[4])
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,(no_verts*4*4),vert_col,GL_STATIC_DRAW_ARB)
		EndIf
			
		If reset_vbo&16
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,vbo_id[5])
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,no_tris*3*2,tris,GL_STATIC_DRAW_ARB)
		EndIf
		
		reset_vbo=False
	
	End Method
	
	Method FreeVBO()
	
		If vbo_id[0]<>0 
			glDeleteBuffersARB(6,vbo_id)
		EndIf
	
	End Method
	
	' removes a tri from a surface
	Function RemoveTri(surf:TSurface,tri)
	
		Local no_tris=surf.CountTriangles()
		Local tris[no_tris,3]
		
		For Local t=0 To no_tris-1
	
			tris[t,0]=surf.TriangleVertex(t,0)
			tris[t,1]=surf.TriangleVertex(t,1)	
			tris[t,2]=surf.TriangleVertex(t,2)		
				
		Next
		
		surf.ClearSurface False,True
	
		For Local t=0 To no_tris-1
	
			Local v0=tris[t,0]
			Local v1=tris[t,1]
			Local v2=tris[t,2]
		
			If t<>tri Then surf.AddTriangle(v0,v1,v2)
			
		Next
		
	End Function
	
	' removes redundent verts (non-working)
	Function RemoveVerts(surf:TSurface)
	
		Local no_tris=surf.CountTriangles()
		Local tris[no_tris,3]
		
		Local no_verts=surf.CountVertices()
		
		Local vert_used[no_verts]
		Local vert_info:Float[no_verts,15]
		Local new_vert_index[no_verts]
		
		For Local t=0 To no_tris-1
	
			For Local i=0 To 2
	
				tris[t,i]=surf.TriangleVertex(t,i)
				
				vert_used[tris[t,i]]=True
			
				vert_info[tris[t,i],0]=surf.VertexX(i)
				vert_info[tris[t,i],1]=surf.VertexY(i)
				vert_info[tris[t,i],2]=surf.VertexZ(i)
				vert_info[tris[t,i],3]=surf.VertexNX(i)
				vert_info[tris[t,i],4]=surf.VertexNY(i)
				vert_info[tris[t,i],5]=surf.VertexNZ(i)
				vert_info[tris[t,i],6]=surf.VertexRed(i)
				vert_info[tris[t,i],7]=surf.VertexGreen(i)
				vert_info[tris[t,i],8]=surf.VertexBlue(i)
				vert_info[tris[t,i],9]=surf.VertexU(i,0)
				vert_info[tris[t,i],10]=surf.VertexV(i,0)
				vert_info[tris[t,i],11]=surf.VertexW(i,0)
				vert_info[tris[t,i],12]=surf.VertexU(i,1)
				vert_info[tris[t,i],13]=surf.VertexV(i,1)
				vert_info[tris[t,i],14]=surf.VertexW(i,1)
					
			Next
					
		Next
		
		surf.ClearSurface True,True
	
		For Local v=0 To no_verts-1
		
			If vert_used[v]=True
		
				Local new_index=surf.AddVertex(vert_info[v,0],vert_info[v,1],vert_info[v,2])
				surf.VertexNormal new_index,vert_info[v,3],vert_info[v,4],vert_info[v,5]
				surf.VertexColor new_index,vert_info[v,6],vert_info[v,7],vert_info[v,8]
				surf.VertexTexCoords new_index,vert_info[v,9],vert_info[v,10],vert_info[v,11],0
				surf.VertexTexCoords new_index,vert_info[v,12],vert_info[v,13],vert_info[v,14],1
				new_vert_index[v]=new_index
		
			EndIf
			
		Next
		
		For Local t=0 To no_tris-1
	
			Local v0=new_vert_index[tris[t,0]]
			Local v1=new_vert_index[tris[t,1]]
			Local v2=new_vert_index[tris[t,2]]
			
			surf.AddTriangle(v0,v1,v2)
			
		Next
		
	End Function

End Type
