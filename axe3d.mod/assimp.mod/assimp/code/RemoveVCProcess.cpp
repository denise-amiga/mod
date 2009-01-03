/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2008, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the following 
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/
/** @file Implementation of the post processing step to remove
 *        any parts of the mesh structure from the imported data.
*/

#include "AssimpPCH.h"
#include "RemoveVCProcess.h"

using namespace Assimp;


// ------------------------------------------------------------------------------------------------
// Constructor to be privately used by Importer
RemoveVCProcess::RemoveVCProcess()
{
}

// ------------------------------------------------------------------------------------------------
// Destructor, private as well
RemoveVCProcess::~RemoveVCProcess()
{
	// nothing to do here
}

// ------------------------------------------------------------------------------------------------
// Returns whether the processing step is present in the given flag field.
bool RemoveVCProcess::IsActive( unsigned int pFlags) const
{
	return (pFlags & aiProcess_RemoveComponent) != 0;
}

// ------------------------------------------------------------------------------------------------
// Small helper function to delete all elements in a T** aray using delete
template <typename T>
inline void ArrayDelete(T**& in, unsigned int& num)
{
	for (unsigned int i = 0; i < num; ++i)
		delete in[i];

	delete[] in;
	in = NULL;
	num = 0;
}

//// ------------------------------------------------------------------------------------------------
//// Updates the node graph - removes all nodes which have the "remove" flag set and the 
//// "don't remove" flag not set. Nodes with meshes are never deleted.
//bool UpdateNodeGraph(aiNode* node,std::list<aiNode*>& childsOfParent,bool root)
//{
//	register bool b = false;
//
//	std::list<aiNode*> mine;
//	for (unsigned int i = 0; i < node->mNumChildren;++i)
//	{
//		if(UpdateNodeGraph(node->mChildren[i],mine,false))
//			b = true;
//	}
//
//	// somewhat tricky ... mNumMeshes must be originally 0 and MSB2 may not be set,
//	// so we can do a simple comparison against MSB here
//	if (!root && AI_RC_UINT_MSB == node->mNumMeshes )
//	{
//		// this node needs to be removed
//		if(node->mNumChildren)
//		{
//			childsOfParent.insert(childsOfParent.end(),mine.begin(),mine.end());
//
//			// set all children to NULL to make sure they are not deleted when we delete ourself
//			for (unsigned int i = 0; i < node->mNumChildren;++i)
//				node->mChildren[i] = NULL;
//		}
//		b = true;
//		delete node;
//	}
//	else
//	{
//		AI_RC_UNMASK(node->mNumMeshes);
//		childsOfParent.push_back(node);
//
//		if (b)
//		{
//			// reallocate the array of our children here
//			node->mNumChildren = (unsigned int)mine.size();
//			aiNode** const children = new aiNode*[mine.size()];
//			aiNode** ptr = children;
//
//			for (std::list<aiNode*>::iterator it = mine.begin(), end = mine.end();
//				 it != end; ++it)
//			{
//				*ptr++ = *it;
//			}
//			delete[] node->mChildren;
//			node->mChildren = children;
//			return false;
//		}
//	}
//	return b;
//}

// ------------------------------------------------------------------------------------------------
// Executes the post processing step on the given imported data.
void RemoveVCProcess::Execute( aiScene* pScene)
{
	DefaultLogger::get()->debug("RemoveVCProcess begin");
	bool bHas = false,bMasked = false;

	mScene = pScene;

	// handle animations
	if ( configDeleteFlags & aiComponent_ANIMATIONS)
	{

		bHas = true;
		ArrayDelete(pScene->mAnimations,pScene->mNumAnimations);
	}

	// handle textures
	if ( configDeleteFlags & aiComponent_TEXTURES)
	{
		bHas = true;
		ArrayDelete(pScene->mTextures,pScene->mNumTextures);
	}

	// handle materials
	if ( configDeleteFlags & aiComponent_MATERIALS && pScene->mNumMaterials)
	{
		bHas = true;
		for (unsigned int i = 1;i < pScene->mNumMaterials;++i)
			delete pScene->mMaterials[i];

		MaterialHelper* helper = (MaterialHelper*) pScene->mMaterials[0];
		helper->Clear();

		// gray
		aiColor3D clr(0.6f,0.6f,0.6f);
		helper->AddProperty(&clr,1,AI_MATKEY_COLOR_DIFFUSE);

		// add a small ambient color value
		clr = aiColor3D(0.05f,0.05f,0.05f);
		helper->AddProperty(&clr,1,AI_MATKEY_COLOR_AMBIENT);

		aiString s;
		s.Set("Dummy_MaterialsRemoved");
		helper->AddProperty(&s,AI_MATKEY_NAME);
	}

	// handle light sources
	if ( configDeleteFlags & aiComponent_LIGHTS)
	{
		bHas =  true;
		ArrayDelete(pScene->mLights,pScene->mNumLights);
	}

	// handle camneras
	if ( configDeleteFlags & aiComponent_CAMERAS)
	{
		bHas = true;
		ArrayDelete(pScene->mCameras,pScene->mNumCameras);
	}

	// handle meshes
	if (configDeleteFlags & aiComponent_MESHES)
	{
		bHas = true;
		ArrayDelete(pScene->mMeshes,pScene->mNumMeshes);
	}
	else
	{
		for( unsigned int a = 0; a < pScene->mNumMeshes; a++)
		{
			if(	ProcessMesh( pScene->mMeshes[a]))
				bHas = true;
		}
	}


	// now check whether the result is still a full scene
	if (!pScene->mNumMeshes || !pScene->mNumMaterials)
	{
		pScene->mFlags |= AI_SCENE_FLAGS_INCOMPLETE;
		DefaultLogger::get()->debug("Setting AI_SCENE_FLAGS_INCOMPLETE flag");
	}

	if (bHas)DefaultLogger::get()->info("RemoveVCProcess finished. Data structure cleanup has been done.");
	else DefaultLogger::get()->debug("RemoveVCProcess finished. Nothing to be done ...");
}

// ------------------------------------------------------------------------------------------------
// Setup configuration properties for the step
void RemoveVCProcess::SetupProperties(const Importer* pImp)
{
	configDeleteFlags = pImp->GetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,0x0);
	if (!configDeleteFlags)
	{
		DefaultLogger::get()->warn("RemoveVCProcess: AI_CONFIG_PP_RVC_FLAGS is zero.");
	}
}

// ------------------------------------------------------------------------------------------------
// Executes the post processing step on the given imported data.
bool RemoveVCProcess::ProcessMesh(aiMesh* pMesh)
{
	bool ret = false;

	// if all materials have been deleted let the material
	// index of the mesh point to the created default material
	if ( configDeleteFlags & aiComponent_MATERIALS)
		pMesh->mMaterialIndex = 0;

	// handle normals
	if (configDeleteFlags & aiComponent_NORMALS && pMesh->mNormals)
	{
		delete[] pMesh->mNormals;
		pMesh->mNormals = NULL;
		ret = true;
	}

	// handle tangents and bitangents
	if (configDeleteFlags & aiComponent_TANGENTS_AND_BITANGENTS && pMesh->mTangents)
	{
		delete[] pMesh->mTangents;
		pMesh->mTangents = NULL;

		delete[] pMesh->mBitangents;
		pMesh->mBitangents = NULL;
		ret = true;
	}

	// handle texture coordinates
	register bool b = (0 != (configDeleteFlags & aiComponent_TEXCOORDS));
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		if (!pMesh->mTextureCoords[i])break;
		if (configDeleteFlags & aiComponent_TEXCOORDSn(i) || b)
		{
			delete pMesh->mTextureCoords[i];
			pMesh->mTextureCoords[i] = NULL;
			ret = true;

			if (!b)
			{
				// collapse the rest of the array
				unsigned int a;
				for (a = i+1; a < AI_MAX_NUMBER_OF_TEXTURECOORDS;++a)
				{
					pMesh->mTextureCoords[a-1] = pMesh->mTextureCoords[a];
				}
				pMesh->mTextureCoords[AI_MAX_NUMBER_OF_TEXTURECOORDS-1] = NULL;
			}
		}
	}

	// handle vertex colors
	b = (0 != (configDeleteFlags & aiComponent_COLORS));
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		if (!pMesh->mColors[i])break;
		if (configDeleteFlags & aiComponent_COLORSn(i) || b)
		{
			delete pMesh->mColors[i];
			pMesh->mColors[i] = NULL;
			ret = true;

			if (!b)
			{
				// collapse the rest of the array
				unsigned int a;
				for (a = i+1; a < AI_MAX_NUMBER_OF_COLOR_SETS;++a)
				{
					pMesh->mColors[a-1] = pMesh->mColors[a];
				}
				pMesh->mColors[AI_MAX_NUMBER_OF_COLOR_SETS-1] = NULL;
			}
		}
	}

	// handle bones
	if (configDeleteFlags & aiComponent_BONEWEIGHTS && pMesh->mBones)
	{
		ArrayDelete(pMesh->mBones,pMesh->mNumBones);
		ret = true;
	}
	return ret;
}
