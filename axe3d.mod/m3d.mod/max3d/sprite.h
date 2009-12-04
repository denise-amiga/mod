/*
Max3D
Copyright (c) 2008, Mark Sibly
All rights reserved.

Redistribution and use in source and binary forms, with or without
conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

* Neither the name of Max3D's copyright owner nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "scene.h"

class CSpriteSurface;

class CSprite : public CEntity{
public:
	CSprite();
	~CSprite();
	
	void SetMaterial( CMaterial *material );
	CMaterial *Material();
	
	virtual void OnRenderWorld();

private:
	CSprite( CSprite *sprite,CCopier *copier );
	
	CSprite *OnCopy( CCopier *copier ){ return new CSprite( this,copier ); }

	CSpriteSurface *_surface;
};

class CSpriteSurface : public CSurface{
public:
	CSpriteSurface();
	~CSpriteSurface();
	
	vector<CSprite*> &Instances(){ return _instances; }

	void OnRenderCamera( CCamera *camera );
	void OnRenderInstances( const CHull &bounds );
	void OnClearInstances();

private:
	int _capacity;
	CVertexBuffer *_vertexBuffer;
	CIndexBuffer *_indexBuffer;
	
	vector<CSprite*> _instances;
};

#endif
