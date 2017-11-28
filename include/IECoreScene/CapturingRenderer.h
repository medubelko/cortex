//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010-2013, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef IECORESCENE_CAPTURINGRENDERER_H
#define IECORESCENE_CAPTURINGRENDERER_H

#include "IECoreScene/Export.h"
#include "IECoreScene/Renderer.h"

namespace IECoreScene
{

IE_CORE_FORWARDDECLARE( Primitive )
IE_CORE_FORWARDDECLARE( Group )

/// The CapturingRenderer doesn't render images at all, but instead turns what it is
/// given into IECore Groups and Primitives which can then be used for further processing.
/// Currently it doesn't support any calls before worldBegin(), as there is no IECore
/// class to represent an entire scene. The world generated by the renderer can be retrieved
/// as an IECore::Group using the world() method.
/// \ingroup renderingGroup
class IECORESCENE_API CapturingRenderer : public Renderer
{

	public :

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( CapturingRenderer, CapturingRendererTypeId, Renderer );

		CapturingRenderer();
		~CapturingRenderer() override;

		/// \par Implementation specific options :
		////////////////////////////////////////////////////////////
		///
		/// \li <b>"cp:objectFilter" StringVectorData []</b><br>
		/// Use this to specify a list of object filters. Procedurals and primitives
		/// where the "name" renderer attribute does not match one of the filters will
		/// be skipped. The "name" attribute is interpreted as a path, and if a path is
		/// requested, all parents of that path will be output as well. For example, if
		/// you set the option to [ "/root/leftArm/thumb" ], "/root", "/root/leftArm"
		/// and "/root/leftArm/thumb" will be output. Objects without a name will always
		/// be output. The filters also support wildcards, so you can specify things like
		/// "/root/wheel*Rim/bolt", "/root/torso/rib*", and "/root/*", the last of which
		/// will output "/root" and all its descendants.
		void setOption( const std::string &name, IECore::ConstDataPtr value ) override;

		IECore::ConstDataPtr getOption( const std::string &name ) const override;

		void camera( const std::string &name, const IECore::CompoundDataMap &parameters ) override;
		void display( const std::string &name, const std::string &type, const std::string &data, const IECore::CompoundDataMap &parameters ) override;

		void worldBegin() override;
		void worldEnd() override;

		void transformBegin() override;
		void transformEnd() override;
		void setTransform( const Imath::M44f &m ) override;
		void setTransform( const std::string &coordinateSystem ) override;
		Imath::M44f getTransform() const override;
		Imath::M44f getTransform( const std::string &coordinateSystem ) const override;
		void concatTransform( const Imath::M44f &m ) override;
		void coordinateSystem( const std::string &name ) override;

		void attributeBegin() override;
		void attributeEnd() override;
		/// \par Implementation specific procedural attributes :
		////////////////////////////////////////////////////////////
		///
		/// \li <b>"cp:procedural:reentrant" BoolData true</b><br>
		/// When true, procedurals may be evaluated in multiple parallel threads.
		/// When false they will be evaluated from the thread they were specified from.

		void setAttribute( const std::string &name, IECore::ConstDataPtr value ) override;
		IECore::ConstDataPtr getAttribute( const std::string &name ) const override;
		void shader( const std::string &type, const std::string &name, const IECore::CompoundDataMap &parameters ) override;
		void light( const std::string &name, const std::string &handle, const IECore::CompoundDataMap &parameters ) override;
		void illuminate( const std::string &lightHandle, bool on ) override;

		void motionBegin( const std::set<float> &times ) override;
		void motionEnd() override;

		void points( size_t numPoints, const PrimitiveVariableMap &primVars ) override;
		void disk( float radius, float z, float thetaMax, const PrimitiveVariableMap &primVars ) override;
		void curves( const IECore::CubicBasisf &basis, bool periodic, IECore::ConstIntVectorDataPtr numVertices, const PrimitiveVariableMap &primVars ) override;
		void text( const std::string &font, const std::string &text, float kerning = 1.0f, const PrimitiveVariableMap &primVars=PrimitiveVariableMap() ) override;
		void sphere( float radius, float zMin, float zMax, float thetaMax, const PrimitiveVariableMap &primVars ) override;
		void image( const Imath::Box2i &dataWindow, const Imath::Box2i &displayWindow, const PrimitiveVariableMap &primVars ) override;
		void mesh( IECore::ConstIntVectorDataPtr vertsPerFace, IECore::ConstIntVectorDataPtr vertIds, const std::string &interpolation, const PrimitiveVariableMap &primVars ) override;
		void nurbs( int uOrder, IECore::ConstFloatVectorDataPtr uKnot, float uMin, float uMax, int vOrder, IECore::ConstFloatVectorDataPtr vKnot, float vMin, float vMax, const PrimitiveVariableMap &primVars ) override;
		void patchMesh( const IECore::CubicBasisf &uBasis, const IECore::CubicBasisf &vBasis, int nu, bool uPeriodic, int nv, bool vPeriodic, const PrimitiveVariableMap &primVars ) override;
		void geometry( const std::string &type, const IECore::CompoundDataMap &topology, const PrimitiveVariableMap &primVars ) override;

		void procedural( ProceduralPtr proc ) override;

		void instanceBegin( const std::string &name, const IECore::CompoundDataMap &parameters ) override;
		void instanceEnd() override;
		void instance( const std::string &name ) override;

		IECore::DataPtr command( const std::string &name, const IECore::CompoundDataMap &parameters ) override;

		void editBegin( const std::string &editType, const IECore::CompoundDataMap &parameters ) override;
		void editEnd() override;

		/// Returns the world that was captured.
		/// \todo If we had a class for representing whole Scenes (including stuff before worldBegin)
		/// then we could have a scene() method instead.
		ConstGroupPtr world();

	private :

		class Implementation;

		boost::shared_ptr<Implementation> m_implementation;

};

IE_CORE_DECLAREPTR( CapturingRenderer )

} // namespace IECoreScene

#endif // IECORESCENE_CAPTURINGRENDERER_H
