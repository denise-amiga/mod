// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/Billboard>
#include <osg/BoundingSphere>
#include <osg/CopyOp>
#include <osg/Drawable>
#include <osg/Matrix>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osg/Vec3>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(osg::Billboard::Mode)
	I_DeclaringFile("osg/Billboard");
	I_EnumLabel(osg::Billboard::POINT_ROT_EYE);
	I_EnumLabel(osg::Billboard::POINT_ROT_WORLD);
	I_EnumLabel(osg::Billboard::AXIAL_ROT);
END_REFLECTOR

TYPE_NAME_ALIAS(std::vector< osg::Vec3 >, osg::Billboard::PositionList)

BEGIN_OBJECT_REFLECTOR(osg::Billboard)
	I_DeclaringFile("osg/Billboard");
	I_BaseType(osg::Geode);
	I_Constructor0(____Billboard,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::Billboard &, x, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____Billboard__C5_Billboard_R1__C5_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "clone an object of the same type as the node. ",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "return a clone of a node, with Object* return type. ",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "return true if this and obj are of the same kind of object. ",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "return the name of the node's class type. ",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "return the name of the node's library. ",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "Visitor Pattern : calls the apply method of a NodeVisitor with this node's type. ",
	          "");
	I_Method1(void, setMode, IN, osg::Billboard::Mode, mode,
	          Properties::NON_VIRTUAL,
	          __void__setMode__Mode,
	          "Set the billboard rotation mode. ",
	          "");
	I_Method0(osg::Billboard::Mode, getMode,
	          Properties::NON_VIRTUAL,
	          __Mode__getMode,
	          "Get the billboard rotation mode. ",
	          "");
	I_Method1(void, setAxis, IN, const osg::Vec3 &, axis,
	          Properties::NON_VIRTUAL,
	          __void__setAxis__C5_Vec3_R1,
	          "Set the rotation axis for the billboard's child Drawables. ",
	          "Only utilized when mode==AXIAL_ROT. ");
	I_Method0(const osg::Vec3 &, getAxis,
	          Properties::NON_VIRTUAL,
	          __C5_Vec3_R1__getAxis,
	          "Get the rotation axis. ",
	          "");
	I_Method1(void, setNormal, IN, const osg::Vec3 &, normal,
	          Properties::NON_VIRTUAL,
	          __void__setNormal__C5_Vec3_R1,
	          "This normal defines child Drawables' front face direction when unrotated. ",
	          "");
	I_Method0(const osg::Vec3 &, getNormal,
	          Properties::NON_VIRTUAL,
	          __C5_Vec3_R1__getNormal,
	          "Get the front face direction normal. ",
	          "");
	I_Method2(void, setPosition, IN, unsigned int, i, IN, const osg::Vec3 &, pos,
	          Properties::NON_VIRTUAL,
	          __void__setPosition__unsigned_int__C5_Vec3_R1,
	          "Set the specified child Drawable's position. ",
	          "");
	I_Method1(const osg::Vec3 &, getPosition, IN, unsigned int, i,
	          Properties::NON_VIRTUAL,
	          __C5_Vec3_R1__getPosition__unsigned_int,
	          "Get the specified child Drawable's position. ",
	          "");
	I_Method1(void, setPositionList, IN, osg::Billboard::PositionList &, pl,
	          Properties::NON_VIRTUAL,
	          __void__setPositionList__PositionList_R1,
	          "Set the list of pivot point positions. ",
	          "");
	I_Method0(osg::Billboard::PositionList &, getPositionList,
	          Properties::NON_VIRTUAL,
	          __PositionList_R1__getPositionList,
	          "Get the list of pivot point positions. ",
	          "");
	I_Method0(const osg::Billboard::PositionList &, getPositionList,
	          Properties::NON_VIRTUAL,
	          __C5_PositionList_R1__getPositionList,
	          "Get a const list of pivot point positions. ",
	          "");
	I_Method1(bool, addDrawable, IN, osg::Drawable *, gset,
	          Properties::VIRTUAL,
	          __bool__addDrawable__Drawable_P1,
	          "Add a Drawable with a default position of Vec3(0,0,0). ",
	          "Call the base-class Geode::addDrawble() to add the given Drawable gset as a child. If Geode::addDrawable() returns true, add the default position to the pivot point position list and return true. Otherwise, return false. ");
	I_Method2(bool, addDrawable, IN, osg::Drawable *, gset, IN, const osg::Vec3 &, pos,
	          Properties::VIRTUAL,
	          __bool__addDrawable__Drawable_P1__C5_Vec3_R1,
	          "Add a Drawable with a specified position. ",
	          "Call the base-class Geode::addDrawble() to add the given Drawable gset as a child. If Geode::addDrawable() returns true, add the given position pos to the pivot point position list and return true. Otherwise, return false. ");
	I_Method1(bool, removeDrawable, IN, osg::Drawable *, gset,
	          Properties::VIRTUAL,
	          __bool__removeDrawable__Drawable_P1,
	          "Remove a Drawable and its associated position. ",
	          "If gset is a child, remove it, decrement its reference count, remove its pivot point position. and return true. Otherwise, return false. ");
	I_Method3(bool, computeMatrix, IN, osg::Matrix &, modelview, IN, const osg::Vec3 &, eye_local, IN, const osg::Vec3 &, pos_local,
	          Properties::NON_VIRTUAL,
	          __bool__computeMatrix__Matrix_R1__C5_Vec3_R1__C5_Vec3_R1,
	          "",
	          "");
	I_Method0(osg::BoundingSphere, computeBound,
	          Properties::VIRTUAL,
	          __BoundingSphere__computeBound,
	          "Compute the bounding sphere around Node's geometry or children. ",
	          "This method is automatically called by getBound() when the bounding sphere has been marked dirty via dirtyBound(). ");
	I_ProtectedMethod0(void, updateCache,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__updateCache,
	                   "",
	                   "");
	I_SimpleProperty(const osg::Vec3 &, Axis, 
	                 __C5_Vec3_R1__getAxis, 
	                 __void__setAxis__C5_Vec3_R1);
	I_SimpleProperty(osg::Billboard::Mode, Mode, 
	                 __Mode__getMode, 
	                 __void__setMode__Mode);
	I_SimpleProperty(const osg::Vec3 &, Normal, 
	                 __C5_Vec3_R1__getNormal, 
	                 __void__setNormal__C5_Vec3_R1);
	I_IndexedProperty(const osg::Vec3 &, Position, 
	                  __C5_Vec3_R1__getPosition__unsigned_int, 
	                  __void__setPosition__unsigned_int__C5_Vec3_R1, 
	                  0);
	I_SimpleProperty(osg::Billboard::PositionList &, PositionList, 
	                 __PositionList_R1__getPositionList, 
	                 __void__setPositionList__PositionList_R1);
END_REFLECTOR

STD_VECTOR_REFLECTOR(std::vector< osg::Vec3 >)

