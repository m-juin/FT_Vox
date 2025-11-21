#ifndef __Frustum_HPP__
#define __Frustum_HPP__

#include "MathGraphicalLib/Matrix/Matrix4.hpp"
#include "MathGraphicalLib/Vectors/Operations.hpp"

#include <vector>

namespace Vox::Front::Rendering::Frustum
{
	struct Frustum
	{
			using Vector3Float = MGL::Vectors::Vector3<float>;
			struct FrustumPlane
			{
					Vector3Float normal;
					float distance;

					FrustumPlane(const Vector3Float &point, const Vector3Float &normal_)
					{
						normal = MGL::Vectors::Operations::Normalize(normal_);
						// CORRECTION: La distance doit être -dot(normal, point) pour l'équation standard du plan
						distance = -MGL::Vectors::Operations::Dot(normal, point);

						// DEBUG
						std::cout << "Plane construction - point: " << point << " normal: " << normal
								  << " calculated distance: " << distance << std::endl;
					}
					FrustumPlane() {};

					bool isOnPositiveSide(const Vector3Float &point) const
					{
						return MGL::Vectors::Operations::Dot(normal, point) > distance;
					}

					// Distance signée d'un point au plan
					float signedDistance(const Vector3Float &point) const
					{
						return MGL::Vectors::Operations::Dot(normal, point) - distance;
					}
			};

			FrustumPlane top;
			FrustumPlane bot;

			FrustumPlane right;
			FrustumPlane left;

			FrustumPlane far;
			FrustumPlane near;

			static Frustum ExtractFrustum(const MGL::Matrix::Matrix4 &vp)
			{
				Frustum f;

				// Left
				f.left.normal[0] = vp(0, 3) + vp(0, 0);
				f.left.normal[1] = vp(1, 3) + vp(1, 0);
				f.left.normal[2] = vp(2, 3) + vp(2, 0);
				f.left.distance = vp(3, 3) + vp(3, 0);

				// Right
				f.right.normal[0] = vp(0, 3) - vp(0, 0);
				f.right.normal[1] = vp(1, 3) - vp(1, 0);
				f.right.normal[2] = vp(2, 3) - vp(2, 0);
				f.right.distance = vp(3, 3) - vp(3, 0);

				// Bottom
				f.bot.normal[0] = vp(0, 3) + vp(0, 1);
				f.bot.normal[1] = vp(1, 3) + vp(1, 1);
				f.bot.normal[2] = vp(2, 3) + vp(2, 1);
				f.bot.distance = vp(3, 3) + vp(3, 1);

				// Top
				f.top.normal[0] = vp(0, 3) - vp(0, 1);
				f.top.normal[1] = vp(1, 3) - vp(1, 1);
				f.top.normal[2] = vp(2, 3) - vp(2, 1);
				f.top.distance = vp(3, 3) - vp(3, 1);

				// Near
				f.near.normal[0] = vp(0, 2);
				f.near.normal[1] = vp(1, 2);
				f.near.normal[2] = vp(2, 2);
				f.near.distance = vp(3, 2);

				// Far
				f.far.normal[0] = vp(0, 3) - vp(0, 2);
				f.far.normal[1] = vp(1, 3) - vp(1, 2);
				f.far.normal[2] = vp(2, 3) - vp(2, 2);
				f.far.distance = vp(3, 3) - vp(3, 2);

				// Normalize
				auto normalizePlane = [](FrustumPlane &p)
				{
					float len = MGL::Vectors::Operations::Length(p.normal);
					p.normal /= len;
					p.distance /= len;
				};

				normalizePlane(f.left);
				normalizePlane(f.right);
				normalizePlane(f.top);
				normalizePlane(f.bot);
				normalizePlane(f.near);
				normalizePlane(f.far);

				return f;
			}

			Vector3Float Intersect3Planes(const Frustum::FrustumPlane &p1, const Frustum::FrustumPlane &p2,
										  const Frustum::FrustumPlane &p3)
			{
				Vector3Float n1 = p1.normal;
				Vector3Float n2 = p2.normal;
				Vector3Float n3 = p3.normal;

				float d1 = p1.distance;
				float d2 = p2.distance;
				float d3 = p3.distance;

				Vector3Float n2xn3 = MGL::Vectors::Operations::Cross(n2, n3);
				Vector3Float n3xn1 = MGL::Vectors::Operations::Cross(n3, n1);
				Vector3Float n1xn2 = MGL::Vectors::Operations::Cross(n1, n2);

				float denom = MGL::Vectors::Operations::Dot(n1, n2xn3);

				// le point d'intersection
				Vector3Float result = (-d1 * n2xn3 - d2 * n3xn1 - d3 * n1xn2) / denom;

				return result;
			}

			struct FrustumCorners
			{
					Vector3Float ntl, ntr, nbl, nbr;
					Vector3Float ftl, ftr, fbl, fbr;
			};

			FrustumCorners GetFrustumCorners()
			{
				FrustumCorners C;

				// Near plane corners
				C.ntl = Intersect3Planes(this->top, this->left, this->near);
				C.ntr = Intersect3Planes(this->top, this->right, this->near);
				C.nbl = Intersect3Planes(this->bot, this->left, this->near);
				C.nbr = Intersect3Planes(this->bot, this->right, this->near);

				// Far plane corners
				C.ftl = Intersect3Planes(this->top, this->left, this->far);
				C.ftr = Intersect3Planes(this->top, this->right, this->far);
				C.fbl = Intersect3Planes(this->bot, this->left, this->far);
				C.fbr = Intersect3Planes(this->bot, this->right, this->far);

				return C;
			}
	};
} // namespace Vox::Front::Rendering::Frustum
#endif // __Frustum_HPP__