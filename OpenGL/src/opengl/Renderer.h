#pragma once
#include "OpenGL.h"
#include "ForwardDecl.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Picker.h"
#include "Shader.h"
#include "math/AABB.h"

namespace OpenGL
{
    using namespace Visual;
    class Renderer
    {
        struct Coordinates
        {
            float mX[6] = { 0, 0, 0, 100000, 0, 0 };
            float mY[6] = { 0, 0, 0, 0, 100000, 0 };
            float mZ[6] = { 0, 0, 0, 0, 0, 100000 };
            std::vector<float> mXZ;
            std::vector<unsigned int> mIndexXZ;
            unsigned int mIndex[2] = { 0, 1 };
            VertexBuffer mVbX, mVbY, mVbZ, mVbXZ;
            VertexArray mVaX, mVaY, mVaZ, mVaXZ;
            IndexBuffer mIb, mIBXZ;
            mutable Shader mCoordShader;
            Coordinates()
            {
                mCoordShader.AttachProgram("res/shaders/DefaultCoord.shader");
                mIb.SetData(&mIndex[0], 2);
                VertexBufferLayout layout;
                layout.Push<float>(3);
                mVbX.SetData(&mX[0], sizeof(float) * 6);
                mVaX.AddBuffer(mVbX, layout);
                mVaX.UnBind();
				mVbY.SetData(&mY[0], sizeof(float) * 6);
				mVaY.AddBuffer(mVbY, layout);
				mVaY.UnBind();
				mVbZ.SetData(&mZ[0], sizeof(float) * 6);
				mVaZ.AddBuffer(mVbZ, layout);
				mVaZ.UnBind();
                mIb.UnBind();
                unsigned int xzIndex = 0;
                for (int x = 0; x <= 8; ++x) 
                {
                    mXZ.push_back(x - 4);
                    mXZ.push_back(0);
                    mXZ.push_back(-4);
                    mIndexXZ.push_back(xzIndex++);

					mXZ.push_back(x - 4);
					mXZ.push_back(0);
					mXZ.push_back(4);
                    mIndexXZ.push_back(xzIndex++);
                }
				for (int z = 0; z <= 8; ++z)
				{
					mXZ.push_back(-4);
					mXZ.push_back(0);
					mXZ.push_back(z - 4);
                    mIndexXZ.push_back(xzIndex++);

					mXZ.push_back(4);
					mXZ.push_back(0);
					mXZ.push_back(z - 4);
                    mIndexXZ.push_back(xzIndex++);
				}
                mIBXZ.SetData(&mIndexXZ[0], mIndexXZ.size());
				mVbXZ.SetData(&mXZ[0], sizeof(float) * mXZ.size());
                mVaXZ.AddBuffer(mVbXZ, layout);
                mVaXZ.UnBind();
            }
        };

        struct BBoxDrawer 
        {
            mutable Shader mBBoxShader;
            VertexBuffer mVb;
            VertexArray mVa;
            IndexBuffer mIb;
            std::vector<float> mVertecesData;
            unsigned int mIndex[24] =
            { 
                0, 1, 1, 2, 2, 3, 3, 0,
                0, 4, 1, 5, 2, 6, 3, 7, 
                4, 5, 5, 6, 6, 7, 7, 4
            };
            BBoxDrawer(const BBox &bbox)
            {
                
                Vec3 minPoint = bbox.minPoint, maxPoint = bbox.maxPoint;
                std::vector<Vec3> vertices;
                vertices.push_back(Vec3(minPoint)),
				vertices.push_back(Vec3(maxPoint.x, minPoint.y, minPoint.z)),
				vertices.push_back(Vec3(maxPoint.x, minPoint.y, maxPoint.z)),
				vertices.push_back(Vec3(minPoint.x, minPoint.y, maxPoint.z)),
				vertices.push_back(Vec3(minPoint.x, maxPoint.y, minPoint.z)),
				vertices.push_back(Vec3(maxPoint.x, maxPoint.y, minPoint.z)),
				vertices.push_back(Vec3(maxPoint.x, maxPoint.y, maxPoint.z)),
				vertices.push_back(Vec3(minPoint.x, maxPoint.y, maxPoint.z));
                
                for (auto& vertex : vertices)
                {
                    mVertecesData.push_back(vertex.x);
                    mVertecesData.push_back(vertex.y);
                    mVertecesData.push_back(vertex.z);
                }


                mBBoxShader.AttachProgram("res/shaders/DefaultCoord.shader");
                mVb.SetData(&mVertecesData[0], sizeof(float) * mVertecesData.size());
                mIb.SetData(&mIndex[0], 24);
				VertexBufferLayout layout;
				layout.Push<float>(3);
                mVa.AddBuffer(mVb, layout);
                mVa.UnBind();
                mIb.UnBind();
            }

        };

		struct BoarderDrawer
        {
			mutable Shader mBoarderShader;
			VertexBuffer mVb;
            VertexArray mVa;
            IndexBuffer mIb;
            unsigned int mIndex[6] = { 0, 1, 2, 0, 2, 3 };
			float data[12] = { -1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0 };
            BoarderDrawer()
			{
                mVa.Bind();
                mBoarderShader.AttachProgram("res/shaders/DefaultBoarder.shader");
				mVb.Bind();
				mVb.SetData(&data[0], 12 * sizeof(float));
                mIb.Bind();
                mIb.SetData(&mIndex[0], 6);
                VertexBufferLayout layout;
                layout.Push<float>(3);
                mVa.AddBuffer(mVb, layout);
				mVb.UnBind();
                mIb.UnBind();
			}
            void Draw(int width, Vec2i res, const Renderer *pRenderer) const
            {
                mBoarderShader.Bind();
                mBoarderShader.SetUniform1i("width", width);
                mBoarderShader.SetUniform2f("res", res.x, res.y);
                pRenderer->Draw(mVa, mIb, mBoarderShader);
                mBoarderShader.UnBind();
            }
		};
    private:
        Camera* mpCamera;
        Coordinates mCoords;
        int mWidth, mHeight;
        SSAO* mpSSAO;

    public:
        Renderer() = delete;
        Renderer(int w, int h) : mWidth(w), mHeight(h){}
        void Init();
        void Resize(int width, int height);
        void SetSize(int width, int height);
        void SetCamera(Camera* pCamera);
        void SetSSAO(SSAO* pSSAO);
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void DrawFaces(const DrawableObject &object) const;
        void DrawFaces(const Scene& scene) const;
        void DrawWireFrame(const DrawableObject& object) const;
        void DrawWireFrame(const Scene& scene) const;
        void DrawObjectId(const DrawableObject& obejct) const;
        void DrawObjectId(const Scene& scene) const;
        void DrawBBox(const DrawableObject& object, const Vec4& lineColor = Vec4(1.0f, 0.0f, 0.0f, 0.5f)) const;
        void DrawBBox(const Scene& scene, const Vec4 &lineColor = Vec4(1.0f, 0.0f, 0.0f, 0.5f)) const;
        void Clear() const;
        void EnableDepthTest() const;
        void DisableDepthTest() const;
        void EnableBlend() const;
        void DisableBlend() const;
        void DrawCoordinates() const;
        void DrawBoarder(int w) const;
        void DrawGbuffer(const DrawableObject& object, FrameBuffer& frameBuffer) const;
        void DrawGbuffer(const Scene& scene, FrameBuffer& frameBuffer) const;
        void DrawToFrameBuffer(const DrawableObject& object, FrameBuffer& frameBuffer) const;
        void DrawToFrameBuffer(const Scene& scene, FrameBuffer& frameBuffer) const;
        void DebugDraw(DrawQuad& quad, Texture2D &texture) const;
        void DebugDraw(DrawQuad& quad) const;
        void Flush() const { glFlush(); }

        inline Camera* GetCamera() const { return mpCamera; }
        inline SSAO* GetSSAO() const { return mpSSAO; }
        static unsigned int IsReady();

    };

}