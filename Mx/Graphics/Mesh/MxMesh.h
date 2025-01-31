#pragma once
#ifndef MX_MESH_H_
#define MX_MESH_H_
#include <memory>
#include <vector>
#include "../../Vulkan/Buffers/MxVkBuffer.h"
#include "../../Resource/MxResourceBase.h"
#include "../../Math/MxVector.h"
#include "../../Utils/MxColor.h"
#include "../../Utils/MxArrayProxy.h"
#include "../../Utils/MxFlags.h"
#include "../../Definitions/MxCommonType.h"
#include "../MxVertexDeclaration.h"

namespace Mix {
	namespace Vulkan {
		class ShaderBase;
	}

	class Mesh :public ResourceBase {
		friend class Vulkan::ShaderBase;
	public:

		struct SubMesh {
			uint32_t baseVertex = 0;
			uint32_t firstIndex = 0;
			uint32_t indexCount = 0;
			MeshTopology topology = MeshTopology::Triangles_List;
		};

		using PositionType = Vector3f;
		using NormalType = Vector3f;
		using TangentType = Vector3f;
		using UV2DType = Vector2f;
		using ColorType = Color32;

		using Index32Type = uint32_t;
		using Index16Type = uint16_t;

		void setPositions(const std::vector<PositionType>& _vertices);
		void setPositions(std::vector<PositionType>&& _vertices);
		std::vector<PositionType>& getPositions();
		void getPositions(std::vector<PositionType>& _vertices) const;

		void setNormals(const std::vector<NormalType>& _normals);
		void setNormals(std::vector<NormalType>&& _normals);
		std::vector<NormalType>& getNormals();
		void getNormals(std::vector<NormalType>& _normals) const;

		void setTangents(const std::vector<TangentType>& _tangents);
		void setTangents(std::vector<TangentType>&& _tangents);
		std::vector<TangentType>& getTangents();
		void getTangents(std::vector<TangentType>& _tangents) const;

		void setUVs(UVChannel _channel, const std::vector<UV2DType>& _uvs);
		void setUVs(UVChannel _channel, std::vector<UV2DType>&& _uvs);
		std::vector<UV2DType>& getUVs(UVChannel _channel);
		void getUVs(UVChannel _channel, std::vector<UV2DType>& _uvs) const;

		void setColors(const std::vector<ColorType>& _colors);
		void setColors(std::vector<ColorType>&& _colors);
		std::vector<ColorType>& getColors();
		void getColors(std::vector<ColorType>& _colors) const;

		void setIndices(const std::vector<uint32_t>& _indices, MeshTopology _topology, uint32_t _submesh, uint32_t _baseVertex = 0);
		void setIndices(std::vector<uint32_t>&& _indices, MeshTopology _topology, uint32_t _submesh, uint32_t _baseVertex = 0);
		std::vector<uint32_t>& getIndices(uint32_t _submesh);
		void getIndices(std::vector<uint32_t>& _indices, uint32_t _submesh);

        void recalculateNormals();

        void recalculateTangents();

		void uploadMeshData(bool _markNoLongerReadable);

		void markNoLongerReadable();

		IndexFormat indexFormat() const { return mIndexFormat; }

		bool isReadable() const { return mMeshData != nullptr; }

		uint32_t subMeshCount()const { return static_cast<uint32_t>(mSubMeshes.size()); }

		MeshTopology getTopology(uint32_t _submesh) const;

		void clear();

		bool hasAttributes(Flags<VertexAttribute> _attributesMask) const { return mAttributes.isAllSet(_attributesMask); }

		bool hasIndices() const { return mHasIndex; }

		Flags<VertexAttribute> getAttributesFlags() const { return mAttributes; }

		std::shared_ptr<VertexDeclaration> getVertexDeclaration() const { return mVertexDeclaration; }

		void setSubMeshes(const std::vector<SubMesh>& _submeshes) { mSubMeshes = _submeshes; }

		void setSubMeshes(std::vector<SubMesh>&& _submeshes) { mSubMeshes = std::move(_submeshes); }


		// ---------- static method ----------

		static std::shared_ptr<Mesh> Create(const std::vector<std::byte>& _vertexData,
											Flags<VertexAttribute> _attributeFlags,
											const std::vector<std::byte>& _indexData,
											IndexFormat _format,
											const std::vector<SubMesh>& _subMeshes);

		static std::shared_ptr<Mesh> Create(const std::vector<std::byte>& _vertexData,
											Flags<VertexAttribute> _attributeFlags,
											const std::vector<SubMesh>& _subMeshes);

	private:

		struct MeshData {
			std::vector<PositionType> positions;
			std::vector<NormalType> normals;
			std::vector<TangentType> tangents;
			std::vector<UV2DType> uv0;
			std::vector<UV2DType> uv1;
			std::vector<ColorType> colors;

			std::optional<std::vector<std::vector<uint32_t>>> indexSet;
			std::optional<std::vector<SubMesh>> subMeshes;

			void createIndicesAndSubMeshIfNotExist();
		};

		//----------- Private field ----------

		Flags<VertexAttribute> mAttributes;
		std::shared_ptr<VertexDeclaration> mVertexDeclaration;
		std::shared_ptr<Vulkan::Buffer> mVertexBuffer;
		bool mHasIndex = false;
		IndexFormat mIndexFormat = IndexFormat::UInt16;
		std::shared_ptr<Vulkan::Buffer> mIndexBuffer;
		std::shared_ptr<MeshData> mMeshData;
		std::vector<SubMesh> mSubMeshes;

		// ---------- Private method ----------

		void createMeshDataIfNotExist();

		// ---------- static method ----------

		static bool SendToGPU(ArrayProxy<const std::byte, vk::DeviceSize> _vertexData,
							  ArrayProxy<const std::byte, vk::DeviceSize> _indexData,
							  std::shared_ptr<Vulkan::Buffer>& _outVertexBuffer,
							  std::shared_ptr<Vulkan::Buffer>& _outIndexBuffer);
	};

}

#endif
