/*
 *
 *
 *  Created on: 2013-6-27
 *      Author: scott.cgi
 */

#ifndef skeleton_data_h
#define skeleton_data_h

#include <stdbool.h>

#include "Mojoc/Toolkit/Utils/ArrayStrMap.h"
#include "Mojoc/Graphics/Draw/Color.h"
#include "Mojoc/Graphics/OpenGL/Mesh.h"
#include "Mojoc/Extension/TextureAtlas.h"

typedef struct SkeletonBoneData SkeletonBoneData;
struct  SkeletonBoneData
{
	SkeletonBoneData* parent;
	char*             name;
	float             length;
	float             x;
	float             y;
	float             rotationZ;
	float             scaleX;
	float             scaleY;
	bool              isInheritScale;
	bool              isInheritRotation;
};



typedef struct
{
	Color                               color[1];

	char*                               name;
	char*                               attachmentName;
	bool                                isAdditiveBlending;
	SkeletonBoneData*                   boneData;

	/** Each skin corresponding SkeletonAttachmentData may not exits */
	ArrayList(SkeletonAttachmentData*)* attachmentDataList;
}
SkeletonSlotData;



typedef struct
{
	char* name;
	char* stringValue;
	int   intValue;
	float floatValue;
}
SkeletonEventData;



typedef struct
{
	ArrayList(SkeletonTimeline*) timelineArr[1];

	char*                        name;
	float                        duration;
}
SkeletonAnimationData;



typedef struct
{
	/**
	 * The slotAttachmentMap key is slotData name,        value is attachmentDataMap
	 * the attachmentDataMap key is slot attachment name, value is SkeletonAttachmentData
	 */
	ArrayStrMap
	(
        slotName,
        ArrayStrMap(attachmentName, SkeletonAttachmentData*)*
    )
    slotAttachmentMap[1];
	char* name;
}
SkeletonSkinData;



//--------------------------------------------------------------------------------------------------


typedef enum
{
	skeleton_attachment_region,
	skeleton_attachment_mesh,
	skeleton_attachment_skinned_mesh,
	skeleton_attachment_boundingbox,
}
SkeletonAttachmentDataType;


typedef struct
{
	/** Attachment actual name*/
	char*                      name;
	/** Subclass attachment pointer */
	void*                      subPtr;
	SkeletonAttachmentDataType type;
}
SkeletonAttachmentData;


typedef struct
{
	SkeletonAttachmentData attachmentData[1];
	float                  x;
	float                  y;
	float                  rotationZ;
	float                  scaleX;
	float                  scaleY;
	float                  width;
	float                  height;

	/**
	 * Index in meshList from Skeleton
	 */
	int                    meshIndex;

	/**
	 * Index in Mesh from meshList
	 */
	int                    subMeshIndex;

	/**
	 * Texture atlas quad info
	 */
	Quad*                  quad;
}
SkeletonRegionAttachmentData;



typedef struct
{
	SkeletonAttachmentData attachmentData[1];
	Array(float)           vertexArr     [1];
}
SkeletonBoundingboxAttachmentData;



typedef struct
{
	SkeletonAttachmentData attachmentData[1];

	Array(float)           vertexArr  [1];
	Array(float)           uvArr      [1];
	/** Careful 4 byte aligned */
	Array(short)           triangleArr[1];

	/**
	 * Index in meshList from Skeleton
	 */
	int                    meshIndex;

	/**
	 * Index in Mesh from meshList
	 */
	int                    subMeshIndex;

	/**
	 * Texture atlas quad info
	 */
	Quad*                  quad;

	float                  width;
	float                  height;

	/**
	 * Whether convert uv data into TextureAtlas
	 */
	bool                   isUVMappedInTexture;
}
SkeletonMeshAttachmentData;


typedef struct
{
	SkeletonMeshAttachmentData meshAttachmentData[1];
	Array(int)                 boneArr           [1];
	Array(float)               weightArr         [1];
	Array(float)               weightVertexArr   [1];
}
SkeletonSkinnedMeshAttachmentData;



typedef enum
{
	skeleton_attachment_region_mesh_index       = offsetof(SkeletonRegionAttachmentData,      meshIndex),
	skeleton_attachment_mesh_mesh_index         = offsetof(SkeletonMeshAttachmentData,        meshIndex),
	skeleton_attachment_skinned_mesh_mesh_index = offsetof(SkeletonSkinnedMeshAttachmentData, meshAttachmentData) + skeleton_attachment_mesh_mesh_index,
}
SkeletonAttachmentMeshOffset;

extern SkeletonAttachmentMeshOffset skeletonAttachmentMeshOffset[3];

typedef enum
{
	skeleton_attachment_region_sub_mesh_index       = offsetof(SkeletonRegionAttachmentData,      subMeshIndex),
	skeleton_attachment_mesh_sub_mesh_index         = offsetof(SkeletonMeshAttachmentData,        subMeshIndex),
	skeleton_attachment_skinned_mesh_sub_mesh_index = offsetof(SkeletonSkinnedMeshAttachmentData, meshAttachmentData) + skeleton_attachment_mesh_sub_mesh_index,
}
SkeletonAttachmentSubMeshOffset;


extern SkeletonAttachmentSubMeshOffset skeletonAttachmentSubMeshOffset[3];


//--------------------------------------------------------------------------------------------------

typedef struct
{
	ArrayStrMap(boneName,      SkeletonBoneData*)         boneDataMap     [1];
	ArrayStrMap(slotName,      SkeletonSlotData*)         slotDataMap     [1];
	ArrayStrMap(skinName,      SkeletonSkinData*)         skinDataMap     [1];
	ArrayStrMap(animationName, SkeletonAnimationDataMap*) animationDataMap[1];
	ArrayStrMap(eventName,     SkeletonEventDataMap*)     eventDataMap    [1];

	SkeletonSkinData*                      skinDataDefault;
	/** All SkeletonBoneData order same as in JOSN */
	Array(SkeletonBoneData*)*              boneDataOrderArr;
	/** All SkeletonSlotData order same as in JOSN */
	Array(SkeletonSlotData*)*              slotDataOrderArr;

	/** All slot attachments */
	ArrayList(SkeletonAttachmentData*)     attachmentDataList[1];

	TextureAtlas*                          textureAtlas;
	float                                  width;
	float                                  height;
	const char*                            filePath;
}
SkeletonData;


struct ASkeletonData
{
	/** Scales the bones, images, and animations as they are loaded, default 1.0 */
	float scale;

	/**
	 * Get SkeletonData by filePath, not found will create one
	 */
	SkeletonData*           (*Get)                        (const char*       filePath);
	void                    (*Release)                    (SkeletonData*     skeletonData);
	SkeletonAttachmentData* (*GetAttachmentDataBySkinData)(SkeletonSkinData* skinData, const char* slotName, const char* attachmentName);
};


extern struct ASkeletonData ASkeletonData[1];


#endif